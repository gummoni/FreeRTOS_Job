#pragma once
#include "rtos.h"


struct Job;

struct Dispatcher {
#ifdef ESP_H
    void Start(const char* name, int size, int priority, int coreId);
#else//ESP_H
    void Start(const char* name, int size, int priority);
#endif//ESP_H
    Job* Invoke(Job* parent, void (*execute)(Job* job));
    Job* Interrupt(Job* parent, void (*execute)(Job* job));

  private:
    QueueHandle_t queueHandler;
    TaskHandle_t taskHandler;
    static void TaskMain(void* arg);
};
