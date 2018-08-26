#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "Exception.h"

namespace MyLib
{

template <typename T, int N>
class StaticStack : public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;
public:
    StaticStack()
    {
        m_top = -1;
        m_size = 0;
    }

    int capacity() const
    {
        return N;
    }

    void push(const T& e)
    {
        if(m_size < N)
        {
            m_space[m_top + 1] = e;
            m_top++;
            m_size++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "no space in current stack...");
        }
    }

    void pop()
    {
        if(m_size > 0)
        {
            m_top--;
            m_size--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "no element in curretn stack");        }
    }

    T top() const
    {
        if(m_size > 0)
        {
            return m_space[m_top];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "no element in curretn stack");        }
    }

    void clear()
    {
        m_top = -1;
        m_size = 0;
    }

    int size() const
    {
        return m_size;
    }

    bool isEmpty() const
    {
        return (m_size == 0);
    }

    bool isFull() const
    {
        return (m_size == N);
    }

    ~StaticStack()
    {

    }
};

}

#endif // STATICSTACK_H
