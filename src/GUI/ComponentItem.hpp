#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QtGui/QWidget>
#include "ui_ComponentItem.h"

class ComponentItem : public QWidget
{
    Q_OBJECT

public:
    ComponentItem(QWidget *parent = 0);
    ComponentItem(QString name, QString type, QString executor, int priority, QWidget *parent = 0);
    ~ComponentItem();

    QString getName() { return ui.nName->text(); }

private:
    Ui::ComponentItem ui;
};

#endif // COMPONENTITEM_H
