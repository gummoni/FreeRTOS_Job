#pragma once
#include "rtos.h"

struct JobManager {
  Job* Jobs;
  int jobIdx;
  int capacity;

  JobManager(int capacity);
  void Init();
  Job* Create(Job* job, void (*execute)(Job* job));
};
