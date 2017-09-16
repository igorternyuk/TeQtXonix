#include <QDebug>
#include <QTime>
#include "cube.h"
#include "xonixmodel.h"


Cube::Cube(XonixModel *pModel, QObject *parent) :
    Enemy(pModel, parent)
{
    init();
}

bool Cube::hasCollisionWithXonix() const
{
    return position_ == pModel_->getXonixPosition();
}

void Cube::setRandomPosition()
{
    qsrand(QTime::currentTime().msec());
    auto randX = 0;
    auto isBadPosition = true;
    do {
        randX = qrand() % (pModel_->getFieldWidth() - 1);
        if(randX != pModel_->getXonixPosition().x() &&
                !pModel_->getCubesPositions().contains(QPoint(randX, 0)))
            isBadPosition = false;
    } while(isBadPosition);
    position_.setX(randX);
    position_.setY(0);
}

void Cube::init()
{
    position_.setX(1);
    position_.setY(0);
    velocity_.setX(1);
    velocity_.setY(1);
}

void Cube::updateVelocity()
{
    auto x = position_.x(), y = position_.y();
    auto dx = velocity_.x(), dy = velocity_.y();
    // Проверка на столкновение кубика с вертикальными стенками
    // или границами моря
    if(pModel_->isPointOutsideOfTheField(QPoint(x + dx, y)) ||
      pModel_->getPointState(QPoint(x + dx, y)) == PointState::WATER)
        velocity_.setX(-velocity_.x());
    // Проверка на столкновения кубика с горизонтальными стенками
    // или границами моря
    if(pModel_->isPointOutsideOfTheField(QPoint(x, y + dy)) ||
       pModel_->getPointState(QPoint(x, y + dy)) == PointState::WATER)
        velocity_.setY(-velocity_.y());
    // Проверка кубика на столкновение с другими кубиками
    if(pModel_->getCubesPositions().contains(QPoint(x + dx, y + dy))) {
        velocity_.setX(-velocity_.x());
        velocity_.setY(-velocity_.y());
    }
}
