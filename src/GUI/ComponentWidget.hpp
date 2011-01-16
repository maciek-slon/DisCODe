#ifndef COMPONENTWIDGET_HPP
#define COMPONENTWIDGET_HPP

#include <QWidget>

#include "../Client/ComponentProxy.hpp"

class ComponentWidget : public QWidget
{
    Q_OBJECT

public:
	explicit ComponentWidget(DisCODe::ComponentProxy * proxy, QWidget *parent = 0);
    ~ComponentWidget();

private:
	DisCODe::ComponentProxy * m_proxy;
};

#endif // COMPONENTWIDGET_HPP
