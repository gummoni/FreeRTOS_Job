#pragma once
#include "job.h"

#define QUEUE_COUNT	(16)
#define QUEUE_MASK	(QUEUE_COUNT - 1)

class queue {
  private:
    xSemaphoreHandle queueMutex;

  public:
    TaskHandle_t taskHandler;
    int writeIndex;
    int readIndex;
    job jobQueue[QUEUE_COUNT];

    static void start(void* parameter);

    queue();
    job* invoke(void* sender, void (*func)(job*), void* arg, void* result);
};

extern void taskStart(queue* que, const char* name, int size, int priority, int coreId);

