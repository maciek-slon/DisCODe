#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeWidgetItem>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabCloseRequested ( int index ) {
	ui->tabWidget->removeTab(index);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem * item, int column) {
	ui->tabWidget->addTab(new QWidget, item->text(column));
}
