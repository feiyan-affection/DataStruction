#include "Object.h"
#include <cstdlib>
#include <iostream>

using namespace std;

namespace MyLib
{
void* Object::operator new (unsigned int size) throw()
{
    // cout << "Object::operator new : size = " << size << endl;
    return malloc(size);
}

void Object::operator delete (void* p)
{
    // cout << "Object::operator delete : " << p << endl;
    free(p);
}

void* Object::operator new[] (unsigned int size) throw()
{
    return malloc(size);
}

void Object::operator delete[] (void* p)
{
    free(p);
}

bool Object::operator ==(const Object& obj)
{
	return (this == &obj);
}

bool Object::operator !=(const Object& obj)
{
	return (this == &obj);
}

Object::~Object()
{

}

}
