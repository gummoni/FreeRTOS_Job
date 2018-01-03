#include "job.h"
// file::esp32\platform.txt
// -fno-exceptions を -fexceptionsにする

void job::invoke(void* sender, void (*func)(job*), void* arg, void* result) {
  this->sender = sender;
  this->func = func;
  this->arg = arg;
  this->result = result;
  isPause = isCompleted = isCancel = false;
  state = errorCode = 0;
}

void job::cancel() {
  isCancel = true;
}

void job::pause() {
  isPause = true;
}

void job::resume() {
  isPause = false;
}

void job::report(int state) {
  this->state = state;
}

void job::wait() {
  while (!isCompleted) {
    vTaskDelay(Delay1MS);
  }
  if (0 != errorCode) {
    throw errorCode;
  }
}

void job::dispatch() {
  try {
    func(this);
  } catch (int errorCode) {
    this->errorCode = errorCode;
  }
  isCompleted = true;
}

void job::action(job* p) {
  while (!p->isCompleted) {
    p->isCancel = isCancel;
    p->isPause = isPause;
    vTaskDelay(Delay1MS);
  }
  if (0 != errorCode) {
    throw errorCode;
  }
}

void job::fork(job** jobs, int length) {
  bool fin = false;
  int errorCode = 0;

  while (!fin) {
    fin = true;
    for (int i = 0; i < length; i++) {
      job* p = jobs[i];
      p->isCancel = isCancel;
      p->isPause = isPause;
      fin &= p->isCompleted;
      if (errorCode == 0) {
        errorCode != p->errorCode;
      }
    }

    if (0 != errorCode) {
      throw errorCode;
    }

    vTaskDelay(Delay1MS);
  }
}

