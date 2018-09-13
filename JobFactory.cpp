#include "Job.h"
#include "JobFactory.h"

JobFactory Jobs;

//�R���X�g���N�^����
JobFactory::JobFactory() {
  this->Clear();
}

//�S�W���u�������ς݂ɂ���
void JobFactory::Clear() {
  int idx = 0;
  for (idx = 0; idx < MAX_JOB_COUNT; idx++) {
    Jobs[idx].is_completed = true;
  }
  Index = 0;
}

//Job�쐬(�����O�o�b�t�@�\���Ŏg���܂킵����̂ŁA�A�v���̋K�͂ɍ��킹�ăo�b�t�@�T�C�Y�𒲐�����K�v������)
Job* JobFactory::Create(Job* job, void (*execute)(Job* job)) {
  while (!Jobs[Index].is_completed) {
    Index++;
    if (MAX_JOB_COUNT <= Index) Index = 0;
  }
  Job* result = &Jobs[Index];
  result->Init(job, execute);
  return result;
}

