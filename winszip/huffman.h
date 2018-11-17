#ifndef HUFFMAN_H
#define HUFFMAN_H

#include"heap.h"
#include<QStack>
#include<QString>


template<class T>
struct HuffmanNode      //树节点的定义
{
    T data;             //节点的数据
    HuffmanNode<T> *leftChild, *rightChild, *parent;        //左右子女和 父节点指针
    HuffmanNode() :leftChild(NULL), rightChild(NULL), parent(NULL) {}
    HuffmanNode(T elem, HuffmanNode<T> *left = NULL, HuffmanNode<T> *right = NULL)
    {
        this->data = elem;
        this->leftChild = left;
        this->rightChild = right;
    }

    bool operator<=(HuffmanNode<T> &R)
    {
        return data <= R.data;
    }

    bool operator >(HuffmanNode<T> &R)
    {
        return data>R.data;
    }

    bool operator<=(HuffmanNode<T> *R)
    {
        return data<=R->data;
    }

    bool operator >(HuffmanNode<T> *R)
    {
        return data>R->data;
    }
};

//比较函数，之后会改
template<class T>
bool compare1(HuffmanNode<T> *x, HuffmanNode<T> * y)
{
    if (x->data <= y->data)
        return true;
    else
        return false;
}

template<class T>
bool compare2(HuffmanNode<T> *x, HuffmanNode<T> * y)
{
    if (x->data>y->data)
        return true;
    else
        return false;
}

template<class T>
class HuffmanTree
{
public:

    //构造函数根据数组创造树
    HuffmanTree(T w[], int n);
    ~HuffmanTree();//析构函数

    HuffmanNode<T> *getRoot()       //获取根节点
    {
        return root;
    }

protected:

    HuffmanNode<T> *root;          //根节点
    //void deleteTree(HuffmanNode<T> *t);    //删除子树
    void mergeTree(HuffmanNode<T> &ht1, HuffmanNode<T> &ht2, HuffmanNode<T> *&parent);                   //融合函数
};

template<class T>
HuffmanTree<T>::HuffmanTree(T w[], int n)       //通过数组构建哈夫曼树
{

    Heap<HuffmanNode<T> *> hp(256);    //使用最小堆放森林

    for (int i = 0; i<n; i++)
    {
        HuffmanNode<T> *work=new HuffmanNode<T>();
        work->data = w[i];
        work->leftChild = NULL;
        work->rightChild = NULL;
        work->parent = NULL;
        hp.Insert(work);
    }


    HuffmanNode<T> *parent = NULL;

    while (hp.size()>1)
    {
        HuffmanNode<T> *first, *second;
        //HuffmanNode<T> *parent = NULL;
        if (hp.RemoveMin(first) && hp.RemoveMin(second))
        {
            this->mergeTree(*first,*second, parent);
            hp.Insert(parent);
        }
    }
    hp.RemoveMin(root);

}

//析构函数（不能使用递归的函数，因为树非常大,所以采用非递归方法）
template<class T>
HuffmanTree<T>::~HuffmanTree()
{
    QStack<HuffmanNode<T> *>Q1;
    QStack<HuffmanNode<T> *>Q2;
    HuffmanNode<T> *p = NULL;
    Q1.push(root);
    while (!Q1.empty())
    {
        p = Q1.pop();
        Q2.push(p);
        if (p->rightChild != NULL)
            Q1.push(p->rightChild);
        if (p->leftChild != NULL)
            Q1.push(p->leftChild);
    }

    while (!Q2.empty())
    {
        p = Q2.pop();
        delete p;
    }
}


//融合函数
template<class T>
void HuffmanTree<T>::mergeTree(HuffmanNode<T> &ht1, HuffmanNode<T> &ht2, HuffmanNode<T> *&parent)
{
    parent = new HuffmanNode<T>( ht1.data + ht2.data);
    parent->leftChild = &ht1;
    parent->rightChild = &ht2;
    ht1.parent = parent;
    ht2.parent = parent;
}

#endif // HUFFMAN_H
