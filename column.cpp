#include "column.h"

/**
 * @brief Column::Column конструктор столбика - объекта для QGraphicsScene
 * @param bottom уровень дна столбика (максимальная высота)
 * @param width ширина столбика
 */
Column::Column(int bottom, int width) : QGraphicsObject(), bottom(bottom), width(width), deltaTimer(200)
{
    height = 0;
    timer = new QTimer(this);
    upperTimer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Column::timerRedraw);
    connect(upperTimer, &QTimer::timeout, this, &Column::upHeight);
}

Column::~Column()
{

}

/**
 * @brief Column::enableAnimation включение анимации столбика
 */
void Column::enableAnimation()
{
    timer->start(deltaTimer);
}

/**
 * @brief Column::disableAnimation остановка анимации столбика
 */
void Column::disableAnimation()
{
    timer->stop();
    upperTimer->stop();
}

/**
 * @brief Column::updateAnimation обнуление уровня столбцов для создания эффекта переключения песни
 */
void Column::updateAnimation()
{
    height = 0;
    update(0, 0, width, bottom);
}

/**
 * @brief Column::timerRedraw получение нового уровня, к которому движется столбец и запуск плавной анимации
 */
void Column::timerRedraw()
{
    upperTimer->stop();
    deltaUp = (getRandomHeight() - height) / static_cast<float>(deltaTimer) * 10;
    upperTimer->start(10);
}

/**
 * @brief Column::upHeight плавная анимация движения столбика
 */
void Column::upHeight()
{
    height += static_cast<int>(deltaUp);
    update(0, 0, width, bottom);
}

void Column::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPolygon polygon;
    polygon << QPoint(0, bottom - height) << QPoint(0, bottom) << QPoint(width, bottom) << QPoint(width, bottom - height);
    painter->setBrush(Qt::red);
    painter->drawPolygon(polygon);
}

/**
 * @brief Column::boundingRect
 * @return область, которую занимает столбик
 */
QRectF Column::boundingRect() const
{
    return QRectF(0, 0, width, bottom);
}

/**
 * @brief Column::getRandomHeight генерирует случайное число
 * @return случайное число от 0 до максимално возможной высоты столбика
 */
int Column::getRandomHeight() const
{
    return qrand() % (bottom + 1);
}
