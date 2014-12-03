#include "WelcomePage.hpp"
#include <QProcess>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "Connecting.hpp"

WelcomePage::WelcomePage(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	ui.lblTitle->resetText("Where do you want to go?");
	QFont fnt(ui.lblTitle->font());
	fnt.setFamily("Ubuntu Light");
	fnt.setPointSize(16);
	ui.lblTitle->setFont(fnt);
	ui.btnErrOk->setFont(fnt);

	fnt.setPointSize(14);
	ui.lblErr->setFont(fnt);
	m_last_page = 0;

	connect(ui.btnErrOk, SIGNAL(clicked()), this, SLOT(restorePage()));
}

WelcomePage::~WelcomePage()
{

}

void WelcomePage::on_btnConnect_clicked()
{
	ui.lblTitle->setText("Connecting to DisCODe");
	ui.stackedWidget->setCurrentIndex(1);
}

void WelcomePage::on_btnConnectOk_clicked()
{
	m_last_page = 1;
	emit do_connect_sig(ui.editConnectHost->text(), ui.editConnectPort->text());
}

void WelcomePage::on_btnConnectCancel_clicked()
{
	ui.lblTitle->setText("Where do you want to go?");
	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::on_btnSpawn_clicked()
{
	ui.lblTitle->setText("Spawning DisCODe process");
	ui.stackedWidget->setCurrentIndex(2);
}

void WelcomePage::on_btnSpawnOk_clicked()
{
	QString program = "discode";
	QStringList arguments;
	arguments << "-T" << ui.editTaskName->text().trimmed();
	m_last_page = 2;

	if (!QProcess::startDetached(program, arguments))
		showError("Could not spawn DisCODe!");
	else {
		//QMessageBox::information(this, tr("Info"), tr("DisCODe spawned."));
		emit do_connect_sig(ui.editConnectHost->text(), ui.editConnectPort->text());
	}
//
//	ui.lblTitle->setText("Where do you want to go?");
//	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::on_btnSpawnCancel_clicked()
{
	ui.lblTitle->setText("Where do you want to go?");
	ui.stackedWidget->setCurrentIndex(0);
}

void WelcomePage::reset() {
	ui.lblTitle->setText("Where do you want to go?");
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

void WelcomePage::showError(const QString& text) {
	ui.lblTitle->setText("Error!");
	ui.lblErr->setText(text);
	ui.stackedWidget->setCurrentIndex(3);
}

void WelcomePage::restorePage() {
	QStringList titles;
	titles << "Where do you want to go?";
	titles << "Connecting to DisCODe";
	titles << "Spawning DisCODe process";

	ui.lblTitle->setText(titles[m_last_page]);
	ui.stackedWidget->setCurrentIndex(m_last_page);

	m_last_page = 0;
}
