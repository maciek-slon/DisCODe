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
}

SubtaskItem::~SubtaskItem()
{

}

void SubtaskItem::on_btnStart_clicked() {
	emit startSubtask(ui.nName->text());
	ui.nState->setText("Running");
}

void SubtaskItem::on_btnStop_clicked() {
	emit stopSubtask(ui.nName->text());
	ui.nState->setText("Stopped");
}
