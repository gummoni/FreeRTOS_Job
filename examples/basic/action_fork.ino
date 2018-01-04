class ledModule {
  private:
    queue que;

    void blink(void* p) {
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

    void write(void* p) {
      int arg = *((int*)p->arg);
      Serial.println(arg);
    }

  public:
    void init() {
      Serial.begin(19200);
      taskStart(&que, "serialModule", 4096, 3, 1);
    }

    job* writeAsync(int* value) {
      return que.invoke(this, write, value, NULL);
    }
};


class DINModule {
  private:
    queue que;

    void read(void* p) {
      int* ret = (int*)p->result;
      *ret = digitalRead(D1);
    }

  public:
    void init() {
      pinMode(D1, INPUT);
      taskStart(&que, "DINModule", 4096, 3, 1);
    }

    job* readAsync(int* result) {
      return que.invoke(this, read, NULL, result);
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

      job* j1 = d.readAsync(&v);
      p->action(j1);

      job* j2[] = {
        l.blinkAsync(),
        s.writeAsync(&v)
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
