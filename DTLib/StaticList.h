#ifndef STATICLIST_H
#define STATICLIST_H

#include "SeqList.h"

namespace MyLib
{

template <typename T, int N>
class StaticList : public SeqList<T>
{
protected:
    T m_space[N];       // 定义顺序存储空间
public:
    StaticList()        // 指定父类成员的具体值
    {
        this->m_array = m_space;
        this->m_length = 0;
    }

    int capacity() const
    {
        return N;
    }
};


}

#endif // STATICLIST_H
