/*
 * 互斥类(hux);
 * 适用范围：可以跨进程同步，还可以用来保证程序只有一个实例运行（创建命名互斥量）,也可以用来做线程间的同步;
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

    bool Lock(); //锁定，否则阻塞
    bool Unlock(); // 解锁
    bool TryLock(); //尝试锁定，但不会阻塞

private:
    void *m_hMutex;
};


#endif // CMUTEX_H
