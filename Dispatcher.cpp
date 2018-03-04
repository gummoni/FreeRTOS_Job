#include "Job.h"
#include "JobManager.h"

static JobManager Jobs = JobManager(8);		//とりあえず固定でジョブ最大8個

#ifdef ESP_H
//コンストラクタ処理
void Dispatcher::Start(const char* name, int size, int priority, int coreId) {
  //キュー作成とタスク起動
  queueHandler = xQueueCreate(8, sizeof(Job*));  //8個 * 4byte(pointer分)
  xTaskCreatePinnedToCore(TaskMain, name, size, this, priority, &taskHandler, coreId);
}
#else//ESP_H
//コンストラクタ処理
void Dispatcher::Start(const char* name, int size, int priority) {
  //キュー作成とタスク起動
  queueHandler = xQueueCreate(8, sizeof(Job*));  //8個 * 4byte(pointer分)
  xTaskCreate(TaskMain, name, size, this, priority, &taskHandler);
}
#endif//ESP_H


//処理をキューの後方に積み込む
Job* Dispatcher::Invoke(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  job->is_scheduled = true;
  xQueueSend(queueHandler, &job, portMAX_DELAY);
  return job;
}


//処理をキューの前方に積み込む
Job* Dispatcher::Interrupt(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  job->is_scheduled = true;
  xQueueSendToFront(queueHandler, &job, portMAX_DELAY);
  return job;
}


//タスクメイン処理
void Dispatcher::TaskMain(void* arg) {
  Job* job;
  Dispatcher* self = (Dispatcher*)arg;
  QueueHandle_t queueHandler = self->queueHandler;

  while (true) {
    //キューが溜まったら実行
    xQueueReceive(queueHandler, &job, portMAX_DELAY);
    job->Dispatch();
  }
}
