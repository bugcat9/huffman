#ifndef HEAP_H
#define HEAP_H

#include"huffman.h"

//模板构造堆
template<class T>
class Heap
{
public:
    //构造函数、析构函数
    Heap(int sz = 10);
    Heap(T arr[], int n);
    ~Heap()
    {
        delete[]heap;
    }

    int size()                      //返回堆当前的大小
    {
        return currentsize;
    }
    bool Insert(const T &x);         //插入函数
    bool RemoveMin(T &x);           //删除函数
    bool IsEmpty()const             //判空
    {
        return (currentsize == 0) ? true : false;
    }

    bool IsFull()                   //判满
    {
        return (currentsize == maxHeapsize) ? true : false;
    }

    void MakeEmpty()                //判空函数
    {
        currentsize == 0;
    }

private:
    T * heap;                             //数组
    int currentsize;                     //当前元素个数
    int maxHeapsize;                    //允许最大元素个数
    void siftDown(int start, int m);    //下滑调整
    void siftUp(int start);             //上滑调整
};

//构造函数
template<class T>
Heap<T>::Heap(int sz)
{
    maxHeapsize = sz;
    heap = new T[maxHeapsize];
    currentsize = 0;
}


template<class T>
Heap<T>::Heap(T arr[], int n)
{
    maxHeapsize = n;
    heap = new T[maxHeapsize];
    for (int i = 0; i++; i<n)    //复制数组元素到堆中
        heap[i] = arr[i];
    currentsize = n;
    int currentPos = (currentsize - 2) / 2;
    while (currentPos >= 0)
    {
        siftDown(currentPos, currentsize - 1);         //从上到下下滑调整
        currentPos--;
    }
}

//插入函数
template<class T>
bool Heap<T>::Insert(const T &x)
{
    if (currentsize == maxHeapsize)
        return false;
    heap[currentsize] = x;        //插入元素
    siftUp(currentsize);        //进行上滑调整
    currentsize++;
    return true;

}

template<class T>
bool Heap<T>::RemoveMin(T &x)
{
    if (!currentsize)
        return false;
    x = heap[0];
    heap[0] = heap[currentsize - 1];    //最后元素补到根节点
    currentsize--;
    siftDown(0, currentsize - 1);      //自上向下调整为堆
    return true;
}

template<class T>                   //下滑函数
void Heap<T>::siftDown(int start, int m)
{
    int i = start;            //开始的节点
    int j = 2 * i + 1;            //开始节点的左子女
    T temp = heap[i];
    while (j <= m)             //检查是否在最后的位置
    {
        if (j<m && compare2(heap[j],heap[j+1]))
            j++;  //让j指向两者中较小者
        if (compare1(temp,heap[j]))               //小则不调整
            break;
        else
        {
            heap[i] = heap[j]; i = j; j = 2 * j + 1;
        }
    }
    heap[i] = temp;
}

template<class T>
void Heap<T>::siftUp(int start)     //上调函数
{
    int j = start, i = (j - 1) / 2;      //j为子女，i为父节点
    T temp = heap[j];
    while (j>0)
    {
        if (compare1(heap[i],temp))
            break;
        else
        {
            heap[j] = heap[i];
            j = i;
            i = (i - 1) / 2;
        }
    }
    heap[j] = temp;
}


#endif // HEAP_H
