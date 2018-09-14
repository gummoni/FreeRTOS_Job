#include "job.h"
#include "dispatcher.h"

#define LED (LED_BUILTIN)
#define delay2(value) vTaskDelay(value / portTICK_PERIOD_MS)

class ledWorker : dispatcher {

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
      return invoke(parent, blink1, NULL, NULL);
    }

    job* blink2Async(job* parent) {
      return invoke(parent, blink2, NULL, NULL);
    }
};

//============================================================
//MAIN
//============================================================

static ledWorker led;

void setup() {
  Serial.begin(9600);
  Serial.println("--setup--");
  led.init();
  Serial.println("--start--");
}

void loop() {
  Serial.println("--go--");
  Serial.println(1000 / portTICK_PERIOD_MS);
  job* pjob1 = led.blink1Async(NULL);
  job* pjob2 = led.blink2Async(NULL);

  Serial.println("--1--");
  pjob1->wait();
  Serial.println("--2--");
  pjob2->wait();  

}
