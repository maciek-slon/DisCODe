#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem * item, int column);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
