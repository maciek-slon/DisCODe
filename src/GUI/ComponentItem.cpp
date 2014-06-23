#include "ComponentItem.hpp"

ComponentItem::ComponentItem(QWidget *parent)
    : QWidget(parent), m_prio(0)
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

	m_exec = executor;
	m_prio = priority;
}

ComponentItem::~ComponentItem()
{

}

bool ComponentItem::operator<(const ComponentItem & other) const {
	if (m_exec != other.m_exec)
		return m_exec < other.m_exec;
	else
		return m_prio < other.m_prio;
}
