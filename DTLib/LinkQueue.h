#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include "Queue.h"
#include "LinkList.h"
#include "Exception.h"
#include "LinuxList.h"

namespace MyLib
{

template <typename T>
class LinkQueue : public Queue<T>
{
protected:
    // 以下注释掉的是使用组合LinkList单链表实现的链式队列, 但是不够高效
    // 为了提高效率，又采用Linux内核链表（双向链表）实现链式队列
    // LinkList<T> m_list;

    struct Node : public Object
    {
        list_head head;
        T value;
    };

    list_head m_header;
    int m_length;
public:
    LinkQueue()
    {
        m_length = 0;

        INIT_LIST_HEAD(&m_header);
    }

    void add(const T& e)
    {
        // m_list.insert(e);

        Node* node = new Node();

        if(node)
        {
            node->value = e;

            list_add_tail(&node->head, &m_header);

            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "no memory to add new element...");
        }
    }

    void remove()
    {
//        if(m_list.length() > 0)
//        {
//            m_list.remove(0);
//        }
//        else
//        {
//            THROW_EXCEPTION(InvalidOperationException, "no element in curretn queue...");
//        }

        if(m_length > 0)
        {
            list_head* toDel = m_header.next;

            list_del(toDel);

            m_length--;

            delete list_entry(toDel, Node, head);
        }
    }

    T front() const
    {
//        if(m_list.length() > 0)
//        {
//            return m_list.get(0);
//        }
//        else
//        {
//            THROW_EXCEPTION(InvalidOperationException, "no element in curretn queue...");
//        }

        if(m_length > 0)
        {
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "no element in curretn queue...");
        }
    }

    void clear()
    {
//        m_list.clear();

        while(m_length > 0)
        {
            remove();
        }
    }

    int length() const
    {
//        return m_list.length();

        return m_length;
    }

    ~LinkQueue()
    {
        clear();
    }
};


}
#endif // LINKQUEUE_H
