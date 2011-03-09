#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>


#include "../Client/TaskProxy.hpp"
#include "../Client/ExecutorProxy.hpp"
#include "../Client/ComponentProxy.hpp"
#include "../Client/SystemProxy.hpp"
#include "../Client/DisCODeClient.hpp"

#include "WelcomePage.hpp"

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

public slots:
	void on_treeWidget_itemClicked(QTreeWidgetItem * item, int column);

	void on_actionConnect_triggered(bool checked);
	void on_actionFinish_triggered(bool checked);

	void do_connect(const QString & host,const QString & port);
	void do_disconnect();

private:
    Ui::MainWindow *ui;

	bool tryToConnect(const QString & host, const QString & port);

	DisCODe::Client * client;
	DisCODe::TaskProxy * task;
	DisCODe::SystemProxy * system;

	QMap<QString, QWidget*> component_props;

	bool m_connected;

	WelcomePage wp;
};

#endif // MAINWINDOW_H
