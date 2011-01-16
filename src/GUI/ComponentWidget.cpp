#include "ComponentWidget.hpp"

#include <QtGui>

ComponentWidget::ComponentWidget(DisCODe::ComponentProxy * proxy, QWidget *parent) :
	QWidget(parent),
	m_proxy(proxy)
{
	QGridLayout *layout = new QGridLayout;

	layout->addWidget(new QLabel("Prop 1"), 0, 0);
	QLineEdit * p1edit = new QLineEdit;
	p1edit->setObjectName("p1edit");
	layout->addWidget(p1edit, 0, 1);

	layout->addWidget(new QLabel("Prop 2"), 1, 0);
	QLineEdit * p2edit = new QLineEdit;
	p2edit->setObjectName("p2edit");
	layout->addWidget(p2edit, 1, 1);

	layout->setRowStretch(2, 1);

	//layout->addStretch();
	this->setLayout(layout);
}

ComponentWidget::~ComponentWidget()
{
}
