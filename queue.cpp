#include "queue.h"

queue::queue() {
  writeIndex = readIndex = 0;
  queueMutex = xSemaphoreCreateMutex();
}

job* queue::invoke(void* sender, void (*func)(job*), void* arg, void* result) {
  xSemaphoreTake(queueMutex, portMAX_DELAY);
  job* job = &jobQueue[writeIndex];
  job->invoke(sender, func, arg, result);
  writeIndex = (writeIndex + 1) & QUEUE_MASK;
  xSemaphoreGive(queueMutex);
  return job;
}


static void mainLoop(void* parameter) {
  queue* sender = (queue*)parameter;

  while (true) {
    if (sender->writeIndex != sender->readIndex) {
      //実行
      sender->jobQueue[sender->readIndex].dispatch();
      sender->readIndex = (sender->readIndex + 1) & QUEUE_MASK;
    }
    vTaskDelay(Delay1MS);
  }
}

//タスク開始
void taskStart(queue* que, const char* name, int size, int priority, int coreId) {
  xTaskCreatePinnedToCore(mainLoop, name, size, que, priority, &que->taskHandler, coreId);
}

