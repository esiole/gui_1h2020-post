#ifndef COLUMN_H
#define COLUMN_H

#include <QObject>
#include <QGraphicsObject>
#include <QPainter>
#include <QTimer>

class Column : public QGraphicsObject
{
    Q_OBJECT

public:
    Column(int maxHeight, int width);

public slots:
    void timerRedraw();

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QRectF boundingRect() const;
    int getRandomHeight() const;

    int bottom;
    int width;
    int height;
    QTimer* timer;
};

#endif // COLUMN_H
