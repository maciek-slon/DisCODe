#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QMap>


#include "../Client/TaskProxy.hpp"
#include "../Client/ExecutorProxy.hpp"
#include "../Client/ComponentProxy.hpp"
#include "../Client/SystemProxy.hpp"
#include "../Client/DisCODeClient.hpp"

#include "WelcomePage.hpp"
#include "Connecting.hpp"

class QTreeWidgetItem;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void setClient(DisCODe::Client * c) {
    	client = c;
    }

    void setup(DisCODe::Client * c);

    void onConnectionLost() {
    	emit connectionLost();
    }

signals:
	void connectionLost();

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void on_treeWidget_itemClicked(QTreeWidgetItem * item, int column);
	void on_listComponents_itemClicked(QListWidgetItem * item);

	void on_actionConnect_triggered(bool checked);
	void on_actionFinish_triggered(bool checked);

	void do_connect(const QString & host,const QString & port);
	void do_disconnect();
	void do_disconnect_on_connectionlost();

	void onConnectionEstablished();
	void onConnectionFailed();

private:
    Ui::MainWindow *ui;

	bool tryToConnect(const QString & host, const QString & port);

	DisCODe::Client * client;
	DisCODe::TaskProxy * task;
	DisCODe::SystemProxy * system;

	QMap<QString, QWidget*> component_props;

	bool m_connected;

	WelcomePage wp;
	Connecting cn;
};

#endif // MAINWINDOW_H
