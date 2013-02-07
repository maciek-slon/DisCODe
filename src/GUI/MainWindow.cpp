#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ComponentWidget.hpp"
#include "ComponentItem.hpp"

#include <QtGui>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_connected = false;

    client = NULL;
	ui->treeWidget->clear();

	ui->dockWidget->hide();
	ui->mainToolBar->hide();
	ui->menuBar->hide();
	ui->scrollArea->setWidget(&wp);

	connect(&wp, SIGNAL(do_connect_sig(const QString &, const QString &)), this, SLOT(do_connect(const QString &, const QString &)));

	QIcon * appicon = new QIcon;
	appicon->addFile(":/icons/app", QSize(256,256));
	QApplication::setWindowIcon(*appicon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::tryToConnect(const QString & host, const QString & port) {
	client = new DisCODe::Client(host.toStdString(), port.toStdString());
	if (client->connected())
		return true;
	else {
		delete client;
		client = NULL;
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(host + ":" + port + " - connection refused.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return false;
	}
}

void MainWindow::do_connect(const QString & host, const QString & port) {
	if (tryToConnect(host, port)) {
		setup(client);
		m_connected = true;
		ui->actionConnect->setIcon(QIcon(":/icons/disconnect"));

		ui->dockWidget->show();
		ui->menuBar->show();
		ui->mainToolBar->show();
		ui->scrollArea->takeWidget();
	}
}

void MainWindow::do_disconnect() {
	delete client;
	delete task;
	client = NULL;
	ui->actionConnect->setIcon(QIcon(":/icons/connect"));

	ui->treeWidget->clear();
	ui->scrollArea->takeWidget();

	foreach (QWidget * cw, component_props)
		delete cw;

	component_props.clear();

	m_connected = false;

	ui->dockWidget->hide();
	ui->mainToolBar->hide();
	ui->menuBar->hide();
	wp.reset();
	ui->scrollArea->setWidget(&wp);
}

void MainWindow::setup(DisCODe::Client * c) {
	client = c;
	task = new DisCODe::TaskProxy(client);
	task->refresh();

	system = new DisCODe::SystemProxy(client);

	ui->treeWidget->clear();

	QTreeWidgetItem * i_task = new QTreeWidgetItem;
	i_task->setText(0, client->host().c_str());
	i_task->setText(1, client->port().c_str());

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




			ComponentItem *myItem = new ComponentItem(cp->name().c_str(), "type", ex->name().c_str(), 0);
			QListWidgetItem *item = new QListWidgetItem();
			item->setSizeHint(QSize(0,50));
			ui->listComponents->addItem(item);
			ui->listComponents->setItemWidget(item,myItem);
		}

		i_task->addChild(i_ex);
	}

	ui->treeWidget->addTopLevelItem(i_task);
	ui->treeWidget->expandAll();
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem * item, int /*column*/) {
	if (!item->parent()) { // Task
	} else if (!item->parent()->parent()) { // Executor
	} else { // Component
		ui->scrollArea->takeWidget();
		ui->scrollArea->setWidget(component_props[item->text(0)]);
	}
}

void MainWindow::on_listComponents_itemClicked(QListWidgetItem * item) {
	ui->scrollArea->takeWidget();
	ui->scrollArea->setWidget(component_props[dynamic_cast<ComponentItem*>(ui->listComponents->itemWidget(item))->getName()]);
}

void MainWindow::on_actionConnect_triggered(bool /*checked*/) {
	if (m_connected) {
		do_disconnect();
	} else {
		//do_connect();
	}
}

void MainWindow::on_actionFinish_triggered(bool /*checked*/) {
	system->finish();
	do_disconnect();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (m_connected) {
		QMessageBox msgBox;
		msgBox.setText("DisCODe is still runnig.");
		msgBox.setInformativeText("Do you want to terminate DisCODe?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Yes);
		int ret = msgBox.exec();
		
		switch (ret) {
		case QMessageBox::Yes:
			system->finish();
			event->accept();
			break;
			
		case QMessageBox::No:
			event->accept();
			break;
		
		case QMessageBox::Cancel:
			event->ignore();
			break;
		default:
			// should never be reached
			break;
		}
		
	} else {
		event->accept();
	}
}
