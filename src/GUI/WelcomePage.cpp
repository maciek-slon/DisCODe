#include "WelcomePage.hpp"
#include <QProcess>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

WelcomePage::WelcomePage(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);
}

WelcomePage::~WelcomePage()
{

}

void WelcomePage::on_btnConnect_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void WelcomePage::on_btnConnectOk_clicked()
{
	emit do_connect_sig(ui.editConnectHost->text(), ui.editConnectPort->text());
}

void WelcomePage::on_btnConnectCancel_clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::on_btnSpawn_clicked()
{
	ui.stackedWidget->setCurrentIndex(2);
}

void WelcomePage::on_btnSpawnOk_clicked()
{
	QString program = "./discode";
	QStringList arguments;
	arguments << "-T" << ui.editTaskName->text();

	QProcess::startDetached(program, arguments);

	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::on_btnSpawnCancel_clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::on_btnLoadTask_linkActivated ( const QString & link )
{
	if (link == "load") {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("XML files (*.xml);;All files (*.*)"));

		if (fileName != "") {
			QFile file(fileName);
			if (!file.open(QIODevice::ReadOnly)) {
				QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
			} else {
				ui.editTaskName->setText(fileName);
				file.close();
			}
		}
	}

}
