
#include "ccommon.h"
#include "csemaphore.h"

#ifdef __GNUC__
#ifdef M_GNU
#undef M_GNU
#endif
#define M_GNU
#elif _MSC_VER
#ifdef M_MSVC
#undef M_MSVC
#endif
#define M_MSVC
#endif

#ifdef M_GNU
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#else
#include <Windows.h>
#endif


CSemaphore::CSemaphore()
{
    m_sName = "";
    m_nSemSize = 1;
    m_handle = nullptr;
}

CSemaphore::CSemaphore(string name, unsigned int sz)
{
    m_sName = name;
    m_nSemSize = sz;
    m_handle = nullptr;
}


CSemaphore::~CSemaphore()
{
    Close();
}

bool CSemaphore::SetSemaphoreNum(int sz)
{
    if(nullptr != m_handle){ return false;}

    m_nSemSize = sz;
    return true;
}

bool CSemaphore::Create()
{
    if(nullptr != m_handle){ return false;}
#ifdef M_MSVC
    m_handle = CreateSemaphore(nullptr, m_nSemSize, m_nSemSize, nullptr);
    return m_handle != nullptr;
#else
    return sem_init(&m_handle, 0, m_nSemSize) == 0;
#endif
}

bool CSemaphore::Close()
{
    bool ret = false;
    if(nullptr == m_handle){ return false;}
#ifdef M_MSVC
    ret = CloseHandle((HANDLE)m_handle) == TRUE;
    if(ret) m_handle = nullptr;
#else
    int num = 0;
    ret = sem_getvalue(&m_handle, &num) == 0;
    if(ret){
        while(num < m_nSemSize){
            Signal();
            num++;
        }
        ret = sem_destroy(&m_handle) == 0;
        if(ret) m_handle = nullptr;
    }
#endif
    return ret;
}

int CSemaphore::Count()
{
    int num = -1;
    if(m_handle == nullptr) return -1;
#ifdef M_MSVC

#else
    sem_getvalue(&m_handle, &num);
#endif
    return num;
}


bool CSemaphore::Wait(int ms)
{
    bool ret = false;
    if(nullptr == m_handle){ return false;}
#ifdef M_MSVC
    ret = WaitForSingleObject((HANDLE)m_handle, (unsigned int)ms) == WAIT_OBJECT_0;
#else
    timespec ts;
    struct timeval tt;
    gettimeofday(&tt, nullptr);
    ts.tv_sec = tt.tv_sec + ms / 1000;
    ts.tv_nsec = tt.tv_usec * 1000 + (ms % 1000) * 1000 * 1000;
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 *1000);
    ts.tv_nsec %= (1000 * 1000 *1000);
    if(ms == -1)
        ret = sem_wait(&(m_handle)) == 0;
    else
        ret = sem_timedwait(&(m_handle), &ts) == 0;
#endif
    return ret;
}


bool CSemaphore::Signal()
{
    bool ret = false;
#ifdef M_MSVC
    ret = ReleaseSemaphore((HANDLE)m_handle, 1, nullptr) == TRUE;
#else
    ret = sem_post(&m_handle) == 0;
#endif
    return ret;
}


