#include <iostream>
#include "ListGraph.h"

using namespace  std;
using namespace MyLib;

int serach_max_path(Graph<int, int>& g, int v, Array<int>& count, Array< LinkList<int>* >& path, Array<bool>& mark)
{
    int ret = 0;
    SharedPointer< Array<int> > aj = g.getAdjacent(v);

    for(int i=0; i<aj->length(); i++)
    {
        int num  = 0;

        if(!mark[(*aj)[i]])
        {
            num = serach_max_path(g, (*aj)[i], count, path, mark);
        }
        else
        {
            num = count[(*aj)[i]];
        }

        if(ret < num)
        {
            ret = num;
        }
    }

    for(int i=0; i<aj->length(); i++)
    {
        if( ret == count[(*aj)[i]] )
        {
            path[v]->insert((*aj)[i]);
        }
    }

    ret++;

    count[v] = ret;
    mark[v] = true;

    return ret;
}

SharedPointer< Graph<int, int> > create_graph(int* a, int len)
{
    ListGraph<int, int>* ret = new ListGraph<int, int>(len);

    for(int i=0; i<len; i++)
    {
        ret->setVertex(i, a[i]);
    }

    for(int i=0; i<len; i++)
    {
        for(int j=i+1; j<len; j++)
        {
            if(a[i] <= a[j])
            {
                ret->setEdge(i, j, 1);
            }
        }
    }

    return ret;
}

void init_array( Array<int>& count, Array< LinkList<int>* >& path, Array<bool>& mark)
{
    for(int i=0; i<count.length(); i++)
    {
        count[i] = 0;
    }

    for(int i=0; i<path.length(); i++)
    {
        path[i] = new LinkList<int>();
    }

    for(int i=0; i<mark.length(); i++)
    {
        mark[i] = false;
    }

}

void print_path(Graph<int,int>& g, int v, Array< LinkList<int>* >& path, LinkList<int>& cp)
{
    cp.insert(v);               // 加入顶点号

    if(path[v]->length() > 0)
    {
        for(path[v]->move(0); !path[v]->end(); path[v]->next())
        {
            print_path(g, path[v]->current(), path, cp);
        }
    }
    else
    {
        cout << "Element: ";

        for(cp.move(0); !cp.end(); cp.next())
        {
            cout << g.getVertex(cp.current()) << " ";
        }

        cout << endl;
    }

    cp.remove(cp.length() - 1);
}

void print_max_path(Graph<int, int>& g, Array<int>& count, Array< LinkList<int>* >& path)
{
    int max = 0;
    LinkList<int> cp;

    for(int i=0; i<count.length(); i++)
    {
        if(max < count[i])
        {
            max = count[i];
        }
    }

    cout << "len: " << max << endl;

    for(int i=0; i<count.length(); i++)
    {
        if(max == count[i])
        {
            print_path(g, i, path, cp);
        }
    }
}

void serach_max_path(Graph<int, int>& g, Array<int>& count, Array< LinkList<int>* >& path, Array<bool>& mark)
{
    for(int i=0; i<g.vertexCount(); i++)
    {
        if(!mark[i])
        {
            serach_max_path(g, i, count, path, mark);
        }
    }
}

void solution(int* a, int len)
{
    DynamicArray<int> count(len);
    DynamicArray< LinkList<int>* > path(len);
    DynamicArray<bool> mark(len);
    SharedPointer< Graph<int, int> > g;

    g = create_graph(a, len);

    init_array(count, path, mark);

    serach_max_path(*g, count, path, mark);

    print_max_path(*g, count, path);
}

int main()
{
    int a[]=  {5, 4, 3, 2, 1};

    solution(a, sizeof(a)/sizeof(*a));


    return 0;
}
