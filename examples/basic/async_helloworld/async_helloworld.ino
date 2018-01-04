#include "queue.h"

class example01 {
   private:
    queue que;

    static void func(job* parameter) {
      Serial.println("hello world");
    }

  public:
    void init() {
      Serial.begin(19200);
      taskStart(&que, "example01", 4096, 3, 1);
    }

    job* funcAsync() {
      return que.invoke(this, func, NULL, NULL);
    }
};


static example01 ex;

void setup() {
  ex.init();
}

void loop() {
  ex.funcAsync()->wait();
}
