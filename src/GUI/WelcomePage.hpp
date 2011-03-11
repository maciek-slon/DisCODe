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
	void do_connect_sig(const QString & host, const QString & port);

public slots:
	void on_btnConnect_clicked();
	void on_btnConnectOk_clicked();
	void on_btnConnectCancel_clicked();

	void on_btnSpawn_clicked();
	void on_btnSpawnOk_clicked();
	void on_btnSpawnCancel_clicked();

	void on_btnLoadTask_linkActivated ( const QString & link );

private:
    Ui::WelcomePageClass ui;
};

#endif // WELCOMEPAGE_H
