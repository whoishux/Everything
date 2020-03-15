#include "cthreadpool.h"
#include "ccommon.h"
#include "cmutex.h"

#define TaskMaxNum       1000

typedef struct
{
    ThreadTask_t task;
    CThreadManager *tm;
}TmpTask_t;

void Func(void *arg)
{
    bool ret = false;
//    TmpTask_t *tmpTask = (TmpTask_t*)arg;
    CThreadManager *tm = (CThreadManager*)arg;

    if(tm){
        ThreadTask_t task;
        ret = tm->m_qTaskQueue.Pop(task);
        if(!ret){LogPos(1); return;}

        if(task.run && task.arg){
//            LogFile("Func> tid=%d\n", GetTID());
            task.run(task.arg);
            tm->m_nMaxThreadNum.Signal();
        }else{
            LogPos(1);
        }
    }else{
        LogPos(1);
    }
}

CThreadManager::CThreadManager()
{
    m_qTaskQueue.Resize(TaskMaxNum);
}

CThreadManager::CThreadManager(int num)
{
    SetMaxThreadNum(num);
    m_qTaskQueue.Resize(TaskMaxNum);
}

CThreadManager::~CThreadManager(void)
{
    for(int i=0; i<m_lThreadList.size(); i++){
        if(!m_lThreadList[i]) continue;
        m_lThreadList[i]->Join();
        MDelete(m_lThreadList[i]);
    }
}

void CThreadManager::SetMaxThreadNum(int num)
{
    m_nMaxThreadNum.SetSemaphoreNum(num);
    m_nMaxThreadNum.Create();
    m_lThreadList.resize(num);
}

bool CThreadManager::AddTask(void (*func)(void *), void *arg)
{
    bool ret = false;
    ThreadTask_t task;

    task.run = func;
    task.arg = arg;

    ret = m_qTaskQueue.Push(task);

    return ret;
}

void CThreadManager::Run()
{
    bool ret = false;
    CCounter cnt = 0;

    while (!m_qTaskQueue.IsEmpty()) {
        ThreadTask_t task;
        m_nMaxThreadNum.Wait();
//        LogFile("------------------\n");
//        LogFile("cnt=%d, qsz=%d\n", cnt++, m_qTaskQueue.Count());

//        ret = m_qTaskQueue.Pop(task);
//        if(!ret){LogPos(1); break;}

        ret = RunTask();
        if(!ret){
//            LogPos(1);
            break;
        }
    }

    for(int i=0; i<m_lThreadList.size(); i++){
        m_lThreadList[i]->Join();
    }
}

bool CThreadManager::RunTask()
{
    bool ret = false;


//LogFile("m_lThreadList.sz=%d\n", m_lThreadList.size());
    for(int i=0; i<m_lThreadList.size(); i++){
        if(m_lThreadList[i] && !m_lThreadList[i]->IsRun()) MDelete(m_lThreadList[i]);
        if(m_lThreadList[i] == NULL){
            TmpTask_t tmp;

            m_lThreadList[i] = MNew(CThreadFunc);
            if(!m_lThreadList[i]){LogPos(1); break;}

//            tmp.task = task;
//            tmp.tm = this;

            m_lThreadList[i]->SetFunc(Func, this);
            m_lThreadList[i]->Start();
            ret = true;
            break;
        }
    }

    return ret;
}

void ThreadPoolFunc(void *arg)
{
    bool ret = false;
//    TmpTask_t *tmpTask = (TmpTask_t*)arg;
    CThreadPool *tp = (CThreadPool*)arg;

    if(tp){
        while(tp->m_bIsRun){
            ThreadTask_t task;
            ret = tp->PopTask(task);
            if(!ret){
//                LogPos(1);
                MsSleep(10);
                continue;
            }

            if(task.run && task.arg){
    //            LogFile("Func> tid=%d\n", GetTID());
                task.run(task.arg);
            }else{
                LogPos(1);
            }
        }
    }else{
        LogPos(1);
    }
}

CThreadPool::CThreadPool()
{
    m_bIsRun = false;

//    m_TaskNum.SetSemaphoreNum(TaskMaxNum);
    m_qTaskQueue.Resize(TaskMaxNum);
    m_lThreadList.resize(4);
}

CThreadPool::CThreadPool(int num)
{
    m_bIsRun = false;

//    m_TaskNum.SetSemaphoreNum(TaskMaxNum);
    m_qTaskQueue.Resize(TaskMaxNum);
    m_lThreadList.resize(num);
}

bool CThreadPool::PushTask(void (*func)(void *), void *arg)
{
    bool ret = false;
    ThreadTask_t task;
    CMutex mutex;

//    mutex.Lock();

    task.run = func;
    task.arg = arg;

//    m_TaskNum.Signal();
    ret = m_qTaskQueue.Push(task);

//    mutex.Unlock();

    return ret;
}

void CThreadPool::Start()
{
    m_bIsRun = true;

    for(int i=0; i<m_lThreadList.size(); i++){
        m_lThreadList[i].SetFunc(ThreadPoolFunc, this);
        m_lThreadList[i].Start();
    }

}

void CThreadPool::Join()
{
    while(!m_qTaskQueue.IsEmpty()) MsSleep(100);

    m_bIsRun = false;
    for(int i=0; i<m_lThreadList.size(); i++)
        m_lThreadList[i].Join();
}

bool CThreadPool::PopTask(ThreadTask_t &task)
{
    bool ret = false;
    CMutex mutex;

//    mutex.Lock();
    if(!m_bIsRun) goto done;

//    m_TaskNum.Wait();
//    if(!m_bIsRun) goto done;

    ret = m_qTaskQueue.Pop(task);

done:
//    mutex.Unlock();
    return ret;
}
