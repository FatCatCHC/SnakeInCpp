#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMessageBox>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"
#include "wall.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this))
{
    //开启游戏循环的定时器，间隔为1000/33=30，每秒30桢
    timer.start( 1000/33 );

    Food *a1 = new Food(0, -50);
    scene.addItem(a1);

    scene.addItem(snake);
    wall *a2 = new wall();
    scene.addItem(a2);
    scene.installEventFilter(this);

    resume();
}

GameController::~GameController()
{

}

void GameController::snakeAteFood(Snake *snake, Food *food)
{
    scene.removeItem(food);
    delete food;

    addNewFood();
}

void GameController::snakeHitWall(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::snakeAteItself(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

/*
重写键盘输入
*/
void GameController::handleKeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        snake->setMoveDirection(Snake::MoveLeft);
        break;
    case Qt::Key_Right:
        snake->setMoveDirection(Snake::MoveRight);
        break;
    case Qt::Key_Up:
        snake->setMoveDirection(Snake::MoveUp);
        break;
    case Qt::Key_Down:
        snake->setMoveDirection(Snake::MoveDown);
        break;
    case Qt::Key_Escape:
        pause();
        break;
    case Qt::Key_Space:
        resume();
        break;
    }
}

void GameController::addNewFood()
{
    int x, y;
    do
    {
        x = (int(qrand() % 200)-100) / 10;
        y = (int(qrand() % 200)-100) / 10;

        x *=10;
        y *=10;
    }
    while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))));

    Food *food = new Food(x , y);
    scene.addItem(food);
}

void GameController::gameOver()
{
    scene.clear();
    QMessageBox::information(NULL, tr("告知"), tr("game over"), QMessageBox::Yes);

    snake = new Snake(*this);
    scene.addItem(snake);
    addNewFood();
    wall *a2 = new wall;
    scene.addItem(a2);
}

//断开定时器(Esc键)
void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    QMessageBox::information(NULL, tr("告知"),tr("游戏暂停"));
}

//开启定时器(space键)
void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
}

//事件过滤器
bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    }
    else
    {
        return QObject::eventFilter(object, event);
    }
}


