#include <QtGui/QApplication>
#include "mainwindow.h"
#include "HostSelector.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    HostSelector hs;

    int res = hs.exec();

    if (res == QDialog::Accepted) {
    	w.setup(hs.getClient());
    	w.show();
        return a.exec();
    } else {
    	w.close();
    	return 0;
    }
}
