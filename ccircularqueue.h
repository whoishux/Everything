/*
 * 循环队列类(hux)，模板类;
*/

#ifndef CCIRCULARQUEUE_H
#define CCIRCULARQUEUE_H

#include <vector>

#ifdef ALGDLL
#include "cinclude.h"
#else
#define ALGAPI
#endif

template <typename T> class  ALGAPI  CCircularQueue
{
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    CCircularQueue()
    {
        m_QueSIdx = 0;
        m_QueEIdx = 0;
    }
    CCircularQueue(int num)
    {
        m_QueData.resize(num+1);
        m_QueSIdx = 0;
        m_QueEIdx = 0;
    }

    // 设置Queue大小;
    void Resize(int n){if(n > 0) m_QueData.resize(n+1);}
    // 获取Queue空间大小;
    size_type Size() const {return (m_QueData.size() > 0 ?  m_QueData.size() - 1 : 0);}
    // 获取Queue当前数据量;
    size_type Count() const {return m_QueSIdx >= m_QueEIdx ? (m_QueSIdx - m_QueEIdx) : (Size() - (m_QueEIdx - m_QueSIdx) + 1);}
    // 置空;
    void Empty() {m_QueEIdx = m_QueSIdx;}
    // 判断是否空;
    bool IsEmpty() const {return m_QueSIdx == m_QueEIdx;}
    // 判断是否满;
    bool IsFull() const {return Count() == Size();}

    bool Push(T &v)
    {
        if(!IsFull()){
            m_QueData[m_QueSIdx++] = v;
            m_QueSIdx %= m_QueData.size();
            return true;
        }
        return false;
    }
    bool Pop(T &v)
    {
        if(!IsEmpty()){
            v = m_QueData[m_QueEIdx++];
            m_QueEIdx %= m_QueData.size();
            return true;
        }
        return false;
    }
    bool Pop() {T v; return Pop(v);}

private:
    // Queue数据列表;
    std::vector<T> m_QueData;
    // Queue数据头索引;
    size_type m_QueSIdx;
    // Queue数据尾索引;
    size_type m_QueEIdx;
};


#endif // CCIRCULARQUEUE_H
