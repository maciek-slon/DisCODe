#include "TextRoller.hpp"

#include <QPainter>
#include <QEasingCurve>

TextRoller::TextRoller(QWidget *parent) :
    QWidget(parent)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(animate()));
    m_progress = 0;

    m_text << "TextRoller";

    m_length = 500;
}

TextRoller::~TextRoller()
{
}

void TextRoller::setText(const QString &text) {
    m_text << text;
    if (!m_timer.isActive()) {
        m_timer.start(m_length/20);
        m_progress = 0;
    }
}

void TextRoller::resetText(const QString &text) {
    m_text.clear();
    m_text << text;
    m_timer.stop();
    m_progress = 0;
    update();
}

void TextRoller::setLength(int length) {
    m_length = length;
}

void TextRoller::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (m_progress > 0)
        animateRoll(m_text[0], m_text[1], m_progress, painter);
    else
        painter.drawText(rect(), Qt::AlignCenter | Qt::AlignHCenter, m_text[0]);
}

void TextRoller::animate() {
    m_progress = m_progress + 5;
    if (m_progress >= 100) {
        m_progress = 0;
        m_text.pop_front();
        if (m_text.size() == 1)
            m_timer.stop();
    }
    update();
}

void TextRoller::animateRoll(const QString &s1, const QString &s2, int progress, QPainter &painter) {
    float perc = 1.0 * progress / 100;

    QFontMetrics fm = fontMetrics();
    int s1w = fm.width(s1);
    int s2w = fm.width(s2);
    int sh = fm.height();

    int sw = qMax(s1w, s2w);

    int w = width(), h = height();
    int x = w/2 - sw/2;
    int y = h/2 - sh/2;
    w = sw;
    h = sh;

    QEasingCurve easing(QEasingCurve::InOutCubic);

    QPolygonF from, to;
    QTransform trans;

    painter.setOpacity(easing.valueForProgress(1.0 - perc));
    from << QPointF(x, y) << QPointF(x, y+h) << QPointF(x+w, y+h) << QPointF(x+w, y);
    // down
    //to << QPointF(x, y+perc*h) << QPointF(x+perc*w/20, y+h) << QPointF(x+w-perc*w/20, y+h) << QPointF(x+w, y+perc*h);
    // up
    to << QPointF(x+perc*w/20, y) << QPointF(x, y+(1-perc)*h) << QPointF(x+w, y+(1-perc)*h) << QPointF(x+w-perc*w/20, y);
    QTransform::quadToQuad(from, to, trans);
    painter.setTransform(trans);
    painter.drawText(rect(), Qt::AlignCenter | Qt::AlignHCenter, s1);

    painter.setOpacity(easing.valueForProgress(perc));
    to.clear();
    // down
    //to << QPointF(x+(1-perc)*w/20, y) << QPointF(x, y+perc*h) << QPointF(x+w, y+perc*h) << QPointF(x+w-(1-perc)*w/20, y);
    // up
    to << QPointF(x, y+(1-perc)*h) << QPointF(x+(1-perc)*w/20, y+h) << QPointF(x+w-(1-perc)*w/20, y+h) << QPointF(x+w, y+(1-perc)*h);
    QTransform::quadToQuad(from, to, trans);
    painter.setTransform(trans);
    painter.drawText(rect(), Qt::AlignCenter | Qt::AlignHCenter, s2);
}
