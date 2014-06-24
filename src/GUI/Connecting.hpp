#ifndef CONNECTING_H
#define CONNECTING_H

#include <QtGui/QWidget>
#include <QtCore/QTimer>
#include <QtGui>

#include "ui_Connecting.h"

#include "../Client/DisCODeClient.hpp"
#include "../Client/TaskProxy.hpp"

class Connecting : public QWidget
{
    Q_OBJECT

public:
    Connecting(QWidget *parent = 0);
    ~Connecting();

    void setup(DisCODe::Client * client, QString host, QString port);

signals:
	void connected();
	void aborted();
	void failed();

public slots:
	void proceed();
	void abort();

private:
	void closeEvent(QCloseEvent *e);

    Ui::ConnectingClass ui;
    DisCODe::Client * m_client;
    DisCODe::TaskProxy * m_task;
    QString m_host;
    QString m_port;

    QTimer m_timer;

    int state;
    int tries;
};

#endif // CONNECTING_H
