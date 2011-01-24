#ifndef HOSTSELECTOR_H
#define HOSTSELECTOR_H

#include <QtGui/QDialog>
#include "ui_HostSelector.h"

#include "../Client/DisCODeClient.hpp"

class HostSelector : public QDialog
{
    Q_OBJECT

public:
    HostSelector(QWidget *parent = 0);
    virtual ~HostSelector();

    DisCODe::Client * getClient() {
    	return client;
    }

public slots:
	void on_btnCancel_clicked();

	void on_btnConnect_clicked();

private:
    Ui::HostSelectorClass ui;

    DisCODe::Client * client;
};

#endif // HOSTSELECTOR_H
