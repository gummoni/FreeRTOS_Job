#include "Job.h"

static JobManager Jobs = JobManager(MAX_JOB_COUNT);		//�Ƃ肠�����Œ�ŃW���u�ő�8��

#ifdef ESP_H
//�R���X�g���N�^����
void Dispatcher::Start(const char* name, int size, int priority, int coreId) {
  //�L���[�쐬�ƃ^�X�N�N��
  queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(Job*));  //MAX_QUEUE_COUNT�� * 4byte(pointer��)
  xTaskCreatePinnedToCore(TaskMain, name, size, this, priority, &taskHandler, coreId);
}
#else//ESP_H
//�R���X�g���N�^����
void Dispatcher::Start(const char* name, int size, int priority) {
  //�L���[�쐬�ƃ^�X�N�N��
  queueHandler = xQueueCreate(MAX_QUEUE_COUNT, sizeof(Job*));  //MAX_QUEUE_COUNT�� * 4byte(pointer��)
  xTaskCreate(TaskMain, name, size, this, priority, &taskHandler);
}
#endif//ESP_H


//�������L���[�̌���ɐςݍ���
Job* Dispatcher::Invoke(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  xQueueSend(queueHandler, &job, portMAX_DELAY);
  return job;
}


//�������L���[�̑O���ɐςݍ���
Job* Dispatcher::Interrupt(Job* parent, void (*execute)(Job* job)) {
  Job* job = Jobs.Create(parent, execute);
  xQueueSendToFront(queueHandler, &job, portMAX_DELAY);
  return job;
}


//�^�X�N���C������
void Dispatcher::TaskMain(void* arg) {
  Job* job;
  Dispatcher* self = (Dispatcher*)arg;
  QueueHandle_t queueHandler = self->queueHandler;

  while (true) {
    //�L���[�����܂�������s
    xQueueReceive(queueHandler, &job, portMAX_DELAY);
    job->Dispatch();
  }
}
