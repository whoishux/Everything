/*
 * �������ࣨԭ�Ӳ�����(hux);
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
    // ǰ��;
    int operator++();
    int operator--();
    // ����;
    int operator++(int);
    int operator--(int);
    CCounter &operator+=(const CCounter&);
    CCounter &operator-=(const CCounter&);
    int operator+(int);
    int operator-(int);

    // ��ǰֵ;
    int Value(){return m_Count;}
    // ���Ӽ�����;
    int Add(int v);
    // ��С������;
    int Deduct(int v);


private:
    inline void Lock();
    inline void Unlock();

    int m_Count;
    void* m_Lock;
};

#endif // CCOUNTER_H
