#include "Job.h"


void Job::Init(Job* parent, void (*execute)(Job* job)) {
  this->Execute = execute;
  this->parent = (NULL == parent) ? this : parent->parent;
  is_paused = false;
  is_canceled = false;
  is_completed = false;
  progress = 0;
}

//処理実行
void Job::Dispatch() {
  if (parent->is_canceled) {
    //キャンセル
  } else {
    while (parent->is_paused) {
      taskYIELD();
    }
    Execute(parent);
  }
  is_completed = true;
}

void Job::Wait() {
  while (!is_completed) {
    taskYIELD();
  }
}

void Job::Cancel() {
  parent->is_canceled = true;
}

void Job::Resume() {
  parent->is_paused = false;
}

void Job::Pause() {
  parent->is_paused = true;
}

void Job::Report(int progress) {
  this->progress = progress;
}

void Job::Action(Job* job1) {
  job1->Wait();
}

void Job::Fork(Job* job1, Job* job2) {
  job1->Wait();
  job2->Wait();
}

void Job::Fork(Job* job1, Job* job2, Job* job3) {
  job1->Wait();
  job2->Wait();
  job3->Wait();
}

void Job::Fork(Job* job1, Job* job2, Job* job3, Job* job4) {
  job1->Wait();
  job2->Wait();
  job3->Wait();
  job4->Wait();
}
