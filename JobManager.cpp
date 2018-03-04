#include "Job.h"
#include "JobManager.h"



JobManager::JobManager(int capacity) {
  this->capacity = capacity;
  Jobs = new Job[capacity];
  Init();
}

void JobManager::Init() {
  int idx = 0;
  for (idx = 0; idx < capacity; idx++) {
    Jobs[idx].is_completed = true;
  }
  jobIdx = 0;
}

Job* JobManager::Create(Job* job, void (*execute)(Job* job)) {
  //‹ó‚«Job‚ð’T‚·
  while (!Jobs[jobIdx].is_completed) {
    jobIdx++;
    if (capacity <= jobIdx) jobIdx = 0;
  }
  Job* result = &Jobs[jobIdx];
  result->Init(job, execute);
  return result;
}
