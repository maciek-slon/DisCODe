#include "Connecting.hpp"
#include <QProcess>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

Connecting::Connecting(QWidget *parent) :
	QWidget(parent), m_client(NULL), m_task(NULL), state(0), tries(0)
{
	ui.setupUi(this);
	this->hide();

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(proceed()));
}

Connecting::~Connecting()
{

}

void Connecting::setup(DisCODe::Client* client, QString host, QString port) {
	m_client = client;
	m_host = host;
	m_port = port;

	ui.pbServer->setMaximum(0);
	ui.pbTask->setMaximum(1);
	ui.pbTask->setValue(0);

	m_timer.start(500);
}

void Connecting::proceed() {
	tries++;

	// waiting for server reply
	if (state == 0) {
		if (m_client->connect(m_host.toStdString(), m_port.toStdString())) {
			state = 1;

			ui.pbServer->setMaximum(1);
			ui.pbServer->setValue(1);
			ui.pbTask->setMaximum(0);

			m_task = new DisCODe::TaskProxy(m_client);

			tries = 0;
		}
	}

	// waiting for task to start
	if (state == 1) {
		m_timer.stop();
		emit connected();
	}

	if (tries > 10) {
		m_timer.stop();
		emit failed();
	}

}
