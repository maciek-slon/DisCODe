#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ComponentWidget.hpp"
#include "ComponentItem.hpp"
#include "SubtaskItem.hpp"

#include <QtGui>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_connected = false;

    client = new DisCODe::Client;
    client->setConnectionLostHandler(boost::bind(&MainWindow::onConnectionLost, this));

	tabifyDockWidget(ui->wComponents, ui->wSubtasks);

	ui->wComponents->hide();
	ui->wSubtasks->hide();
	ui->mainToolBar->hide();
	ui->menuBar->hide();
	ui->scrollArea->setWidget(&wp);

	connect(&wp, SIGNAL(do_connect_sig(const QString &, const QString &)), this, SLOT(do_connect(const QString &, const QString &)));
	connect(this, SIGNAL(connectionLost()), this, SLOT(do_disconnect_on_connectionlost()), Qt::QueuedConnection);

	connect(&cn, SIGNAL(connected()), this, SLOT(onConnectionEstablished()));
	connect(&cn, SIGNAL(failed()), this, SLOT(onConnectionFailed()));
	connect(&cn, SIGNAL(aborted()), this, SLOT(onConnectionAborted()));

	QIcon * appicon = new QIcon;
	appicon->addFile(":/icons/app", QSize(256,256));
	QApplication::setWindowIcon(*appicon);

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

	wp.ui.stackedWidget->addWidget(&cn);
	cn.setTextRoller(wp.ui.lblTitle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSubtask(const QString& name) {
	task->startSubtask(name.toStdString());
}

void MainWindow::stopSubtask(const QString& name) {
	task->stopSubtask(name.toStdString());
}

bool MainWindow::tryToConnect(const QString & host, const QString & port) {
	if (client->connect(host.toStdString(), port.toStdString()))
		return true;
	else {
		wp.showError(host + ":" + port + " - connection refused.");
		return false;
	}
}

void MainWindow::do_connect(const QString & host, const QString & port) {
	wp.ui.stackedWidget->setCurrentIndex(4);
	cn.setup(client, host, port);
}

void MainWindow::onConnectionEstablished() {
	setup(client);
	m_connected = true;
	ui->actionConnect->setIcon(QIcon(":/icons/disconnect"));

	ui->wComponents->show();
	ui->wSubtasks->show();
	//ui->menuBar->show();
	ui->mainToolBar->show();
	ui->scrollArea->takeWidget();
}

void MainWindow::onConnectionFailed() {
	client->disconnect();

	QString msg = QString(client->host().c_str()) + ":" + QString(client->port().c_str()) + " - connection refused.";

	wp.showError(msg);
}

void MainWindow::onConnectionAborted() {
	client->disconnect();
	wp.restorePage();
}

void MainWindow::do_disconnect() {
	client->disconnect();
	delete task;
	ui->actionConnect->setIcon(QIcon(":/icons/connect"));

	ui->scrollArea->takeWidget();

	foreach (QWidget * cw, component_props)
		delete cw;

	component_props.clear();

	m_connected = false;

	ui->wComponents->hide();
	ui->wSubtasks->hide();
	ui->mainToolBar->hide();
	ui->menuBar->hide();
	wp.reset();
	ui->scrollArea->setWidget(&wp);
}

void MainWindow::do_disconnect_on_connectionlost() {
	if (!m_connected) return;
	do_disconnect();

	wp.showError("Connection with DisCODe is lost.");
}

void MainWindow::setup(DisCODe::Client * c) {
	client = c;
	task = new DisCODe::TaskProxy(client);
	task->refresh();

	system = new DisCODe::SystemProxy(client);

	ui->listComponents->clear();
	ui->listSubtasks->clear();

	QVector <ComponentItem*> items;

	for (int i = 0; i < task->countExecutors(); ++i) {
		DisCODe::ExecutorProxy * ex = task->getExecutor(i);

		for (int j = 0; j < ex->countComponents(); ++j) {
			DisCODe::ComponentProxy * cp = ex->getComponent(j);

			component_props[cp->name().c_str()] = new ComponentWidget(cp, system);

			ComponentItem *myItem = new ComponentItem(cp->name().c_str(), cp->getType().c_str(), ex->name().c_str(), cp->getPriority());
			items.push_back(myItem);
		}

	}

	qSort(items.begin(), items.end(), PtrLess<ComponentItem>());
	Q_FOREACH(ComponentItem * myItem, items) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setSizeHint(QSize(0,50));
		ui->listComponents->addItem(item);
		ui->listComponents->setItemWidget(item,myItem);
	}

	for (int i = 0; i < task->countSubtasks(); ++i) {
		QString state;
		if (task->subtaskState(task->getSubtask(i).c_str()) == DisCODe::TaskProxy::Running)
			state = "Running";
		else
			state = "Stopped";

		SubtaskItem *sitem = new SubtaskItem(task->getSubtask(i).c_str(), state, this);
		QListWidgetItem *item = new QListWidgetItem();
		item->setSizeHint(QSize(0,50));
		ui->listSubtasks->addItem(item);
		ui->listSubtasks->setItemWidget(item,sitem);
		connect(sitem, SIGNAL(startSubtask(const QString &)), this, SLOT(startSubtask(const QString &)));
		connect(sitem, SIGNAL(stopSubtask(const QString &)), this, SLOT(stopSubtask(const QString &)));
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
