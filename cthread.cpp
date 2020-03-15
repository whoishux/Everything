
#include "cthread.h"

#ifdef ALG
#include "ccommon.h"
#else
#ifdef __GNUC__
#ifdef M_GNU
#undef M_GNU
#endif
#define M_GNU
#elif defined _MSC_VER
#ifdef M_MSVC
#undef M_MSVC
#endif
#define M_MSVC
#endif
#define LogPos
#endif

#ifdef M_GNU
#include <unistd.h>
#include <pthread.h>
#else
#include   <windows.h>
#include   <process.h>
#endif


CThread::CThread(void)
{
    m_bRun = false;
    m_ThreadID = 0;
    m_ThreadPriority = 0;
    m_ThreadName = "";
    m_handle = nullptr;
}

CThread::CThread(std::string ThreadName)
{
    m_ThreadName = (ThreadName);
    m_bRun = (false);
    m_ThreadID = 0;
    m_ThreadPriority = 0;
    m_handle = nullptr;
}

CThread::~CThread(void)
{
#ifndef M_GNU
    if(nullptr != m_handle){
        ::CloseHandle(static_cast<HANDLE>(m_handle));
        m_handle = nullptr;
     }
#else
#endif
}

bool CThread::Start(bool bSuspend)
{
    bool ret;

    if(m_bRun){LogPos(1); return true;}
#ifndef M_GNU
    unsigned int initFlag = 0;
    if(bSuspend || m_ThreadPriority > 0) initFlag |= CREATE_SUSPENDED;
    m_handle = (void*)_beginthreadex(nullptr, 0, StaticThreadFunc, this, initFlag, &m_ThreadID);
    ret = (nullptr != m_handle);
    if(ret && m_ThreadPriority > 0){
        ::SetThreadPriority((HANDLE)m_handle, m_ThreadPriority);
        if(!bSuspend) ::ResumeThread((HANDLE)m_handle);
    }
#else
    ret = pthread_create((pthread_t*)&m_ThreadID, nullptr, StaticThreadFunc, this) == 0;
#endif

    if(ret){
        m_bRun = true;
    }
    return ret;
}

void CThread::Join(unsigned int ms)
{
#ifndef M_GNU
    if(nullptr == m_handle || !m_bRun){return;}

    ::WaitForSingleObject((HANDLE)m_handle, ms);

#else
    if(m_ThreadID == 0){return;}
    if(!m_bRun){return;}

    if(ms == 0){
         pthread_join((pthread_t)m_ThreadID, nullptr);
    }else{
        unsigned long k = 0;
        while(m_bRun && k <= ms){
            usleep(1000);
            k++;
        }
    }

#endif

}

void CThread::Resume()
{
#ifndef M_GNU
    if(nullptr == m_handle || !m_bRun){LogPos(1); return;}

    ::ResumeThread((HANDLE)m_handle);
#else

#endif
}

void CThread::Suspend()
{
#ifndef M_GNU
    if(nullptr == m_handle || !m_bRun){LogPos(1); return;}

    ::SuspendThread((HANDLE)m_handle);
#else

#endif
}

bool CThread::Terminate(unsigned long ExitCode)
{
#ifndef M_GNU
    if(nullptr == m_handle || !m_bRun){LogPos(1); return false;}

    if(::TerminateThread((HANDLE)m_handle, ExitCode)){
        return true;
    }
    return false;
#else
    if(m_ThreadID == 0 || !m_bRun){LogPos(1); return false;}
    pthread_cancel((pthread_t)m_ThreadID);
    return true;
#endif

}

bool CThread::SetThreadPriority(int nPriority)
{
#ifndef M_GNU
    switch (nPriority) {
    case 1:
        m_ThreadPriority = THREAD_PRIORITY_IDLE;
        break;
    case 2:
        m_ThreadPriority = THREAD_PRIORITY_LOWEST;
        break;
    case 3:
        m_ThreadPriority = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case 4:
        m_ThreadPriority = THREAD_PRIORITY_NORMAL;
        break;
    case 5:
        m_ThreadPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case 6:
        m_ThreadPriority = THREAD_PRIORITY_HIGHEST;
        break;
    case 7:
        m_ThreadPriority = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    default:
        m_ThreadPriority = 0;
        break;
    }
    return m_ThreadPriority > 0;
#else
    return false;
#endif
}

unsigned int CThread::GetThreadID()
{
    return m_ThreadID;
}

std::string CThread::GetThreadName()
{
    return m_ThreadName;
}

void CThread::SetThreadName(std::string ThreadName)
{
    m_ThreadName = ThreadName;
}

void CThread::run1()
{
    m_IsRun = true;
    Run();
#ifndef M_GNU
    _endthreadex( 0 );
#endif
    m_bRun = false;
    m_IsRun = false;
}

threadfunc_t CThread::StaticThreadFunc(threadparam_t arg)
{
    CThread * pThread = (CThread *)arg;

    pThread->run1();

    return 0;
}

