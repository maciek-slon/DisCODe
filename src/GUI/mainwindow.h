#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "../Client/TaskProxy.hpp"
#include "../Client/ExecutorProxy.hpp"
#include "../Client/ComponentProxy.hpp"
#include "../Client/DisCODeClient.hpp"

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

public slots:
    void on_tabWidget_tabCloseRequested(int index);
	void on_treeWidget_itemClicked(QTreeWidgetItem * item, int column);

private:
    Ui::MainWindow *ui;


	DisCODe::Client * client;
	DisCODe::TaskProxy * task;
};

#endif // MAINWINDOW_H
