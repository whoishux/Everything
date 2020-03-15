#ifndef CCRITICALSECTION_H
#define CCRITICALSECTION_H


#include "cinclude.h"


class ALGAPI CCriticalSection
{
public:
    CCriticalSection();
    ~CCriticalSection();

    bool TryEnterCriticalSection();
    void EnterCriticalSection();
    void LeaveCriticalSection();

    bool TryLock(){return TryEnterCriticalSection();}
    void Lock(){EnterCriticalSection();}
    void Unlock(){LeaveCriticalSection();}

private:
    void *m_gSection;
};

#endif // CCRITICALSECTION_H
