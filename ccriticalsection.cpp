
#ifdef M_MSVC
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "ccriticalsection.h"

CCriticalSection::CCriticalSection()
{
#ifdef M_MSVC
    LPCRITICAL_SECTION pCs = new CRITICAL_SECTION;
    InitializeCriticalSection(pCs);
    m_gSection = pCs;
#else
    pthread_mutex_init(&m_gSection, NULL);
#endif
}

CCriticalSection::~CCriticalSection()
{
#ifdef M_MSVC
    LPCRITICAL_SECTION pCs = (CRITICAL_SECTION*)m_gSection;
    DeleteCriticalSection(pCs);
    delete pCs;
#else
    pthread_mutex_destroy(&m_gSection);
#endif
}

bool CCriticalSection::TryEnterCriticalSection()
{
#ifdef M_MSVC
    LPCRITICAL_SECTION pCs = (CRITICAL_SECTION*)m_gSection;
    return ::TryEnterCriticalSection(pCs) == TRUE;
#else
    return pthread_mutex_trylock(&m_gSection) == 0;
#endif
}

void CCriticalSection::EnterCriticalSection()
{
#ifdef M_MSVC
    LPCRITICAL_SECTION pCs = (CRITICAL_SECTION*)m_gSection;
    ::EnterCriticalSection(pCs);
#else
    pthread_mutex_lock(&m_gSection);
#endif
}

void CCriticalSection::LeaveCriticalSection()
{
#ifdef M_MSVC
    LPCRITICAL_SECTION pCs = (CRITICAL_SECTION*)m_gSection;
    ::LeaveCriticalSection(pCs);
#else
    pthread_mutex_unlock(&m_gSection);
#endif
}
