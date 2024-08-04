#include "compression.h"
#include<QStack>
#include<QString>
#include <QFile>
#include <QDataStream>
#include<QMessageBox>
#include<QDebug>
#include <stdio.h>
#include <stdlib.h>
#include<QTime>
#include<QByteArray>

//构造函数
Compression::Compression(QObject *parent):QThread(parent)
{
    bInfo = NULL;
    my_Tree = NULL;
    size = 0;
    sum = 0;
    currentcount=0;
    percentage=0;
}

//析构函数
Compression::~Compression()
{
    if(bInfo!=NULL)
        delete[]bInfo;
    if(my_Tree!=NULL)
        delete my_Tree;
    size = 0;
    sum = 0;
}

bool Compression::CreatInfo(QString path)
{


  //  time.start();     //时间函数用于测试压缩时间
    this->clear();      //清空内容
    for (int i = 0; i<256; i++)
    {
        My_Infos[i].ch = (unsigned char)i;
    }
    quint8 ch=0;

    QFile openfile(path);           //读取二进制文件
    if(!openfile.open(QIODevice::ReadOnly))
    {

        qDebug()<<QString("打开失败");
        isoK=false;
        return false;
    }


    QByteArray a;
    while(!openfile.atEnd())
    {
        a=openfile.read(1024);
        for(int i=0;i<a.size();i++)
        {
            ch=a[i];
            if (My_Infos[ch].count==0)//如果第一次读到这个字符
            {
                size++;
            }
            My_Infos[ch].count++;  //字符类型数增加
            sum++; //次数增加
        }

    }

    openfile.close();
    bInfo = new Info[size];
    for (int i = 0, j = 0; i<256; i++)
    {
        if (My_Infos[i].count != 0)
        {
            bInfo[j].ch = My_Infos[i].ch;
            bInfo[j].count = My_Infos[i].count;
            j++;
        }
    }

    percentage=sum/100;
    my_Tree = new HuffmanTree<Info>(bInfo, size); //创建树
    this->setCode(my_Tree->getRoot());//完善数组中的信息
    return true;
}

//压缩,有待优化
void Compression::Coding(QString path)
{



    QFile openfile(path);   //读文件
    if(!openfile.open(QIODevice::ReadOnly))
    {
       isoK=false;
        return ;
    }

    quint8 ch = 0;

    path=path+QString(".Nzip");     //写文件
    QFile savefile(path);
    if(!savefile.open(QIODevice::WriteOnly))
    {

        qDebug()<<"打开失败";
        isoK=false;
        return ;
    }
    QDataStream fout(&savefile);

    //将配置文件写进去
    fout<<sum;
    fout<<size;
    for(int i=0;i<size;i++)
    {
        fout<<bInfo[i].ch<<bInfo[i].count;
    }

    //进行压缩
    quint64  temp = 0;  //存储的单位
    int pos = 0;

    QByteArray a;
    while(!openfile.atEnd())
    {
        a=openfile.read(1024);
        for(int i=0;i<a.size();i++)
        {
            ch=a[i];
            currentcount++;
            QString &strRecord = My_Infos[ch].str;
            quint16 &len= My_Infos[ch].len;
            for(int i=0;i<len;i++)
            {
                temp<<=1;
                if(strRecord[i]=='1')
                {
                    temp|=1;
                }

                if(++pos==64)
                {
                    fout<<temp;
                    pos = 0;
                    temp = 0;
                }
            }

            if(currentcount==percentage)
            {
                emit mysigals();
                currentcount=0;
            }

        }

    }

    if (pos)    //最后的编码不满足8个字节,填充0
    {
        temp = temp << (64- pos);
        fout<<temp;
    }

    openfile.close();
    savefile.close();
    qDebug()<<"11111111111";
  //  qDebug()<<time.elapsed()/1000<<"s";
}

//解压
void Compression::Decoding(QString path)
{

    this->clear();

    QFile openfile(path);   //读文件
    if(!openfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,QString("失败"),QString("文件打开失败"));
        qDebug()<<QString("打开失败");
        isoK=false;
        return ;
    }
    QDataStream  fin(&openfile);

    //读配置文件
    fin>>sum;
    fin>>size;
    percentage=sum/100;
    bInfo = new Info[size];

    for(int i=0;i<size;i++)
    {
       fin>>bInfo[i].ch>>bInfo[i].count;
    }


    my_Tree = new HuffmanTree<Info>(bInfo, size); //创建树
    HuffmanNode<Info>  *p = my_Tree->getRoot();
    path=path.remove(QString(".Nzip"));
    //path="myfile.txt";
    QFile savefile(path);
    if(!savefile.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(NULL,QString("失败"),QString("文件打开失败"));
        qDebug()<<QString("打开失败");
    }
    QDataStream fout(&savefile);

    unsigned char ch = 0;
    quint64 a=0;
    quint64 temp=0;
    int count = 0;          //字符数量的记录
    while (sum>count)
    {

       fin>>a;
        for (int i=0;i<64;i++)
        {

            temp = a;
            temp >>= 63;
            if (temp==1)
                p = p->rightChild;  //向右搜索;
            else
                p = p->leftChild;	//向左搜索;
            if (p->leftChild == NULL && p->rightChild == NULL)
            {
                // 将其中的信息写到文件中 ；
                // *P = 根节点 ；
                ch = p->data.ch;
                p = my_Tree->getRoot();
                fout<<ch;
                count++;		//字符数++
                currentcount++;
                if(currentcount==percentage)
                {
                    emit mysigals();
                    currentcount=0;
                }
                if (count == sum)
                    break;
            }
            a<<=1;
        }
    }

    openfile.close();
    savefile.close();
    qDebug()<<"11111111111";

}



//设置字符
void Compression::setCode(HuffmanNode<Info> *node)
{

    QStack<HuffmanNode<Info> *>s;
    QStack<QString> s1;
    s.push(node);
    QString str;
    s1.push(str);
    HuffmanNode<Info> *p = NULL;
    while (!s.empty())
    {
        p = s.pop();

        QString temp = s1.pop();


        if (p->leftChild == NULL && p->rightChild == NULL)
        {
            QString a = temp;
            My_Infos[p->data.ch].str = a;
            My_Infos[p->data.ch].len = a.size();

        }

        if (p->rightChild != NULL)
        {
            s.push(p->rightChild);
            QString a = temp + "1";
            s1.push(a);
        }

        if (p->leftChild != NULL)
        {
            s.push(p->leftChild);
            QString a = temp + "0";
            s1.push(a);
        }
    }
}

void Compression::clear()
{
    for(int i=0;i<256;i++)
    {
        My_Infos[i].ch=0;
        My_Infos[i].count=0;
        My_Infos[i].len=0;
        My_Infos[i].str.clear();
    }


    if(bInfo!=NULL)
    {
        delete[]bInfo;
        bInfo=NULL;
    }

    if(my_Tree!=NULL)
    {
        delete my_Tree;
        my_Tree=NULL;
    }
    size = 0;
    sum = 0;
    currentcount=0;
    percentage=0;
}

//开线程
void Compression::run()
{
    isoK=true;
    if(condition==coding)
    {
      if(this->CreatInfo(MYpath))
      {
          this->Coding(MYpath);
      }

    }
    else if(condition==decoding)
        this->Decoding(MYpath);
}
