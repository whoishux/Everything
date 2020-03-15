/*
 * �߳���(hux);
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
      ��ʼ�����߳�
      @arg bSuspend ��ʼ����ʱ�Ƿ����
    **/
    bool Start(bool bSuspend = false);

    /**
      ������ֹ�߳�ָ�Run������ʵ���˳�����;
    **/
    void Stop(){m_IsRun = false;}

    /**
      ���е��̺߳���������ʹ����������д�˺���
    **/
    virtual void Run() = 0;

    /**
      ��ǰִ�д˺����̵߳ȴ��߳̽���
      @arg timeout �ȴ���ʱʱ�䣬���Ϊ�������ȴ�����ʱ��
    **/
    void Join(unsigned int ms = -1);
    /**
      �ָ�������߳�
    **/
    void Resume();
    /**
      �����߳�
    **/
    void Suspend();
    /**
      ��ֹ�̵߳�ִ��
    **/
    bool Terminate(unsigned long ExitCode = 0);
    /**
     * �����߳����ȼ�;
     *  nPriority:
     *      1   IDLE ���
     *      2   LOWEST ��
     *      3   BELOW ���ڱ�׼
     *      4   NORMAL ��׼
     *      5   ABOVE ���ڱ�׼
     *      6   HIGHEST ��
     *      7   CRITICAL ���
     */
    bool SetThreadPriority(int nPriority);

    /**
      �����߳�ID��;
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
