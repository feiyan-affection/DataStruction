#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include "Pointer.h"
#include <cstdlib>
#include "Exception.h"

namespace MyLib
{

template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if(this->m_ref)
        {
            (*this->m_ref)++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if(p)
        {
            this->m_ref = static_cast<int*>(std::malloc(sizeof(int)));

            if(this->m_ref)
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create SharedPointer object...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
    }

    SharedPointer<T>& operator =(const SharedPointer<T>& obj)
    {
        if(this != &obj)
        {
            clear();

            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if(ref)
        {
            (*ref)--;

            if(*ref == 0)
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

template <typename T>
bool operator ==(const SharedPointer<T>& obj1, const SharedPointer<T>& obj2)
{
    return (obj1.get() == obj2.get());
}

template <typename T>
bool operator !=(const SharedPointer<T>& obj1, const SharedPointer<T>& obj2)
{
    return !(obj1 == obj2);
}

}
#endif // SHAREDPOINTER_H
