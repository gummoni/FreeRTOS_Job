#include "Job.h"
#include "JobFactory.h"
#include "Dispatcher.h"


#ifdef ESP_H
void Dispatcher::Start(const char* name, int size, int priority, int coreId) {
  this->queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(Job*));  //MAX_QUEUE_COUNT個 * 4byte(pointer分)
  xTaskCreatePinnedToCore(TaskMain, name, size, this, priority, &this->taskHandler, coreId);
}
#else//ESP_H
void Dispatcher::Start(const char* name, int size, int priority) {
  this->queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(Job*));  //MAX_QUEUE_COUNT個 * 4byte(pointer分)
  xTaskCreate(TaskMain, name, size, this, priority, &this->taskHandler);
}
#endif//ESP_H

//処理移譲
Job* Dispatcher::Invoke(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  TaskHandle_t currentTaskHandler = xTaskGetCurrentTaskHandle();
  if (currentTaskHandler != this->taskHandler)
  {
	job->is_scheduled = true;
    xQueueSend(this->queueHandler, &job, portMAX_DELAY);
  }

  return job;
}

//処理移譲（優先順位：高め）
Job* Dispatcher::Interrupt(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  TaskHandle_t currentTaskHandler = xTaskGetCurrentTaskHandle();
  if (currentTaskHandler != this->taskHandler)
  {
	job->is_scheduled = true;
    xQueueSendToFront(this->queueHandler, &job, portMAX_DELAY);
  }

  return job;
}

//ディスパッチャタスクメイン
void Dispatcher::TaskMain(void* arg) {
  Job* job;
  Dispatcher* self = (Dispatcher*)arg;
  QueueHandle_t queues = self->queueHandler;

  while (true) {
    xQueueReceive(queues, &job, portMAX_DELAY);
    job->Dispatch();
  }
}

