#ifndef BTREE_H
#define BTREE_H

#include "Tree.h"
#include "BTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

namespace MyLib
{

enum BTTraversal
{
    PreOrder,
    InOrder,
    PostOrder,
    LevelOrder
};

template <typename T>
class BTree : public Tree<T>
{
protected:
    LinkQueue<BTreeNode<T>*> m_queue;

    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value) const
    {
        BTreeNode<T>* ret = NULL;

        if(node)
        {
            if(node->value == value)
            {
                ret = node;
            }
            else
            {
                if(ret == NULL)
                {
                    ret = find(node->left, value);
                }

                if(ret == NULL)
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    virtual BTreeNode<T>* find(BTreeNode<T>* node, BTreeNode<T>* obj) const
    {
        BTreeNode<T>* ret = NULL;

        if(node == obj)
        {
            ret = node;
        }
        else
        {
            if(node)
            {
                if(ret == NULL)
                {
                    ret = find(node->left, obj);
                }

                if(ret == NULL)
                {
                    ret = find(node->right, obj);
                }
            }
        }

        return ret;
    }

    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;

        if(pos == ANY)
        {
            if(np->left == NULL)
            {
                np->left = n;
            }
            else if(np->right == NULL)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else if(pos == LEFT)
        {
            if(np->left == NULL)
            {
                np->left = n;
            }
            else
            {
                ret = false;
            }
        }
        else if(pos == RIGHT)
        {
            if(np->right == NULL)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    virtual bool insert(TreeNode<T>* node, BTNodePos pos)
    {
        bool ret = true;

        if(node)
        {
            if(this->m_root == NULL)
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
                BTreeNode<T>* np = find(node->parent);

                if(np)
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(node), np, pos);
                }
                else
                {
                    THROW_EXCEPTION(InvalidParameterException, "parameter parent tree node...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "parameter node canot be NULL...");
        }

        return ret;
    }

    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
    {
        bool ret =true;
        // 使用工厂模式
        BTreeNode<T>* node = BTreeNode<T>::NewNode();

        if(node)
        {
            node->value = value;
            node->parent = parent;

            ret = insert(node, pos);

            if(!ret)
            {
                delete node;
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node...");
        }

        return ret;
    }

    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();

        if(ret)
        {
            if(root() == node)
            {
                this->m_root = NULL;
            }
            else
            {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);

                if(parent->left == node)
                {
                    parent->left = NULL;
                }
                else if(parent->right == node)
                {
                    parent->right = NULL;
                }

                node->parent = NULL;

            }

            ret->m_root = node;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");;
        }
    }

    virtual void free(BTreeNode<T>* node)
    {
        if(node)
        {
            free(node->left);
            free(node->right);

            // 释放从堆空间申请的内存
            if(node->flag())
            {
                delete node;
            }
        }
    }

    int count(BTreeNode<T>* node) const
    {
        return (node) ? (count(node->left) + count(node->right) + 1) : (0);
    }

    int height(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node)
        {
            int lh = height(node->left);
            int rh = height(node->right);

            ret = ((lh > rh) ? lh : rh) + 1;
        }

        return ret;
    }

    int degree(BTreeNode<T>* node) const
    {
        int ret = 0;

//        if(node)
//        {
//            int dl = degree(node->left);
//            int dr = degree(node->right);

//            ret = (!!(node->left) + !!(node->right));

//            if(ret < dl)
//            {
//                ret = dl;
//            }

//            if(ret < dr)
//            {
//                ret = dr;
//            }
//        }
/************************************************************/

        if(node)
        {
            BTreeNode<T>* child[] = {node->left, node->right};

            ret = (!!(node->left) + !!(node->right));

            for(int i=0; (i<2) && (ret<2); i++)
            {
                int d = degree(child[i]);

                if(ret < d)
                {
                    ret = d;
                }
            }
        }

        return ret;
    }


    void preOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node)
        {
            queue.add(node);
            preOrderTraversal(node->left, queue);
            preOrderTraversal(node->right, queue);
        }
    }

    void inOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node)
        {
            inOrderTraversal(node->left, queue);
            queue.add(node);
            inOrderTraversal(node->right, queue);
        }
    }

    void postOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node)
        {
            postOrderTraversal(node->left, queue);
            postOrderTraversal(node->right, queue);
            queue.add(node);
        }
    }

    void levelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node)
        {
            LinkQueue<BTreeNode<T>*> tmp;

            tmp.add(node);

            while (tmp.length() > 0)
            {
                BTreeNode<T>* n = tmp.front();

                if(n->left)
                {
                    tmp.add(n->left);
                }

                if(n->right)
                {
                    tmp.add(n->right);
                }

                tmp.remove();
                queue.add(n);
            }
        }
    }

    BTreeNode<T>* clone(BTreeNode<T>* node) const
    {
        BTreeNode<T>* ret = NULL;

        if(node)
        {
            ret = BTreeNode<T>::NewNode();

            if(ret)
            {
                ret->value = node->value;

                ret->left = clone(node->left);
                ret->right = clone(node->right);

                if(ret->left)
                {
                    ret->left->parent = ret;
                }

                if(ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
            }
        }

        return ret;
    }

    bool equal(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        if(lh == rh)
        {
            return true;
        }
        else if(lh && rh)
        {
            return (lh->value == rh->value) && equal(lh->left, rh->left) && equal(lh->right, rh->right);
        }
        else
        {
            return false;
        }
    }

    BTreeNode<T>* add(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        BTreeNode<T>* ret = NULL;

        if((lh == NULL) && rh)
        {
            ret = clone(rh);
        }
        else if(lh && (rh == NULL))
        {
            ret = clone(lh);
        }
        else if(lh && rh)
        {
            ret = BTreeNode<T>::NewNode();

            if(ret)
            {
                ret->value = lh->value + rh->value;

                ret->left = add(lh->left, rh->left);
                ret->right = add(lh->right, rh->right);

                if(ret->left)
                {
                    ret->left->parent = ret;
                }

                if(ret->right)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
            }
        }

        return ret;
    }

    void traversal(BTTraversal order, LinkQueue<BTreeNode<T>*>& queue)
    {
        switch (order)
        {
        case PreOrder:
            preOrderTraversal(root(), queue);
            break;
        case InOrder:
            inOrderTraversal(root(), queue);
            break;
        case PostOrder:
            postOrderTraversal(root(), queue);
            break;
        case LevelOrder:
            levelOrderTraversal(root(), queue);
            break;
        default:
            THROW_EXCEPTION(InvalidParameterException, "parameter order is invalid...");
            break;
        }
    }

    BTreeNode<T>* connect(LinkQueue<BTreeNode<T>*>& queue)
    {
        BTreeNode<T>* ret = NULL;

        if(queue.length() > 0)
        {
            ret = queue.front();

            BTreeNode<T>* slider = queue.front();

            queue.remove();

            slider->left = NULL;

            while(queue.length() > 0)
            {
                slider->right = queue.front();
                queue.front()->left = slider;
                slider = queue.front();
                queue.remove();
            }

            slider->right = NULL;
        }

        return ret;
    }

public:
    bool insert(TreeNode<T>* node)
    {
        return insert(node, ANY);
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        return insert(value, parent, ANY);
    }

    SharedPointer< Tree<T> > remove(const T& value)
    {
        BTree<T>* ret = NULL;
        BTreeNode<T>* node = find(value);

        if(node)
        {
            remove(node, ret);

            m_queue.clear();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "can not find the tree node via value...");
        }

        return ret;
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        BTree<T>* ret = NULL;

        node = find(node);

        if(node)
        {
            remove(dynamic_cast<BTreeNode<T>*>(node), ret);

            m_queue.clear();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "parameter node is invalid...");
        }

        return ret;
    }

    BTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }

    BTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<BTreeNode<T>*>(node));
    }

    BTreeNode<T>* root() const
    {
        return dynamic_cast<BTreeNode<T>*>(this->m_root);
    }

    int degree() const
    {
        return degree(root());
    }

    int count() const
    {
        return count(root());
    }

    int height() const
    {
        return height(root());
    }

    void clear()
    {
        free(root());

        m_queue.clear();

        this->m_root = NULL;
    }

    bool begin()
    {
        bool ret = (root() != NULL);

        if(ret)
        {
            m_queue.clear();
            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if(ret)
        {
            BTreeNode<T>* node = m_queue.front();

            m_queue.remove();

            if(node->left)
            {
                m_queue.add(node->left);
            }

            if(node->right)
            {
                m_queue.add(node->right);
            }
        }

        return ret;
    }

    T current()
    {
        if(!end())
        {
            return m_queue.front()->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
        }
    }

    SharedPointer<Array<T>> traversal(BTTraversal order)
    {
        DynamicArray<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);

        ret = new DynamicArray<T>(queue.length());

        if(ret)
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front()->value);
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create retrun array...");
        }

        return ret;
    }

    BTreeNode<T>* thread(BTTraversal order)
    {
        BTreeNode<T>* ret = NULL;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);

        ret = connect(queue);

        this->m_root = NULL;

        m_queue.clear();

        return ret;
    }

    SharedPointer<BTree<T>> clone() const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret)
        {
            ret->m_root = clone(root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
        }

        return ret;
    }

    bool operator ==(const BTree<T>& btree) const
    {
        return equal(root(), btree.root());
    }

    bool operator !=(const BTree<T>& btree) const
    {
        return !(*this == btree);
    }

    SharedPointer<BTree<T>> add(const BTree<T>& btree) const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret)
        {
            ret->m_root = add(root(), btree.root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
        }

        return ret;
    }

    ~BTree()
    {
        clear();
    }
};


}

#endif // BTREE_H
