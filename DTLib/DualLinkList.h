#ifndef DUALLINKLIST_H
#define DUALLINKLIST_H

#include "LinkList.h"

namespace MyLib
{

template <typename T>
class DualLinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
        Node* pre;
    };

    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
        Node* pre;
    } m_header;

    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);

        for(int p=0; p<i; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* pn)
    {
        delete pn;
    }

public:
    DualLinkList()
    {
        m_header.next = NULL;
        m_header.pre = NULL;
        m_length = 0;
        m_step = 1;
        m_current = NULL;
    }

    bool insert(const T& e)
    {
        return insert(m_length, e);
    }

    bool insert(int i, const T& e)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if(ret)
        {
            Node* node = create();

            if(node)
            {
                Node* current = position(i);
                Node* next = current->next;

                node->value = e;

                node->next = next;
                current->next = node;

                if(current != reinterpret_cast<Node*>(&m_header))
                {
                    node->pre = current;
                }
                else
                {
                    node->pre = NULL;
                }

                if(next)
                {
                    next->pre = node;
                }

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "no memory to create DualLinkList object...");
            }
        }

        return ret;
    }

    bool remove(int i)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if(ret)
        {
            Node* current = position(i);
            Node* toDel = current->next;
            Node* next = toDel->next;

            if(m_current == toDel)
            {
                m_current = next;
            }

            current->next = next;

            if(next)
            {
                next->pre = toDel->pre;
            }

            m_length--;

            destroy(toDel);
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if(ret)
        {
            position(i)->next->value = e;
        }

        return ret;
    }

    virtual T get(int i) const
    {
        T ret;

        if(get(i, ret))
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "invalid parameter i to get element...");
        }

        return ret;
    }

    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i) && (i < m_length));

        if(ret)
        {
            e = position(i)->next->value;
        }

        return ret;
    }

    int find(const T& e) const
    {
        int ret = -1;
        int i = 0;
        Node* node = m_header.next;

        while (node)
        {
            if(node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                node = node->next;
                i++;
            }
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        while (m_length > 0)
        {
            remove(0);
        }
    }

    virtual bool move(int i, int step = 1)
    {
        bool ret = ((0 <= i) && (i < m_length) && (step > 0));

        if(ret)
        {
            m_current = position(i)->next;
            m_step = step;
        }

        return ret;
    }

    virtual bool end()
    {
        return (m_current == NULL);
    }

    virtual T current()
    {
        if(!end())
        {
            return m_current->value;
        }
        else\
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current opsition...");
        }
    }

    virtual bool next()
    {
        int i = 0;

        while((i < m_step) && !end())
        {
            m_current = m_current->next;
            i++;
        }

        return (i == m_step);
    }

    virtual bool pre()
    {
        int i = 0;

        while((i < m_step) && !end())
        {
            m_current = m_current->pre;
            i++;
        }

        return (i == m_step);
    }

    ~DualLinkList()
    {
        clear();
    }
};



}
#endif // DUALLINKLIST_H
