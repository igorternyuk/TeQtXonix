#include <QDebug>

#include "enemy.h"
#include "xonixmodel.h"

Enemy::Enemy(XonixModel *pModel, QObject *parent) :
    QObject(parent), pModel_(pModel)
{}

void Enemy::move()
{
    updateVelocity();
    position_.setX(position_.x() + velocity_.x());
    position_.setY(position_.y() + velocity_.y());
}

const QPoint Enemy::getPosition() const
{
    return position_;
}

const QPoint Enemy::getVelocity() const
{
    return velocity_;
}
