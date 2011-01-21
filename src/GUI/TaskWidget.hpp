#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QtGui/QWidget>
#include "ui_TaskWidget.h"

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    TaskWidget(QWidget *parent = 0);
    ~TaskWidget();

private:
    Ui::TaskWidgetClass ui;
};

#endif // TASKWIDGET_H
