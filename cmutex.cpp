#include "cmutex.h"

#include <stdlib.h>
#include <memory.h>

#ifdef __GNUC__
#define M_GNU
#elif _MSC_VER
#define M_MSVC
#endif

#ifdef M_GNU
#include <unistd.h>
#include <pthread.h>
#else
#include   <windows.h>
#include   <process.h>
#endif

CMutex::CMutex()
{
#ifdef M_MSVC
    m_hMutex = CreateMutex(NULL,               /*默认安全属性*/
                           false,               /*创建线程不拥有该信号量*/
                           NULL                /*锁名称*/
                            );
#else
    pthread_mutex_init(&m_hMutex, NULL);
#endif

}

CMutex::~CMutex()
{
#ifdef M_MSVC
    if(m_hMutex) CloseHandle(m_hMutex);
#else
    pthread_mutex_destroy(&m_hMutex);
#endif

}

bool CMutex::Lock()
{
#ifdef M_MSVC
    bool ret = false;
    if(m_hMutex){
        DWORD dRes = -1;
        dRes = WaitForSingleObject(m_hMutex, INFINITE);
        ret = (WAIT_OBJECT_0  == dRes);
    }

    return ret;
#else
    return pthread_mutex_lock(&m_hMutex) == 0;
#endif
}

bool CMutex::Unlock()
{
#ifdef M_MSVC
    if(m_hMutex){
        ReleaseMutex(m_hMutex);
        return true;
    }

    return false;
#else
    return pthread_mutex_unlock(&m_hMutex) == 0;
#endif
}

bool CMutex::TryLock()
{
#ifdef M_MSVC
    return false;
#else
    return pthread_mutex_trylock(&m_hMutex) == 0;
#endif
}
