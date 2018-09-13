#include "Job.h"
#include "JobFactory.h"

JobFactory Jobs;

//コンストラクタ処理
JobFactory::JobFactory() {
  this->Clear();
}

//全ジョブを完了済みにする
void JobFactory::Clear() {
  int idx = 0;
  for (idx = 0; idx < MAX_JOB_COUNT; idx++) {
    Jobs[idx].is_completed = true;
  }
  Index = 0;
}

//Job作成(リングバッファ構造で使いまわしするので、アプリの規模に合わせてバッファサイズを調整する必要がある)
Job* JobFactory::Create(Job* job, void (*execute)(Job* job)) {
  while (!Jobs[Index].is_completed) {
    Index++;
    if (MAX_JOB_COUNT <= Index) Index = 0;
  }
  Job* result = &Jobs[Index];
  result->Init(job, execute);
  return result;
}

