/*
The MIT License (MIT)

Copyright (c) 2018 Koichi Nishino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "job.h"
#include "jobFactory.h"
#include "dispatcher.h"


#ifdef ESP_H
void dispatcher::start(const char* name, int size, int priority, int coreId) {
  this->queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(job*));  //MAX_QUEUE_COUNT個 * 4byte(pointer分)
  xTaskCreatePinnedToCore(TaskMain, name, size, this, priority, &this->taskHandler, coreId);
}
#else//ESP_H
void dispatcher::start(const char* name, int size, int priority) {
  this->queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(job*));  //MAX_QUEUE_COUNT個 * 4byte(pointer分)
  xTaskCreate(TaskMain, name, size, this, priority, &this->taskHandler);
}
#endif//ESP_H

//処理移譲
job* dispatcher::invoke(job* parent, void (*func)(job*), void* arg, void* ret) {
  job* pjob = jobs.create(parent, func, arg, ret);
  TaskHandle_t currentTaskHandler = xTaskGetCurrentTaskHandle();
  if (currentTaskHandler != this->taskHandler)
  {
	pjob->is_scheduled = true;
    xQueueSend(this->queueHandler, &pjob, portMAX_DELAY);
  }

  return pjob;
}

//処理移譲（優先順位：高め）
job* dispatcher::interrupt(job* parent, void (*func)(job*), void* arg, void* ret) {
  job* pjob = jobs.create(parent, func, arg, ret);
  TaskHandle_t currentTaskHandler = xTaskGetCurrentTaskHandle();
  if (currentTaskHandler != this->taskHandler)
  {
	pjob->is_scheduled = true;
    xQueueSendToFront(this->queueHandler, &pjob, portMAX_DELAY);
  }

  return pjob;
}

//ディスパッチャタスクメイン
void dispatcher::TaskMain(void* arg) {
  job* pjob;
  dispatcher* self = (dispatcher*)arg;
  QueueHandle_t queues = self->queueHandler;

  while (true) {
    xQueueReceive(queues, &pjob, portMAX_DELAY);
    pjob->dispatch();
  }
}

