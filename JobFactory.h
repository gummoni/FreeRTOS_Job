#pragma once

class JobFactory {
  private:
    Job Jobs[MAX_JOB_COUNT];
    int Index;

  public:
    JobFactory();
    void Clear();
    Job* Create(Job* job, void (*execute)(Job* job));
};

extern JobFactory Jobs;

