#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"compression.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QDrag>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void display();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);


private slots:
    void on_openButton_clicked();

    void on_codeButton_clicked();

    void on_decodeButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Compression *process;
    QString path;
    int z;

};

#endif // MAINWINDOW_H
