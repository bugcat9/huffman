#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QString>
#include<QDebug>
#include<QTime>
#include<QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process=NULL;
    this->setAcceptDrops(true);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    z=0;
    if(process==NULL)
        process=new Compression();
    //接受信号用于进度条
     connect(process,&Compression::mysigals,this,&MainWindow::display);

}

MainWindow::~MainWindow()
{
    delete ui;
    process->quit();
    if(process!=NULL)
        delete process;

}

//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

//当用户放下这个文件后，就会触发dropEvent事件
void MainWindow::dropEvent(QDropEvent *event)
{
    //注意：这里如果有多文件存在，意思是用户一下子拖动了多个文件，而不是拖动一个目录
    //如果想读取整个目录，则在不同的操作平台下，自己编写函数实现读取整个目录文件名
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    //往文本框中追加文件名
    foreach(QUrl url, urls) {
        QString file_name = url.toLocalFile();
        ui->lineEdit->setText(file_name);
        path=file_name;
    }
}

//进度条
void MainWindow::display()
{
    z++;
    ui->progressBar->setValue(z);
}


void MainWindow::on_openButton_clicked()
{
    path=QFileDialog::getOpenFileName(this,QString("选择文件"),QString("../.."),"Images (*.png *.xpm *.jpg);;"
                                                                            "Text files (*.txt);;XML files (*.xml);;"
                                      " (*.*);;");

    ui->lineEdit->setText(path);
}

//压缩
void MainWindow::on_codeButton_clicked()
{


    ui->progressBar->setValue(0);
    z=0;
    process->MYpath=path;
    process->condition=coding;

    process->start();

    qDebug()<<"开始压缩";

}

void MainWindow::on_decodeButton_2_clicked()
{


    ui->progressBar->setValue(0);
    z=0;
    process->condition=decoding;
    process->MYpath=path;

    process->start();
    qDebug()<<"开始解压";

}
