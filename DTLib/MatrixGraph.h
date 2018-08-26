#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace MyLib
{

// N:顶点个数  V:顶点的类型  E:权值的类型
template <int N, typename V, typename E>
class MartixGraph : public Graph<V, E>
{
protected:
    V* m_vertexes[N];
    E* m_edges[N][N];
    int m_eCount;
public:
    MartixGraph()
    {
        for(int i=0; i<vertexCount(); i++)
        {
            m_vertexes[i] = NULL;

            for(int j=0; j<vertexCount(); j++)
            {
                m_edges[i][j] = NULL;
            }
        }

        m_eCount = 0;
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
    
    bool getVertex(int i, V& value)     // O(1)
    {
        bool ret = ((0 <= i) && (i < vertexCount()));
        
        if(ret)
        {
            if(m_vertexes[i])
            {
                value = *(m_vertexes[i]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex...");
            }
        }
        
        return ret;
    }
    
    bool setVertex(int i, const V& value)       // O(1)
    {
        bool ret = ((0 <= i) && (i < vertexCount()));
        
        if(ret)
        {
            V* data = m_vertexes[i];

            if(data == NULL)
            {
                data = new V();
            }
            
            if(data)
            {
               *data = value;

                m_vertexes[i] = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new vettex value...");
            }
        }
        
        return ret;
    }
    
    SharedPointer<Array<int>> getAdjacent(int i)        // O(n)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vertexCount()))
        {
            int n = 0;

            for(int j=0; j<vertexCount(); j++)
            {
                if(m_edges[i][j])
                {
                    n++;
                }
            }

            ret = new DynamicArray<int>(n);

            if(ret)
            {
                for(int j=0, k=0; j<vertexCount(); j++)
                {
                    if(m_edges[i][j])
                    {
                        ret->set(k++, j);
                    }
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object...");
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
        return ((0 <= i) && (i < vertexCount()) && (0 <= j) && (j < vertexCount()) && (m_edges[i][j] != NULL));
    }

    E getEdge(int i, int j)
    {
        E ret;

        if(!getEdge(i, j, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "index <i, j> is invalid...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value)        // O(1)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
            if(m_edges[i][j])
            {
                value = *(m_edges[i][j]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value)      // O(1)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
           E* ne = m_edges[i][j];

           if(ne == NULL)
           {
               ne = new E();

               if(ne)
               {
                   *ne  = value;

                   m_edges[i][j] = ne;

                   m_eCount++;
               }
               else
               {
                   THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new edge calue...");
               }
           }
           else
           {
               *ne = value;
           }
        }

        return ret;
    }

    bool removeEdge(int i, int j)       // O(1)
    {
        bool ret = ( (0 <= i) && (i < vertexCount()) &&
                     (0 <= j) && (j < vertexCount()) );

        if(ret)
        {
            E* toDel = m_edges[i][j];

            m_edges[i][j] = NULL;

            if(toDel)
            {
                m_eCount--;

                delete toDel;
            }
        }

        return ret;
    }

    int vertexCount()
    {
        return N;
    }

    int edgeCount()
    {
        return m_eCount;
    }

    int OD(int i)       // O(n)
    {
        int ret = 0;

        if((0 <= i) && (i < vertexCount()))
        {
            for(int j=0; j<vertexCount(); j++)
            {
                if(m_edges[i][j])
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    int ID(int i)       // O(n)
    {
        int ret = 0;

        if((0 <= i) && (i < vertexCount()))
        {
            for(int j=0; j<vertexCount(); j++)
            {
                if(m_edges[j][i])
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    ~MartixGraph()
    {
        for(int i=0; i<vertexCount(); i++)
        {
            for(int j=0; j<vertexCount(); j++)
            {
                delete m_edges[i][j];
            }

            delete m_vertexes[i];
        }
    }
};

}

#endif // MATRIXGRAPH_H
