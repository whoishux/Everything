#include <stdio.h>

#include "ccounter.h"
#include "ccriticalsection.h"

#define LOCK_TYPE       CCriticalSection

CCounter::CCounter() : m_Count(0)
{
    m_Lock = new(LOCK_TYPE);
}

CCounter::CCounter(int val) : m_Count(val)
{
    m_Lock = new(LOCK_TYPE);
}

CCounter::CCounter(const CCounter &cnt) :
    m_Count(cnt.m_Count)
{
    m_Lock = new(LOCK_TYPE);
}

CCounter::~CCounter()
{
    LOCK_TYPE *pLock = (LOCK_TYPE*)m_Lock;
    delete(pLock);
}

CCounter &CCounter::operator=(const CCounter &cnt)
{
    m_Count = cnt.m_Count;
    return *this;
}

bool CCounter::operator==(const CCounter &cnt)
{
    return this->m_Count == cnt.m_Count;
}

bool CCounter::operator==(int val)
{
    return this->m_Count == val;
}

int CCounter::operator++()
{
    Lock();
    ++m_Count;
    Unlock();
    return m_Count;
}

int CCounter::operator--()
{
    Lock();
    --m_Count;
    Unlock();
    return m_Count;
}

int CCounter::operator++(int)
{
    int cnt;
    cnt = m_Count;
    ++*this;
    return cnt;
}

int CCounter::operator--(int)
{
    int cnt;
    cnt = m_Count;
    --*this;
    return cnt;
}

CCounter &CCounter::operator+=(const CCounter &cnt)
{
    Lock();
    m_Count += cnt.m_Count;
    Unlock();
    return *this;
}

CCounter &CCounter::operator-=(const CCounter &cnt)
{
    Lock();
    m_Count -= cnt.m_Count;
    Unlock();
    return *this;
}

int CCounter::operator+(int v)
{
    return m_Count + v;
}

int CCounter::operator-(int v)
{
    return m_Count - v;
}

int CCounter::Add(int v)
{
    Lock();
    m_Count += v;
    Unlock();
    return m_Count;
}

int CCounter::Deduct(int v)
{
    Lock();
    m_Count -= v;
    Unlock();
    return m_Count;
}

void CCounter::Lock()
{
    ((LOCK_TYPE*)m_Lock)->Lock();
}

void CCounter::Unlock()
{
    ((LOCK_TYPE*)m_Lock)->Unlock();
}
