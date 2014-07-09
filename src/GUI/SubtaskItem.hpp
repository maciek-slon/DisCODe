#ifndef SUBTASKITEM_H
#define SUBTASKITEM_H

#include <QWidget>
#include "ui_SubtaskItem.h"

class SubtaskItem : public QWidget
{
    Q_OBJECT

public:
    SubtaskItem(QWidget *parent = 0);
    SubtaskItem(QString name, QString state, QWidget *parent = 0);
    ~SubtaskItem();

    QString getName() { return ui.nName->text(); }

protected slots:
	void on_btnStart_clicked();
	void on_btnStop_clicked();

signals:
	void startSubtask(const QString & name);
	void stopSubtask(const QString & name);

private:
    Ui::SubtaskItem ui;
};

#endif // SUBTASKITEM_H
