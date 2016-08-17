#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.button = new QPushButton("login",this);
    w.button->setGeometry(QRect(100,100,100,25));
    connect(w.button,SIGNAL(released(),this,SLOT(w->txtButton()));

    w.show();

    return a.exec();
}
