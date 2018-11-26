#ifndef SRC_THREADPOOL_H
#define SRC_THREADPOOL_H

#include <vector>
#include "thread.h"
#include "job.h"
#include "worker_thread.h"
#include "mutex.h"
#include "condition.h"

class CThreadPool
{
  friend class CWorkerThread;

public:
  CThreadPool();
  CThreadPool(int num);
  virtual ~CThreadPool();
  void TerminateAll();
  void Run(CJob *job, void *jobdata);

protected:
  void AppendToIdleList(CWorkerThread *jobthread);
  void MoveToBusyList(CWorkerThread *idlethread);
  void MoveToIdleList(CWorkerThread *busythread);

private:
  std::vector<CWorkerThread *> thread_list;
  std::vector<CWorkerThread *> idle_list;
  std::vector<CWorkerThread *> busy_list;

  CWorkerThread *GetIdleThread();
  unsigned int init_num;
  unsigned int idle_num;
  CMutex mutex;
  CCondition cond; //blocks the current thread when no idle thread
};

#endif
