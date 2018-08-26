#ifndef DUALSTATICLINKLIST_H
#define DUALSTATICLINKLIST_H

#include "DualLinkList.h"

namespace MyLib
{

template <typename T>
class DualStaticLinkList : public DualLinkList<T>
{
    typedef typename DualLinkList<T>::Node Node;
public:
    DualStaticLinkList ()
    {
    }

    bool insert(const T& e)
    {
        return DualLinkList<T>::insert(this->m_length, e);
    }

    bool insert(int i, const T& e)
    {
        return DualLinkList<T>::insert(i, e);
    }

    bool remove(int i)
    {
        return DualLinkList<T>::remove(i);
    }

    bool set(int i, const T& e)
    {
        return DualLinkList<T>::set(i, e);
    }

    T get(int i) const
    {
        return DualLinkList<T>::get(i);
    }

    bool get(int i, T& e) const
    {
        return DualLinkList<T>::get(i, e);
    }

    int find(const T& e) const
    {
        return DualLinkList<T>::find(e);
    }

    int length() const
    {
        return DualLinkList<T>::length();
    }

    void clear()
    {
        return DualLinkList<T>::clear();
    }

    bool move(int i, int step = 1)
    {
        return DualLinkList<T>::move(i, step);
    }

    bool end()
    {
        return DualLinkList<T>::end();
    }

    T current()
    {
        return DualLinkList<T>::current();
    }

    bool next()
    {
        return DualLinkList<T>::next();
    }

    bool pre()
    {
        return DualLinkList<T>::pre();
    }


};


}

#endif // DUALSTATICLINKLIST_H
