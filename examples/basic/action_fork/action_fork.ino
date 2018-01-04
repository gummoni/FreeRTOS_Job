#include "queue.h"

#define D1  (1)
#define LED (2)

class ledModule {
  private:
    queue que;

    static void blink(job* p) {
      digitalWrite(LED, true);
      delay(500);
      digitalWrite(LED, false);
      delay(500);
    }

  public:
    void init() {
      pinMode(LED, OUTPUT);
      taskStart(&que, "ledModule", 4096, 3, 1);
    }

    job* blinkAsync() {
      return que.invoke(this, blink, NULL, NULL);
    }
};

class serialModule {
  private:
    queue que;

    static void write(job* p) {
      int arg = *((int*)p->arg);
      Serial.println(arg);
    }

  public:
    void init() {
      Serial.begin(19200);
      taskStart(&que, "serialModule", 4096, 3, 1);
    }

    job* writeAsync(int* value) {
      return que.invoke(this, write, (void*)value, NULL);
    }
};


class DINModule {
  private:
    queue que;

    static void read(job* p) {
      int* ret = (int*)p->result;
      *ret = digitalRead(D1);
    }

  public:
    void init() {
      pinMode(D1, INPUT);
      taskStart(&que, "DINModule", 4096, 3, 1);
    }

    job* readAsync(int* result) {
      return que.invoke(this, read, NULL, (void*)result);
    }
};

class example02 {
   private:
    queue que;
    ledModule l;
    serialModule s;
    DINModule d;

    static void func(job* p) {
      int v;
      example02* sender = (example02*)p->sender;

      job* j1 = sender->d.readAsync(&v);
      p->action(j1);

      job* j2[] = {
        sender->l.blinkAsync(),
        sender->s.writeAsync(&v)
      };
      p->fork(j2, 2);
    }

  public:
    void init() {
      l.init();
      s.init();
      d.init();
      taskStart(&que, "example02", 4096, 3, 1);
    }

    job* funcAsync() {
      return que.invoke(this, func, NULL, NULL);
    }
};


static example02 ex;

void setup() {
  ex.init();
}

void loop() {
  ex.funcAsync()->wait();
}