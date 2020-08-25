#include <QGraphicsView>
#include <QTimer>

#include "mainwindow.h"
#include "gamecontroller.h"
#include "constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this)),
      game(new GameController(*scene, this))
{
    setWindowTitle(tr("QKSword"));
    setCentralWidget(view);
    resize(600, 600);

    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{

}

//在扩展时保持纵横比
void MainWindow::adjustViewSize()
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

//设置场景的范围，左上角为（-100，-100），长宽为200的矩阵(坐标系)
void MainWindow::initScene()
{
    scene->setSceneRect(-100, -100, 200, 200);
}

//设置多个矩形
void MainWindow::initSceneBackground()
{
    QPixmap bg(TILE_SIZE, TILE_SIZE);
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, TILE_SIZE, TILE_SIZE);




    view->setBackgroundBrush(QBrush(bg));
}
