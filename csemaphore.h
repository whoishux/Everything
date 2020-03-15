#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H

#include <string>

#include "cinclude.h"

using namespace std;

class ALGAPI CSemaphore
{
public:
    CSemaphore();
    CSemaphore(string name, unsigned int sz); /*���캯��*/
    ~CSemaphore();                /*��������*/

    // �����ź���������;
    bool SetSemaphoreNum(int sz);

    // �������ر�;
    bool Create();
    bool Close();

    // ��ȡ��С��������;
    int Size(){return m_nSemSize;}
    int Count();

    /**
     * �൱���ź������������P����.
     * _count����0(����Դ)ʱ,��������������,������������ô˺������߳�.
     */
    bool Wait(int ms = -1);                    /*P����*/
    /**
     * ������������߳�,���������һ���̣߳��൱���ź������������V����.����
     * ��������.
     */
    bool Signal();                    /*V����*/

private:
    unsigned int m_nSemSize;
    string m_sName;
    void *m_handle;
};

#endif // CSEMAPHORE_H
