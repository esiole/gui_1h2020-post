#include "column.h"

Column::Column(int bottom, int width) : QGraphicsObject()
{
    this->bottom = bottom;
    this->width = width;
    height = bottom / 2;
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Column::timerRedraw);
    timer->start(300);
}

void Column::timerRedraw()
{
    height = getRandomHeight();
    update(0, 0, width, bottom);
}

void Column::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPolygon polygon;
    polygon << QPoint(0, bottom - height) << QPoint(0, bottom) << QPoint(width, bottom) << QPoint(width, bottom - height);
    painter->setBrush(Qt::red);
    painter->drawPolygon(polygon);
}

QRectF Column::boundingRect() const
{
    return QRectF(0, 0, width, bottom);
}

int Column::getRandomHeight() const
{
    return qrand() % (bottom + 1);
}
