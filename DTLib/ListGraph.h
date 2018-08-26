#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace MyLib
{

template <typename V, typename E>
class ListGraph : public Graph<V, E>
{
protected:
    struct Vertex : public Object
    {
        V* data;
        LinkList<Edge<E>> edge;

        Vertex ()
        {
            data = NULL;
        }
    };

    LinkList<Vertex*> m_list;
public:
    ListGraph(unsigned int n = 0)
    {
        for(unsigned int i=0; i<n; i++)
        {
            addVertex();
        }
    }

    int addVertex()     // O(n)
    {
        int ret = -1;
        Vertex* v = new Vertex();

        if(v)
        {
            m_list.insert(v);

            ret = m_list.length() - 1;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new vertex object...");
        }

        return ret;
    }

    int addVertex(const V& value)       // O(n)
    {
        int ret = addVertex();

        if(ret >= 0)
        {
            setVertex(ret, value);
        }

        return ret;
    }

    bool setVertex(int i, const V& value)       // O(n)
    {
        bool ret = ((0 <= i) && (i < vertexCount()));

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            V* data = vertex->data;

            if(data == NULL)
            {
                data = new V();
            }

            if(data)
            {
                *data = value;

                vertex->data = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new vertex value...");
            }
        }

        return ret;
    }

    V getVertex(int i)
    {
        V ret;

        if(!getVertex(i, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    bool getVertex(int i, V& value)
    {
        bool ret = ((0 <= i) && (i < vertexCount()));

        if(ret)
        {
            Vertex* v = m_list.get(i);

            if(v->data)
            {
                value = *(v->data);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex...");
            }
        }

        return ret;
    }

    void removeVertex()     // O(n2)
    {
        if(m_list.length() > 0)
        {
            int index = m_list.length() - 1;
            Vertex* v = m_list.get(index);

            if(m_list.remove(index))
            {
                for(int i =(m_list.move(0), 0); !m_list.end(); i++, m_list.next())
                {
                    int pos = m_list.current()->edge.find(Edge<E>(i, index));

                    if(pos >= 0)
                    {
                        m_list.current()->edge.remove(pos);
                    }
                }

                delete v->data;
                delete v;
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No vertex in current graph...");
        }
    }

    SharedPointer<Array<int>> getAdjacent(int i)        // O(n)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vertexCount()))
        {
            Vertex* vertex = m_list.get(i);

            ret = new DynamicArray<int>(vertex->edge.length());

            if(ret)
            {
                for(int k = (vertex->edge.move(0), 0); !vertex->edge.end(); k++, vertex->edge.next())
                {
                    ret->set(k, vertex->edge.current().end);
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret objbect...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    bool isAdjacent(int i, int j)
    {
        return ((0 <= i) && (i < vertexCount()) && (0 <= j) && (j < vertexCount()) && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0));
    }

    E getEdge(int i, int j)
    {
        E ret;

        if(!getEdge(i, j, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Edge <i, j> is invalid...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                value = vertex->edge.get(pos).data;
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.set(pos, Edge<E>(i, j, value));
            }
            else
            {
                ret = vertex->edge.insert(0, Edge<E>(i, j, value));
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.remove(pos);
            }
        }

        return ret;
    }

    int vertexCount()
    {
        return m_list.length();
    }

    int edgeCount()
    {
        int ret = 0;

        for(m_list.move(0); !m_list.end(); m_list.next())
        {
            ret += m_list.current()->edge.length();
        }

        return ret;
    }

    int ID(int i)
    {
        int ret = 0;

        if((0 <= i) && (i < vertexCount()))
        {
            for(m_list.move(0); !m_list.end(); m_list.next())
            {
                LinkList<Edge<E>>& edge = m_list.current()->edge;

                for(edge.move(0); !edge.end(); edge.next())
                {
                    if(edge.current().end == i)
                    {
                        ret++;
                        break;
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "invalid i is invalid...");
        }

        return ret;
    }

    int OD(int i)
    {
        int ret = 0;

        if((0 <= i) && (i < vertexCount()))
        {
            ret = m_list.get(i)->edge.length();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "invalid i is invalid...");
        }


        return ret;
    }

    ~ListGraph()
    {
        while (m_list.length() > 0)
        {
            Vertex* toDel = m_list.get(0);

            m_list.remove(0);

            delete toDel->data;
            delete toDel;
        }
    }
};


}


#endif // LISTGRAPH_H

