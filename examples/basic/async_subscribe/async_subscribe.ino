#include "job.h"
#include "dispatcher.h"

#define LED (LED_BUILTIN)
#define delay2(value) vTaskDelay(value / portTICK_PERIOD_MS)

////////////////////////////////////////////////////////////////////////
// led task
////////////////////////////////////////////////////////////////////////
class processingWorker : dispatcher {

    static void action1(job* p) {
      int i;
      for (i = 0; i < 10; i++) {
        p->report(i);
        digitalWrite(LED, HIGH);
        delay2(500);
        digitalWrite(LED, LOW);
        delay2(500);
      }
    };

  public:
    void init() {
      pinMode(LED, OUTPUT);
#ifdef ESP_H
      start("processingWorker", 256, 3, 1);
#else
      start("processingWorker", 64, 3);
#endif
    }

    job* action1Async(job* parent) {
      return invoke(parent, action1, this, NULL);
    }
};

//============================================================
//MAIN
//============================================================

static processingWorker proc;

void setup() {
  Serial.begin(9600);
  proc.init();
}

static void callbackFunc(job* p) {
  Serial.println((int)p->progress);
}

void loop() {
  job* pjob = proc.action1Async(NULL);

  Serial.println("--start--");
  pjob->subscribe(callbackFunc);
  pjob->wait();
  Serial.println("--finished--");
}