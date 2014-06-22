#ifndef COMPONENTWIDGET_HPP
#define COMPONENTWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QMap>

#include "../Client/ComponentProxy.hpp"
#include "../Client/SystemProxy.hpp"

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
	explicit ComponentWidget(DisCODe::ComponentProxy * proxy, DisCODe::SystemProxy * system, QWidget *parent = 0);
    virtual ~ComponentWidget();

public slots:
	void triggerHandler(const QString & name);

	void setProperty(QWidget * widget);

	void setBump(int bump);

private:
	DisCODe::ComponentProxy * m_proxy;
	DisCODe::SystemProxy * m_system;

	QMap<QString, QString> m_prop_edits;
};

#endif // COMPONENTWIDGET_HPP

