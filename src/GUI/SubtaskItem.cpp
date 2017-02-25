#include "SubtaskItem.hpp"

SubtaskItem::SubtaskItem(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

SubtaskItem::SubtaskItem(QString name, QString state, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	ui.nName->setText(name);
	ui.nState->setText(state);
	if (state == "Running") {
		ui.nState->setEnabled(true);
		ui.nName->setEnabled(true);
	} else {
		ui.nState->setEnabled(false);
		ui.nName->setEnabled(false);
	}
}

SubtaskItem::~SubtaskItem()
{

}

void SubtaskItem::on_btnStart_clicked() {
	emit startSubtask(ui.nName->text());
	ui.nState->setText("Running");
	ui.nState->setEnabled(true);
	ui.nName->setEnabled(true);
}

void SubtaskItem::on_btnStop_clicked() {
	emit stopSubtask(ui.nName->text());
	ui.nState->setText("Stopped");
	ui.nState->setEnabled(false);
	ui.nName->setEnabled(false);
}
