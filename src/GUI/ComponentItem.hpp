#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QtGui/QWidget>
#include "ui_ComponentItem.h"

template <typename T>
struct PtrLess // public std::binary_function<bool, const T*, const T*>
{
  bool operator()(const T* a, const T* b) const
  {
    // may want to check that the pointers aren't zero...
    return *a < *b;
  }
};

class ComponentItem : public QWidget
{
    Q_OBJECT

public:
    ComponentItem(QWidget *parent = 0);
    ComponentItem(QString name, QString type, QString executor, int priority, QWidget *parent = 0);
    ~ComponentItem();

    QString getName() { return ui.nName->text(); }

    bool operator<(const ComponentItem & other) const;

private:
    Ui::ComponentItem ui;

    QString m_exec;
    int m_prio;
};

#endif // COMPONENTITEM_H
