#include "HostSelector.hpp"

#include <QMessageBox>

HostSelector::HostSelector(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	setResult(QDialog::Accepted);
}

HostSelector::~HostSelector()
{

}

void HostSelector::on_btnCancel_clicked() {
	reject();
}

void HostSelector::on_btnConnect_clicked() {
	client = new DisCODe::Client(ui.editHost->text().toStdString());
	if (client->connected())
		accept();
	else {
		delete client;
		client = NULL;
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setText(ui.editHost->text() + ":" + ui.editPort->text() + " - connection refused.");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
	}

}
