#include "Job.h"

struct example01 {
  static void hello(Job* job) {
    Serial.print("hello ");
  }

  static void world(Job* job) {
    Serial.println("world");
  }
};


static Dispatcher ex01Worker;
static example01 ex01;

void setup() {
  Serial.begin(115200);

  //thread start
#ifdef ESP_H
  ex01Worker.Start("example01", 4096, 4, 1);
#else
  ex01Worker.Start("example01", 128, 4);
#endif

}

void loop() {
  Job* job1 = ex01Worker.Invoke(NULL, example01::hello);
  Job* job2 = ex01Worker.Invoke(NULL, example01::world);

  job2->Wait();
}
