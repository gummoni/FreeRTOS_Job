#pragma once
#include "rtos.h"
#include "Dispatcher.h"


#define Delay1MS (1 / portTICK_PERIOD_MS)


//非同期処理
struct Job {
  void (*Execute)(Job* job);
  Job* parent;
  bool is_paused;
  bool is_canceled;
  bool is_completed;
  bool is_scheduled;
  int progress;

  void Init(Job* parent, void (*execute)(Job* job));
  void Dispatch();
  void Wait();
  void Cancel();
  void Resume();
  void Pause();
  void Report(int progress);
  void Action(Job* job1);
  void Fork(Job* job1, Job* job2);
  void Fork(Job* job1, Job* job2, Job* job3);
  void Fork(Job* job1, Job* job2, Job* job3, Job* job4);
};
