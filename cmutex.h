/*
 * ������(hux);
 * ���÷�Χ�����Կ����ͬ����������������֤����ֻ��һ��ʵ�����У�����������������,Ҳ�����������̼߳��ͬ��;
 *
*/

#ifndef CMUTEX_H
#define CMUTEX_H


#include "cinclude.h"

class ALGAPI CMutex
{
public:
    CMutex();
    ~CMutex();

    bool Lock(); //��������������
    bool Unlock(); // ����
    bool TryLock(); //��������������������

private:
    void *m_hMutex;
};


#endif // CMUTEX_H
