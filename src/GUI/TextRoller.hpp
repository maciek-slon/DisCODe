#ifndef TEXTROLLER_HPP
#define TEXTROLLER_HPP

#include <QWidget>
#include <QTimer>

class TextRoller : public QWidget
{
    Q_OBJECT
    
public:
    explicit TextRoller(QWidget *parent = 0);
    ~TextRoller();
    
public slots:
    void setText(const QString & text);
    void resetText(const QString & text);

    void setLength(int length);

protected slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

    void animateRoll(const QString & s1, const QString & s2, int progress, QPainter & painter);

private:
    QStringList m_text;
    QTimer m_timer;

    int m_progress;

    int m_length;
};

#endif // TEXTROLLER_HPP
