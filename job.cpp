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


void job::init(job* _parent, void (*func)(job*), void* obj, void* _arg) {
  publish = NULL;
  execute = func;
  parent = (NULL == _parent) ? this : _parent;
  is_paused = false;
  is_canceled = false;
  is_completed = false;
  is_scheduled = false;
  progress = NULL;
  self = obj;
  arg = _arg;
}

void job::dispatch() {
  while (parent->is_paused) {
    taskYIELD();
  }
  if (!parent->is_canceled) {
    execute(parent);
  }
  is_completed = true;
}

void job::wait() {
  while (!is_completed) {
    taskYIELD();
  }
}

void job::cancel() {
  is_canceled = true;
  if (parent != this) {
    parent->cancel();
  }
}

void job::resume() {
  is_paused = false;
  if (parent != this) {
    parent->resume();
  }
}

void job::pause() {
  is_paused = true;
  if (parent != this) {
    parent->pause();
  }
}

void job::report(int _progress) {
  if (progress != _progress) {
    progress = _progress;

    if (NULL != publish) {
      // publish
      publish(this);
    }
  }
}

void job::subscribe(void (*subscriber)(job*)) {
  publish = subscriber;

  if (NULL != progress) {
    //publish
    publish(this);
  }
}

void job::unsubscribe() {
  publish = NULL;
}

void job::action(job* job1) {
  job1->wait();
}

void job::fork(job* job1, job* job2) {
  job1->wait();
  job2->wait();
}

void job::fork(job* job1, job* job2, job* job3) {
  job1->wait();
  job2->wait();
  job3->wait();
}

void job::fork(job* job1, job* job2, job* job3, job* job4) {
  job1->wait();
  job2->wait();
  job3->wait();
  job4->wait();
}

