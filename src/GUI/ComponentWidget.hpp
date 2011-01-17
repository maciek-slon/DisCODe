#ifndef COMPONENTWIDGET_HPP
#define COMPONENTWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QMap>

#include "../Client/ComponentProxy.hpp"

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
	explicit ComponentWidget(DisCODe::ComponentProxy * proxy, QWidget *parent = 0);
    virtual ~ComponentWidget();

public slots:
	void triggerHandler(const QString & name);

	void setProperty(int i);

private:
	DisCODe::ComponentProxy * m_proxy;

	QMap<int, QLineEdit *> m_prop_edits;
};

#endif // COMPONENTWIDGET_HPP
