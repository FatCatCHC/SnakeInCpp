#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>

static int zs= 0;

class wall : public QGraphicsItem
{
public:
    wall();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QPainterPath shape() const;

private:
    QPointF		S;
    QPointF		X;
    QPointF		Z;
    QPointF		Y;
    QList<QPointF>  tail;


};

#endif // WALL_H
