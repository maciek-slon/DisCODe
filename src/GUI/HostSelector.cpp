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

}
