/*
 * 线程类(hux);
*/

#ifndef CTHREAD_H
#define CTHREAD_H

#include <iostream>
#include <string>

#include "cinclude.h"


#ifdef _MSC_VER
typedef unsigned threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX __stdcall
#else
typedef void * threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX
#endif

class ALGAPI CThread
{
private:
//    explicit CThread(const CThread & rhs);

public:
    CThread();
    CThread(std::string ThreadName);
    virtual ~CThread(void);


    /**
      开始运行线程
      @arg bSuspend 开始运行时是否挂起
    **/
    bool Start(bool bSuspend = false);

    /**
      发送终止线程指令，Run函数需实现退出操作;
    **/
    void Stop(){m_IsRun = false;}

    /**
      运行的线程函数，可以使用派生类重写此函数
    **/
    virtual void Run() = 0;

    /**
      当前执行此函数线程等待线程结束
      @arg timeout 等待超时时间，如果为负数，等待无限时长
    **/
    void Join(unsigned int ms = -1);
    /**
      恢复挂起的线程
    **/
    void Resume();
    /**
      挂起线程
    **/
    void Suspend();
    /**
      终止线程的执行
    **/
    bool Terminate(unsigned long ExitCode = 0);
    /**
     * 设置线程优先级;
     *  nPriority:
     *      1   IDLE 最低
     *      2   LOWEST 低
     *      3   BELOW 低于标准
     *      4   NORMAL 标准
     *      5   ABOVE 高于标准
     *      6   HIGHEST 高
     *      7   CRITICAL 最高
     */
    bool SetThreadPriority(int nPriority);

    /**
      返回线程ID号;
    **/
    unsigned int GetThreadID();
    std::string GetThreadName();
    void SetThreadName(std::string ThreadName);
    bool IsRun(){return m_bRun;}
    bool IsStop(){return !m_bRun;}

protected:
    bool m_IsRun;

private:
    void run1();
    static threadfunc_t STDPREFIX StaticThreadFunc(threadparam_t arg);

private:
    std::string m_ThreadName;
    void *m_handle;
    unsigned int m_ThreadID;
    int m_ThreadPriority;
    volatile bool m_bRun;
};

class ThreadDemo : public CThread
{
public:
    void Run()
    {
        int i=0;
        while(i++ < 10){
            printf("Id:%d name=%s ThreadDemo Run!\n", GetThreadID(), GetThreadName().c_str());
        }
    }
};

class CThreadFunc : public CThread
{
public:
    CThreadFunc(){m_funcv = (nullptr); m_func = (nullptr); m_p = (nullptr);}
    ~CThreadFunc(){}

    void SetFunc(void (*func)(void)){if(!IsRun())m_funcv = func;}
    void SetFunc(void (*func)(void*), void *p=nullptr)
    {
        if(!IsRun()){
            m_func = func;
            m_p = p;
        }
    }
    void ClearFunc()
    {
        if(!IsRun()){
            m_func = nullptr;
            m_funcv = nullptr;
            m_p = nullptr;
        }
    }

    void Run()
    {
//        printf("CThreadFunc> Run tid=%d\n", GetThreadID());
        if(m_funcv) m_funcv();
        if(m_func) m_func(m_p);
    }

private:
    void (*m_funcv)();
    void (*m_func)(void*);
    void *m_p;
};

#define CThreadP(p)            ((CThread *)(p))
#define CThreadFuncP(p)        ((CThreadFunc *)(p))

#endif // CTHREAD_H
