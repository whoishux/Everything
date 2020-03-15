#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H

#include <string>

#include "cinclude.h"

using namespace std;

class ALGAPI CSemaphore
{
public:
    CSemaphore();
    CSemaphore(string name, unsigned int sz); /*构造函数*/
    ~CSemaphore();                /*析构函数*/

    // 设置信号量计数器;
    bool SetSemaphoreNum(int sz);

    // 创建、关闭;
    bool Create();
    bool Close();

    // 获取大小、计数器;
    int Size(){return m_nSemSize;}
    int Count();

    /**
     * 相当于信号量机制里面的P操作.
     * _count大于0(有资源)时,函数会立即返回,否则会阻塞调用此函数的线程.
     */
    bool Wait(int ms = -1);                    /*P操作*/
    /**
     * 如果有阻塞的线程,则随机唤醒一个线程，相当于信号量机制里面的V操作.否则
     * 立即返回.
     */
    bool Signal();                    /*V操作*/

private:
    unsigned int m_nSemSize;
    string m_sName;
    void *m_handle;
};

#endif // CSEMAPHORE_H
