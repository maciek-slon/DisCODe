#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ComponentWidget.hpp"

#include <QtGui>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup(DisCODe::Client * c) {
	client = c;
	task = new DisCODe::TaskProxy(client);
	task->refresh();

	ui->treeWidget->clear();

	QTreeWidgetItem * i_task = new QTreeWidgetItem;
	i_task->setText(0, "Task name");
	i_task->setText(1, "adres IP");

	for (int i = 0; i < task->countExecutors(); ++i) {
		DisCODe::ExecutorProxy * ex = task->getExecutor(i);
		QTreeWidgetItem * i_ex = new QTreeWidgetItem;
		i_ex->setText(0, ex->name().c_str());

		for (int j = 0; j < ex->countComponents(); ++j) {
			DisCODe::ComponentProxy * cp = ex->getComponent(j);
			QTreeWidgetItem * i_cp = new QTreeWidgetItem;
			i_cp->setText(0, cp->name().c_str());
			i_ex->addChild(i_cp);

			component_props[cp->name().c_str()] = new ComponentWidget(cp);
		}

		i_task->addChild(i_ex);
	}

	ui->treeWidget->addTopLevelItem(i_task);
	ui->treeWidget->expandAll();
}

void MainWindow::on_tabWidget_tabCloseRequested ( int index ) {
	//ui->tabWidget->removeTab(index);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem * item, int column) {
	//ui->tabWidget->addTab(new QWidget, item->text(column));

	if (!item->parent()) { // Task
		ui->label->setText("Task");
	} else if (!item->parent()->parent()) { // Executor
		ui->label->setText(QString("Executor: ") + item->text(0));
	} else { // Component
		ui->label->setText(QString("Component: ") + item->text(0));
		ui->scrollArea->takeWidget();
		ui->scrollArea->setWidget(component_props[item->text(0)]);
	}
}

void MainWindow::on_actionRefresh_triggered(bool checked) {

}
