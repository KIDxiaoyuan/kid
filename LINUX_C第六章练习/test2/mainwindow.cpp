#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mainwindow2.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowIcon(QIcon(":/new/prefixl/ico"));
    button = new QPushButton(this);
    button ->setGeometry(QRect(50,50,100,25));
    button->setText("button");
    connect(button,SIGNAL(clicked(bool)),this,SLOT(showMainwindow2()));
}
void MainWindow::showMainwindow2()
{
    w2.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
