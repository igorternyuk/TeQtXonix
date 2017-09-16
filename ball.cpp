#include "ball.h"
#include "xonixmodel.h"
#include <QTime>

Ball::Ball(XonixModel *pModel, QObject *parent) :
    Enemy(pModel, parent)
{
    init();
}

void Ball::init()
{
   auto x = 0;
   auto y = 0;
   auto isBadPosition = true;
   qsrand(QTime::currentTime().msec());
   do {
        x = qrand() % pModel_->getFieldWidth();
        y = qrand() % pModel_->getFieldHeight();
        auto isNoWaterPaint = pModel_->getPointState(QPoint(x,y)) != PointState::WATER;
        auto hasCollisionWithOtherBalls = pModel_->getBallsPositions().contains(QPoint(x,y));
        //Если случайная точка находится на воде и не имееет коллизии с уже имеющимися точками
        //то она нам подходит
        isBadPosition = isNoWaterPaint || hasCollisionWithOtherBalls;
   } while(isBadPosition);
   position_.setX(x);
   position_.setY(y);
   // Генерируем случайные числа из множества {-1,1} т.к.
   // шарики должны двигаться только по диагонали
   static const int powerOfVelocityValuesSet = 2; // Мощность множества возможных значений скорости шарика
   auto dx = qrand() % powerOfVelocityValuesSet ? 1 : -1;
   velocity_.setX(dx);
   auto dy = qrand() % powerOfVelocityValuesSet ? 1 : -1;
   velocity_.setY(dy);
}

void Ball::updateVelocity()
{
    auto x = position_.x(), y = position_.y();
    auto dx = velocity_.x(), dy = velocity_.y();
    // Проверка на столкновение шарика с вертикальными стенками
    if(pModel_->getPointState(QPoint(x + dx, y)) == PointState::LAND)
        velocity_.setX(-velocity_.x());
    // Проверка на столкновения шарика с горизонтальными стенками
    if(pModel_->getPointState(QPoint(x, y + dy)) == PointState::LAND)
        velocity_.setY(-velocity_.y());
    // Проверка шарика на столкновение с другими шариками
    if(pModel_->getBallsPositions().contains(QPoint(x + dx, y + dy))) {
        velocity_.setX(-velocity_.x());
        velocity_.setY(-velocity_.y());
    }
}

bool Ball::hasCollisionWithXonixOrTrack() const
{
    QPoint nextBallPosition(position_.x(),position_.y());
    return pModel_->getPointState(nextBallPosition) == PointState::TRACK ||
           pModel_->getXonixPosition() == nextBallPosition;
}
