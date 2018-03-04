#include "Job.h"


void Job::Init(Job* parent, void (*execute)(Job* job)) {
  this->Execute = execute;
  this->parent = (NULL == parent) ? this : parent->parent;
  is_paused = false;
  is_canceled = false;
  is_completed = false;
  progress = 0;
  is_completed = false;
}

//処理実行
void Job::Dispatch() {
  if (parent->is_canceled) {
    //キャンセル
  } else {
    while (parent->is_paused) {
      taskYIELD();
    }
#ifdef __ENABLE_EXCEPTION__
    try {
      Execute(parent);
    } catch (int errorCode) {
      progress = errorCode;
    }
#else//__ENABLE_EXCEPTION__
    Execute(parent);
#endif//__ENABLE_EXCEPTION__
  }
  is_completed = true;
}

//完了待ち
void Job::Wait() {
  if (is_scheduled) {
    while (!is_completed) {
      taskYIELD();
    }
  } else {
    Dispatch();
  }
}

//キャンセル
void Job::Cancel() {
  parent->is_canceled = true;
}

//再開
void Job::Resume() {
  parent->is_paused = false;
}

//一時低地
void Job::Pause() {
  parent->is_paused = true;
}

//進捗報告
void Job::Report(int progress) {
  this->progress = progress;
}

//同期処理
void Job::Action(Job* job1) {
  job1->Wait();
}

//並列処理
void Job::Fork(Job* job1, Job* job2) {
  job1->Wait();
  job2->Wait();
}

//並列処理
void Job::Fork(Job* job1, Job* job2, Job* job3) {
  job1->Wait();
  job2->Wait();
  job3->Wait();
}

//並列処理
void Job::Fork(Job* job1, Job* job2, Job* job3, Job* job4) {
  job1->Wait();
  job2->Wait();
  job3->Wait();
  job4->Wait();
}

