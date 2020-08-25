#include <QPainter>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"

static const qreal SNAKE_SIZE = 10;

Snake::Snake(GameController &controller) :
    head(0, 0),
    growing(7),
    speed(3),
    moveDirection(NoMove),
    controller(controller)
{
}


//minx,y为蛇的身体最小坐标，maxx,y为蛇的身体最大坐标，中间部分就是蛇的身体
//mapFromScene用于把坐标系映射到实际场景中

QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach (QPointF p, tail)
    {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF t1 = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(t1.x(),
                          t1.y(),
                          br.x() - t1.x() + SNAKE_SIZE,
                          br.y() - t1.y() + SNAKE_SIZE);

    return bound;
}

//决定了蛇的身体
QPainterPath Snake::shape() const
{
    QPainterPath path;
    //对指定路径进行绘制
    path.setFillRule(Qt::WindingFill);

    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, tail)
    {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }

    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    //用绿色填充身体
    painter->fillPath(shape(), Qt::green);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction)
{
    moveDirection = direction;
}

void Snake::advance(int step)
{
    if(!step)
    {
        return;
    }
    //tickCounter为内部计时器
    if(tickCounter++ % speed != 0)
    {
        return;
    }
    if(moveDirection == NoMove)
    {
        return;
    }

    if(growing > 0)
    {
        QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    }
    else
    {
        tail.takeFirst();
        tail << head;
    }

    switch (moveDirection)
    {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
    }
    setPos(head);
    handlecollisions();
}

void Snake::moveLeft()
{
    head.rx() -= SNAKE_SIZE;
    if(head.rx() < -100)
    {
        controller.snakeHitWall(this);
    }
}

void Snake::moveRight()
{
    head.rx() += SNAKE_SIZE;
    if(head.rx() > 100)
    {
        controller.snakeHitWall(this);
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE;
    if(head.ry() < -100)
    {
        controller.snakeHitWall(this);
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE;
    if(head.ry() > 100)
    {
        controller.snakeHitWall(this);
    }
}


//检查碰撞，函数默认为Qt::IntersectsItemShape
//Qt::ContainsItemShape：如果被检测物的形状（shape()）完全包含在检测物内，算做碰撞；
//Qt::IntersectsItemShape：如果被检测物的形状（shape()）与检测物有交集，算做碰撞；
//Qt::ContainsItemBoundingRect：如果被检测物的包含矩形（boundingRect()）完全包含在检测物内，算做碰撞；
//Qt::IntersectsItemBoundingRect：如果被检测物的包含矩形（boundingRect()）与检测物有交集，算做碰撞。

void Snake::handlecollisions()
{
    //取得所有碰撞的元素
    QList<QGraphicsItem *> collisions = collidingItems();

    foreach (QGraphicsItem *collidingItem, collisions)
    {
        //QGraphicsItem中设置GO_Food就是一个food
        if(collidingItem->data(GD_Type) == GO_Food)
        {
            controller.snakeAteFood(this, (Food *)collidingItem);
            growing += 1;
        }
    }

    if(tail.contains(head))
    {
        controller.snakeAteItself(this);
    }
}
