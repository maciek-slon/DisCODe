#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QtGui/QWidget>
#include "ui_WelcomePage.h"

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    WelcomePage(QWidget *parent = 0);
    ~WelcomePage();

signals:
	void do_connect_sig();

public slots:
	void on_btnConnect_clicked();
	void on_btnSpawn_clicked();

private:
    Ui::WelcomePageClass ui;
};

#endif // WELCOMEPAGE_H
