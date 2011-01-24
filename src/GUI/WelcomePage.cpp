#include "WelcomePage.h"
#include <QProcess>
#include <iostream>

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

WelcomePage::~WelcomePage()
{

}



void WelcomePage::on_btnConnect_clicked() {
	ui.stackedWidget->setCurrentIndex(1);
}

void WelcomePage::on_btnConnectOk_clicked() {
	emit do_connect_sig(ui.editConnectHost->text(), ui.editConnectPort->text());
}

void WelcomePage::on_btnConnectCancel_clicked() {
	ui.stackedWidget->setCurrentIndex(0);
}



void WelcomePage::on_btnSpawn_clicked() {
	ui.stackedWidget->setCurrentIndex(2);
}

void WelcomePage::on_btnSpawnOk_clicked() {
	QString program = "./discode";
	QStringList arguments;
	arguments << "-T" << "../../../share/Tasks/CameraThreshViewer.xml";
	arguments << "-S" << "\"Components.Source.<xmlattr>.type=CameraOpenCV\"";

	QProcess::startDetached(program, arguments);
}

void WelcomePage::on_btnSpawnCancel_clicked() {
	ui.stackedWidget->setCurrentIndex(0);
}
