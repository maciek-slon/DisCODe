#include "ComponentWidget.hpp"

#include <QtGui>
#include <iostream>

ComponentWidget::ComponentWidget(DisCODe::ComponentProxy * proxy, QWidget *parent) :
	QWidget(parent),
	m_proxy(proxy)
{

	QVBoxLayout * top_layout = new QVBoxLayout;

	// Properties
	int pc = proxy->countProperties();

	if (pc > 0) {
		QSignalMapper * signalMapper = new QSignalMapper(this);
		QGroupBox * group_box = new QGroupBox(QString(proxy->name().c_str()) + " properties");
		QGridLayout * layout = new QGridLayout;

		for (int i = 0; i < pc; ++i) {
			QLineEdit * edit = new QLineEdit(proxy->getPropertyValue(i).c_str());
			QString prop_name = proxy->getPropertyName(i).c_str();
			edit->setObjectName(prop_name);

			edit->setToolTip(prop_name);
			edit->setStatusTip(prop_name);

			layout->addWidget(new QLabel(proxy->getPropertyName(i).c_str()), i, 0);
			layout->addWidget(edit, i, 1);
			signalMapper->setMapping(edit, i);
			connect(edit, SIGNAL(returnPressed()), signalMapper, SLOT (map()));
			m_prop_edits[i] = edit;
		}

		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setProperty(int)));

		group_box->setLayout(layout);
		top_layout->addWidget(group_box);
	}


	int hc = proxy->countHandlers();

	if (hc > 0) {
		QSignalMapper * signalMapper = new QSignalMapper(this);
		QGroupBox * group_box = new QGroupBox(QString(proxy->name().c_str()) + " event handlers");
		QGridLayout * layout = new QGridLayout;

		for (int i = 0; i < hc; ++i) {
			QPushButton * button = new QPushButton(proxy->getHandler(i).c_str());
			layout->addWidget(button);
			signalMapper->setMapping(button, QString(proxy->getHandler(i).c_str()));
			connect(button, SIGNAL(clicked()), signalMapper, SLOT (map()));
		}

		connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(triggerHandler(QString)));

		group_box->setLayout(layout);
		top_layout->addWidget(group_box);
	}

	top_layout->addStretch();
	this->setLayout(top_layout);
}

ComponentWidget::~ComponentWidget()
{
}

void ComponentWidget::triggerHandler(const QString & name) {
	m_proxy->triggerHandler(name.toStdString());
	//std::cout << name.toStdString() << std::endl;
}

void ComponentWidget::setProperty(int i) {
	m_proxy->setPropertyValue(i, m_prop_edits[i]->text().toStdString());
	//std::cout << name.toStdString() << std::endl;
}

