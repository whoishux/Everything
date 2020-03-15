/*
 * 计数器类（原子操作）(hux);
*/

#ifndef CCOUNTER_H
#define CCOUNTER_H


#include "cinclude.h"

class ALGAPI CCounter
{
public:
    CCounter();
    CCounter(int);
    CCounter(const CCounter &);
    ~CCounter();

    CCounter &operator=(const CCounter&);
    bool operator==(const CCounter&);
    bool operator==(int val);
    // 前置;
    int operator++();
    int operator--();
    // 后置;
    int operator++(int);
    int operator--(int);
    CCounter &operator+=(const CCounter&);
    CCounter &operator-=(const CCounter&);
    int operator+(int);
    int operator-(int);

    // 当前值;
    int Value(){return m_Count;}
    // 增加计数器;
    int Add(int v);
    // 减小计数器;
    int Deduct(int v);


private:
    inline void Lock();
    inline void Unlock();

    int m_Count;
    void* m_Lock;
};

#endif // CCOUNTER_H
