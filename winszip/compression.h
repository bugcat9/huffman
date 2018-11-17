#ifndef COMPRESSION_H
#define COMPRESSION_H

#include"huffman.h"
#include<QString>
#include<stdio.h>
#include<QObject>
#include<QThread>
#include<QString>
#include<QTime>

//一个小数据
struct  Info
{
    unsigned char  ch;                    //'a'
    quint64        count;                       //出现的频度 23
    QString      str;                      //“10010”
    quint16      len;                 //长度为5  , UIN T  temp<<=len ;

    Info(long count = 0)                      //构造函数
    {
        ch = 0;
        this->count = count;
        len = 0;
        //value = 0;
    }

    ~Info()                                 //析构函数
    {
        ch = 0;
        count = 0;
        len = 0;
        str.clear();
    }

    bool operator<=(Info R)        //运算符重载
    {
        return count <= R.count;
    }

    bool operator >(Info R)
    {
        return count>R.count;
    }

    Info  operator+(Info &R)
    {
        long temp = this->count + R.count;
        return Info(temp);
    }

    bool operator<=(Info *R)        //运算符重载
    {
        return count <= R->count;
    }

    bool operator >(Info *R)
    {
        return count>R->count;
    }

    Info  operator+(Info *R)
    {
        long temp = this->count + R->count;
        return Info(temp);
    }
};

enum programe
{
    coding,
    decoding
};

//压缩类
class Compression:public QThread
{
      Q_OBJECT
public:
   explicit   Compression(QObject *parent = 0);
    ~Compression();
    bool CreatInfo(QString path);                               //得到基本信息创建树
    void Coding(QString path);                                 //压缩函数
    void Decoding(QString path);                               //解压函数
    void setCode(HuffmanNode<Info> *node);
    void clear();

    int sum;                //字符的总个数
    int currentcount;       //是否达到100分之一
    int percentage;         //100分之一
    programe condition;     //进程的状态，是压缩还是解压
    QString MYpath;         //文件路径
    bool isoK;

signals:
    void mysigals();
protected:
    void run();
private:
    Info My_Infos[256];		//储存ASCii码的转化而成的数组
    Info *bInfo;			//建树时需要传的信息
    int size;              //文件中字符类型的个数
    HuffmanTree<Info> *my_Tree;
    QTime time;         //测试时间
};

#endif // COMPRESSION_H
