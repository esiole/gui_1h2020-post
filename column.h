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
    Column(int width, int maxHeight);
    ~Column() override;
    void setNewGeometry(int width, int height);

public slots:
    void enableAnimation();
    void disableAnimation();
    void updateAnimation();

private slots:
    void timerRedraw();
    void upHeight();

private:
    int bottom;             // максимально возможная высота столбика (уровень дна)
    int width;              // ширина столбика
    int height;             // текущая высота столбика
    const int deltaTimer;   // время в мс для таймера генерации высоты нового уровня
    float deltaUp;          // расстояние для движения на один тик для плавной анимации
    QTimer* timer;          // таймер для генерации высоты нового уровня
    QTimer* upperTimer;     // таймер для плавной анимации движения

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    virtual QRectF boundingRect() const override;
    int getRandomHeight() const;
};

#endif // COLUMN_H
