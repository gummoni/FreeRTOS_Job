#pragma once

struct JobManager {
  Job* Jobs;
  int jobIdx;
  int capacity;

  JobManager(int capacity);
  void Initialize();
  Job* Create(Job* job, void (*execute)(Job* job));
};
