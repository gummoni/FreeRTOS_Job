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
#pragma once
#include "config.h"

#define DelayMS(milliSeconds) (milliSeconds / portTICK_PERIOD_MS)


class job {
  public:
    void (*execute)(job*);
    job* parent;
    bool is_paused;
    bool is_canceled;
    bool is_completed;
    bool is_scheduled;
    int progress;
	void* arguments;
	void* result;

    void init(job* _parent, void (*func)(job*), void* arg, void* ret);
    void dispatch();
    void wait();
    void cancel();
    void resume();
    void pause();
    void report(int progress);
    void action(job* job1);
    void fork(job* job1, job* job2);
    void fork(job* job1, job* job2, job* job3);
    void fork(job* job1, job* job2, job* job3, job* job4);
};

