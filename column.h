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
    void enableAnimation();
    void disableAnimation();
    void updateAnimation();

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QRectF boundingRect() const;
    int getRandomHeight() const;

    int bottom;
    int width;
    int height;
    int deltaTimer;
    QTimer* timer;
};

#endif // COLUMN_H
