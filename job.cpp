#include "Job.h"


void Job::Init(Job* parent, void (*execute)(Job* job)) {
  this->Execute = execute;
  this->parent = (NULL == parent) ? this : parent->parent;
  this->is_paused = false;
  this->is_canceled = false;
  this->is_completed = false;
  this->is_scheduled = false;
  this->progress = 0;
}

void Job::Dispatch() {
  while (this->parent->is_paused) {
    taskYIELD();
  }
  if (this->parent->is_canceled) {
    //キャンセル
  } else {
    this->Execute(parent);
  }
  this->is_completed = true;
}

void Job::Wait() {
  while (!this->is_completed) {
    taskYIELD();
  }
}

void Job::Cancel() {
  this->is_canceled = true;
  if (this->parent != this) {
    this->parent->Cancel();
  }
}

void Job::Resume() {
  this->is_paused = false;
  if (this->parent != this) {
    this->parent->Resume();
  }
}

void Job::Pause() {
  this->is_paused = true;
  if (this->parent != this) {
    this->parent->Pause();
  }
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

