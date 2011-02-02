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
			QWidget * emitter = NULL;

			//std::cout << pname.toStdString() << ": " << ptype.toStdString() << std::endl;

			if (ptype == "b") {
				QCheckBox * check = new QCheckBox(pname);
				check->setChecked(boost::lexical_cast<bool>(proxy->getPropertyValue(i)));

				connect(check, SIGNAL(toggled(bool)), signalMapper, SLOT(map()));

				widget = check;
			} else
			if (ptype == "i") {
				QSpinBox * spin = new QSpinBox;
				spin->setValue(boost::lexical_cast<int>(proxy->getPropertyValue(i)));

				connect(spin, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));

				widget = spin;
			} else if (ptype == "combo") {
				QComboBox * combo = new QComboBox;

				QStringList consts;
				std::vector<std::string> vcon = proxy->getPropertyConstraints(i);

				for (int cc = 0; cc < vcon.size(); ++cc) {
					std::cout << vcon[cc] << std::endl;
					consts.append(vcon[cc].c_str());
				}

				combo->addItems(consts);

				combo->setCurrentIndex(combo->findText(proxy->getPropertyValue(i).c_str()));

				connect(combo, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT (map()));
				widget = combo;
			} else if (ptype == "range") {
				QWidget * w = new QWidget;
				QHBoxLayout * lay = new QHBoxLayout;
				lay->setSpacing(3);
				lay->setMargin(0);

				QSlider * slider = new QSlider;
				slider->setOrientation(Qt::Horizontal);

				QSpinBox * spin = new QSpinBox;

				std::vector<std::string> vcon = proxy->getPropertyConstraints(i);

				if (vcon.size() > 0)
					slider->setMinimum(boost::lexical_cast<int>(vcon[0]));

				if (vcon.size() > 1)
					slider->setMaximum(boost::lexical_cast<int>(vcon[vcon.size()-1]));

				lay->addWidget(new QLabel(QString::number(slider->minimum())));
				lay->addWidget(slider);
				lay->addWidget(new QLabel(QString::number(slider->maximum())));

				spin->setRange(slider->minimum(), slider->maximum());

				connect(slider, SIGNAL(valueChanged(int)), spin, SLOT(setValue(int)));
				connect(spin, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

				slider->setValue(boost::lexical_cast<int>(proxy->getPropertyValue(i)));

				connect(slider, SIGNAL(valueChanged(int)), signalMapper, SLOT (map()));

				QFrame * line = new QFrame;
				line->setFrameShape(QFrame::VLine);
				line->setFrameStyle(QFrame::Sunken);
				lay->addWidget(line);
				lay->addWidget(spin);

				w->setLayout(lay);

				emitter = slider;
				widget = w;
			} else {
				QLineEdit * edit = new QLineEdit(proxy->getPropertyValue(i).c_str());

				connect(edit, SIGNAL(returnPressed()), signalMapper, SLOT (map()));

				widget = edit;
			}

			if (!emitter)
				emitter = widget;

			emitter->setObjectName(pname);
			emitter->setToolTip(pttip);
			emitter->setStatusTip(pttip);

			layout->addWidget(new QLabel(pname), i, 0);
			layout->addWidget(widget, i, 1);

			signalMapper->setMapping(emitter, emitter);
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
	if (widget->inherits("QCheckBox")) {
		QCheckBox * check = qobject_cast<QCheckBox*>(widget);
		val = check->isChecked() ? "1" : "0";
	} else
	if (widget->inherits("QLineEdit")) {
		QLineEdit * edit = qobject_cast<QLineEdit*>(widget);
		val = edit->text();
	} else if (widget->inherits("QSpinBox")) {
		QSpinBox * spin = qobject_cast<QSpinBox*>(widget);
		val = spin->text();
	} else if (widget->inherits("QComboBox")) {
		QComboBox * combo = qobject_cast<QComboBox*>(widget);
		val = combo->itemText ( combo->currentIndex() );
	} else if (widget->inherits("QSlider")) {
		QSlider * slider = qobject_cast<QSlider*>(widget);
		val = boost::lexical_cast<std::string>(slider->value()).c_str();
	} else {
	}

	m_proxy->setPropertyValue(widget->objectName().toStdString(), val.toStdString());
	//std::cout << name.toStdString() << std::endl;
}

