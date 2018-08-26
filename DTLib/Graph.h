#ifndef GRAPH_H
#define GRAPH_H

#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"

namespace MyLib
{

template <typename E>
struct Edge : public Object
{
    int begin;
    int end;
    E data;

    Edge(int i=-1, int j=-1)
    {
        begin = i;
        end = j;
    }

    Edge(int i, int j, const E& value)
    {
        begin = i;
        end =  j;
        data = value;
    }

    bool operator ==(const Edge<E>& obj)
    {
        return (begin == obj.begin) && (end == obj.end);
    }

    bool operator !=(const Edge<E>& obj)
    {
        return !(*this == obj);
    }

    bool operator > (const Edge<E>& obj)
    {
        return (data < obj.data);
    }

    bool operator < (const Edge<E>& obj)
    {
        return (data < obj.data);
    }
};


template <typename V, typename E>
class Graph : public Object
{
protected:
    template <typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if(ret)
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front());
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object...");
        }

        return ret;
    }

    SharedPointer< Array< Edge<E> > > getUndirectedEdges()
    {
        DynamicArray< Edge<E> >* ret = NULL;

        if(asUndirected())
        {
            LinkQueue< Edge<E> > queue;

            for(int i=0; i<vertexCount(); i++)
            {
                for(int j=i; j<vertexCount(); j++)
                {
                    if(isAdjacent(i, j))
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }

            ret = toArray(queue);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "this function is for undirection graph only...");
        }

        return ret;
    }

    int find(Array<int>& p, int v)
    {
        while(p[v] != -1)
        {
            v = p[v];
        }

        return v;
    }
public:
    virtual V getVertex(int i) = 0;
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;
    virtual SharedPointer< Array<int> > getAdjacent(int i) = 0;
    virtual bool isAdjacent(int i, int j) = 0;
    virtual E getEdge(int i, int j) = 0;
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;
    virtual bool removeEdge(int i, int j) = 0;
    virtual int vertexCount() = 0;
    virtual int edgeCount() = 0;
    virtual int OD(int i) = 0;
    virtual int ID(int i) = 0;
    virtual int TD(int i)
    {
        return OD(i) + ID(i);
    }

    bool asUndirected()
    {
        bool ret = true;

        for(int i=0; i<vertexCount(); i++)
        {
            for(int j=0; j<vertexCount(); j++)
            {
                if(isAdjacent(i, j))
                {
                    ret = ret && isAdjacent(j, i) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }

        return ret;
    }

    SharedPointer< Array<Edge<E>> > prim(const E& LIMIT, const bool MINIMUM = true)
    {
        LinkQueue< Edge<E> > ret;

        if(asUndirected())
        {
            DynamicArray<int> adjVex(vertexCount());
            DynamicArray<bool> mark(vertexCount());
            DynamicArray<E> cost(vertexCount());
            SharedPointer< Array<int> > aj = NULL;
            bool end = false;
            int v = 0;

            for(int i=0; i<vertexCount(); i++)
            {
                adjVex[i] = -1;
                mark[i] = false;
                cost[i] = LIMIT;
            }

            mark[v] = true;
            aj = getAdjacent(v);

            for(int j=0; j<aj->length(); j++)
            {
                cost[(*aj)[j]] = getEdge(v, (*aj)[j]);
                adjVex[(*aj)[j]] = v;
            }

            for(int i=0; (i<vertexCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1;

                for(int j=0; j<vertexCount(); j++)
                {
                    if(!mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1);

                if(!end)
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));

                    mark[k] = true;

                    aj = getAdjacent(k);

                    for(int j=0; j<aj->length(); j++)
                    {
                        if(!mark[(*aj)[j]] && (MINIMUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : (getEdge(k, (*aj)[j]) > cost[(*aj)[j]])))
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "prim operation is for undirected graph only...");
        }

        if(ret.length() != (vertexCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation...");
        }

        return toArray(ret);
    }

    SharedPointer< Array< Edge<E> > > kurskal(const bool MINMUM = true)
    {
        LinkQueue< Edge<E> > ret;
        SharedPointer< Array<Edge<E> > > edges = getUndirectedEdges();
        DynamicArray<int> p(vertexCount());

        for(int i=0; i<p.length(); i++)
        {
            p[i] = -1;
        }

        Sort::Shell(*edges, MINMUM);

        for(int i=0; (i < edges->length()) && (ret.length() < (vertexCount() - 1)); i++)
        {
            int b = find(p, (*edges)[i].begin);
            int e = find(p, (*edges)[i].end);

            if(b != e)
            {
                p[e] = b;

                ret.add((*edges)[i]);
            }
        }

        if(ret.length() != (vertexCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for kurskal operation...");
        }

        return toArray(ret);
    }

    SharedPointer< Array<int> > BFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vertexCount()))
        {
            LinkQueue<int> q;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vertexCount());

            for(int i=0; i<visited.length(); i++)
            {
                visited[i] = false;
            }

            q.add(i);

            while(q.length() > 0)
            {
                int v = q.front();

                q.remove();

                if(!visited[v])
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for(int j=0; j<aj->length(); j++)
                    {
                        q.add((*aj)[j]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    SharedPointer< Array<int> > DFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i) && (i < vertexCount()))
        {
            LinkStack<int> s;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vertexCount());

            for(int j=0; j<vertexCount(); j++)
            {
                visited[j] = false;
            }

            s.push(i);

            while(s.size() > 0)
            {
                int v = s.top();

                s.pop();

                if(!visited[v])
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v);

                    for(int j=aj->length() - 1; j>=0; j--)
                    {
                        s.push((*aj)[j]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret  = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index i is invalid...");
        }

        return ret;
    }

    SharedPointer< Array<int> > dijkstra(int i, int j, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= i) && (i < vertexCount()) && (0 <= j) && (j < vertexCount()))
        {
            DynamicArray<E> dist(vertexCount());       // 存储路径的值
            DynamicArray<int> path(vertexCount());     // 存储当前节点的前驱节点
            DynamicArray<bool> mark(vertexCount());    // 标记节点是否进入了S集合

            for(int k=0; k<vertexCount(); k++)
            {
                mark[k] = false;
                path[k] = -1;

                dist[k] = isAdjacent(i, k) ? (path[k]=i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true;

            for(int k=0; k<vertexCount(); k++)
            {
                E m = LIMIT;
                int u = -1;

                for(int w=0; w<vertexCount(); w++)
                {
                    if(!mark[w] && (dist[w] < m))
                    {
                        m = dist[w];
                        u = w;
                    }
                }

                if( u == -1 )
                {
                    break;
                }

                mark[u] = true;

                for(int w=0; w<vertexCount(); w++)
                {
                    if( !mark[w] && isAdjacent(u, w) && (dist[u] + getEdge(u, w) < dist[w]))
                    {
                        dist[w] = dist[u] + getEdge(u, w);
                        path[w] = u;
                    }
                }
            }

            LinkStack<int> s;

            s.push(j);

            for(int k=path[j]; k!=-1; k=path[k])
            {
                s.push(k);
            }

            while(s.size() > 0)
            {
                ret.add(s.top());

                s.pop();
            }
        }
        else
        {
             THROW_EXCEPTION(InvalidParameterException, "index <i, j> is invalid...");
        }

        if(ret.length() < 2)
        {
            THROW_EXCEPTION(ArithmeticException, "there is no path from i to j...");
        }

        return toArray(ret);
    }

    SharedPointer< Array<int> > floyd(int x, int y, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= x) && (x < vertexCount()) && (0 <= y) && (y < vertexCount()))
        {
            DynamicArray< DynamicArray<E> > dist(vertexCount());
            DynamicArray< DynamicArray<E> > path(vertexCount());

            for(int k=0; k<vertexCount(); k++)
            {
                dist[k].resize(vertexCount());
                path[k].resize(vertexCount());
            }

            for(int i=0; i<vertexCount(); i++)
            {
                for(int j=0; j<vertexCount(); j++)
                {
                    path[i][j] = -1;
                    dist[i][j] = isAdjacent(i, j) ? (path[i][j]=j, getEdge(i, j)) : LIMIT;
                }
            }

            for(int k=0; k<vertexCount(); k++)
            {
                for(int i=0; i<vertexCount(); i++)
                {
                    for(int j=0; j<vertexCount(); j++)
                    {
                        if( (dist[i][k]) + dist[k][j] < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k];
                        }
                    }
                }
            }

            while((x != -1) && (x != y))
            {
                ret.add(x);
                x = path[x][y];
            }

            if(x != -1)
            {
                ret.add(x);
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "index <x, y> is invalid...");
        }

        if(ret.length() < 2)
        {
            THROW_EXCEPTION(ArithmeticException, "there is no path from i to j...");
        }


        return toArray(ret);
    }

};

}

#endif // GRAPH_H
