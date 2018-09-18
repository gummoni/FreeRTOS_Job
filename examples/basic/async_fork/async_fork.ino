#include "job.h"
#include "dispatcher.h"

#define LED (LED_BUILTIN)
#define delay2(value) vTaskDelay(value / portTICK_PERIOD_MS)


////////////////////////////////////////////////////////////////////////
// serial task
////////////////////////////////////////////////////////////////////////
class serialWorker : dispatcher {
  private:
    static void print(job* p) {
      char* message = (char*)p->arg;
      Serial.println(message);
    }

  public:
    void init() {
      Serial.begin(9600);
#ifdef ESP_H
      start("serialWorker", 256, 3, 1);
#else
      start("serialWorker", 64, 3);
#endif
    }

    job* printAsync(job* parent, char* message) {
      return invoke(parent, print, this, message);
    }
};


////////////////////////////////////////////////////////////////////////
// led task
////////////////////////////////////////////////////////////////////////
class ledWorker : dispatcher {
  private:
    static void blink1(job* p) {
      digitalWrite(LED, HIGH);
      delay2(500);
      digitalWrite(LED, LOW);
      delay2(500);
    };

    static void blink2(job* p) {
      digitalWrite(LED, HIGH);
      delay2(800);
      digitalWrite(LED, LOW);
      delay2(200);
    };

  public:
    void init() {
      pinMode(LED, OUTPUT);
#ifdef ESP_H
      start("ledWorker", 256, 3, 1);
#else
      start("ledWorker", 64, 3);
#endif
    }

    job* blink1Async(job* parent) {
      return invoke(parent, blink1, this, NULL);
    }

    job* blink2Async(job* parent) {
      return invoke(parent, blink2, this, NULL);
    }
};


////////////////////////////////////////////////////////////////////////
// main task
////////////////////////////////////////////////////////////////////////
class mainWorker : dispatcher {
  private:
    serialWorker ser;
    ledWorker led;

    static void action1(job* p) {
      mainWorker* self = p->self;
      char* message = p->arg;
      p->fork(self->led.blink1Async(p), self->ser.printAsync(p, message));
    }

    static void action2(job* p) {
      mainWorker* self = p->self;
      char* message = p->arg;
      p->fork(self->led.blink2Async(p), self->ser.printAsync(p, message));
    }

  public:
    void init() {
      ser.init();
      led.init();
#ifdef ESP_H
      start("mainWorker", 256, 3, 1);
#else
      start("mainWorker", 64, 3);
#endif
    }
    job* action1Async(job* parent, char* message) {
      return invoke(parent, action1, this, message);
    }

    job* action2Async(job* parent, char* message) {
      return invoke(parent, action2, this, message);
    }
};

//============================================================
//MAIN
//============================================================

static mainWorker worker;

void setup() {
  worker.init();
}

void loop() {
  worker.action1Async(NULL, "hello")->wait();
  worker.action2Async(NULL, "world")->wait();
}

