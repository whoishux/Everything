#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include <vector>

#include "cthread.h"
#include "ccounter.h"
#include "ccircularqueue.h"
#include "csemaphore.h"


typedef struct ThreadTask
{
    void (*run)(void *args);  //函数指针，需要执行的任务
    void *arg;              //参数
}ThreadTask_t;


class CThreadManager   :   public CThread
{
    friend void Func(void *arg);
public:
    CThreadManager();
    CThreadManager(int num);
    ~CThreadManager(void);

    // 设置最大线程数;
    void SetMaxThreadNum(int num);

    // 添加任务;
    bool AddTask(void (*func)(void*), void *arg);

private:
    void Run();

    bool RunTask();

    // 最大线程数;
    CSemaphore m_nMaxThreadNum;
    // 任务列表;
    CCircularQueue<ThreadTask_t> m_qTaskQueue;
    std::vector<CThreadFunc*> m_lThreadList;
};

class CThreadPool
{
    friend void ThreadPoolFunc(void *arg);
public:
    CThreadPool();
    CThreadPool(int num);

    // 添加任务;
    bool PushTask(void (*func)(void*), void *arg);
    bool AddTask(void (*func)(void*), void *arg){return PushTask(func, arg);}

    void Start();
    void Stop(){m_bIsRun = false;}
    void Join();

private:
    bool PopTask(ThreadTask_t &task);

    bool m_bIsRun;
//    CSemaphore m_TaskNum;
    // 任务列表;
    CCircularQueue<ThreadTask_t> m_qTaskQueue;
    std::vector<CThreadFunc> m_lThreadList;
};

#endif // CTHREADPOOL_H
