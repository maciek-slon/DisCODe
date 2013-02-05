#include "ComponentItem.hpp"

ComponentItem::ComponentItem(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

ComponentItem::ComponentItem(QString name, QString type, QString executor,
		int priority, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);

	ui.nName->setText(name);
	ui.nExecutor->setText(executor);
	ui.nType->setText(type);
	ui.nPriority->setText(QString::number(priority));

}

ComponentItem::~ComponentItem()
{

}
