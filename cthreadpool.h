#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include <vector>

#include "cthread.h"
#include "ccounter.h"
#include "ccircularqueue.h"
#include "csemaphore.h"


typedef struct ThreadTask
{
    void (*run)(void *args);  //����ָ�룬��Ҫִ�е�����
    void *arg;              //����
}ThreadTask_t;


class CThreadManager   :   public CThread
{
    friend void Func(void *arg);
public:
    CThreadManager();
    CThreadManager(int num);
    ~CThreadManager(void);

    // ��������߳���;
    void SetMaxThreadNum(int num);

    // �������;
    bool AddTask(void (*func)(void*), void *arg);

private:
    void Run();

    bool RunTask();

    // ����߳���;
    CSemaphore m_nMaxThreadNum;
    // �����б�;
    CCircularQueue<ThreadTask_t> m_qTaskQueue;
    std::vector<CThreadFunc*> m_lThreadList;
};

class CThreadPool
{
    friend void ThreadPoolFunc(void *arg);
public:
    CThreadPool();
    CThreadPool(int num);

    // �������;
    bool PushTask(void (*func)(void*), void *arg);
    bool AddTask(void (*func)(void*), void *arg){return PushTask(func, arg);}

    void Start();
    void Stop(){m_bIsRun = false;}
    void Join();

private:
    bool PopTask(ThreadTask_t &task);

    bool m_bIsRun;
//    CSemaphore m_TaskNum;
    // �����б�;
    CCircularQueue<ThreadTask_t> m_qTaskQueue;
    std::vector<CThreadFunc> m_lThreadList;
};

#endif // CTHREADPOOL_H
