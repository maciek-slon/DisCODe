#include "ComponentWidget.hpp"

#include <QtGui>
#include <iostream>

#include <boost/lexical_cast.hpp>

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
			QString ptype = proxy->getPropertyType(i).c_str();
			QString pname = proxy->getPropertyName(i).c_str();
			QString pttip = proxy->getPropertyToolTip(i).c_str();

			QWidget * widget;

			if (ptype == "i") {
				QSpinBox * spin = new QSpinBox;
				spin->setValue(boost::lexical_cast<int>(proxy->getPropertyValue(i)));

				connect(spin, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));

				widget = spin;
			} else {
				QLineEdit * edit = new QLineEdit(proxy->getPropertyValue(i).c_str());

				connect(edit, SIGNAL(returnPressed()), signalMapper, SLOT (map()));

				widget = edit;
			}

			widget->setObjectName(pname);
			widget->setToolTip(pttip);
			widget->setStatusTip(pttip);

			layout->addWidget(new QLabel(pname), i, 0);
			layout->addWidget(widget, i, 1);

			signalMapper->setMapping(widget, widget);
		}

		connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setProperty(QWidget*)));

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

void ComponentWidget::setProperty(QWidget * widget) {
	QString val;
	if (widget->inherits("QLineEdit")) {
		QLineEdit * edit = qobject_cast<QLineEdit*>(widget);
		val = edit->text();
	} else if (widget->inherits("QSpinBox")) {
		QSpinBox * spin = qobject_cast<QSpinBox*>(widget);
		val = spin->text();
	}

	m_proxy->setPropertyValue(widget->objectName().toStdString(), val.toStdString());
	//std::cout << name.toStdString() << std::endl;
}

