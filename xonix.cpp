#include "xonix.h"
#include "xonixmodel.h"
#include <QDebug>

Xonix::Xonix(XonixModel *pModel, QObject *parent) :
    QObject(parent), pModel_(pModel)
{
     livesNumber_ = INITIAL_LIVES_NUMBER;
     init();
}

void Xonix::init()
{
    position_.setX(pModel_->getFieldWidth() / 2);
    position_.setY(0);
    direction_ = Direction::STOP;
    isOnTheWater_ = false;
    isSelfCrossing_ = false;
    if(!pModel_->isGameStarted())
        livesNumber_ = INITIAL_LIVES_NUMBER;
}

void Xonix::move()
{
    //Устанавливаем направление для ксоникса
    switch(direction_) {
        case Direction::LEFT :
            position_.setX(position_.x() - 1);
            break;
        case Direction::UP :
            position_.setY(position_.y() - 1);
            break;
        case Direction::RIGHT :
            position_.setX(position_.x() + 1);
            break;
        case Direction::DOWN :
            position_.setY(position_.y() + 1);
            break;
        default:
            break;
    }
    // Конролируем выход ксоникса за границы игрового поля
    if(position_.x() < 0) position_.setX(0);
    if(position_.y() < 0) position_.setY(0);
    if(position_.x() > pModel_->getFieldWidth() - 1)
        position_.setX(pModel_->getFieldWidth() - 1);
    if(position_.y() > pModel_->getFieldHeight() - 1)
        position_.setY(pModel_->getFieldHeight() - 1);
    //Если только что установленной позиции ксоникса соответствует клетка поля
    //помеченная как след - значит ксоникс пересек свой путь
    isSelfCrossing_ = pModel_->getPointState(position_) == PointState::TRACK;
    //Провряем не вышли ли мы из воды
    if(pModel_->getPointState(position_) == PointState::LAND && isOnTheWater_) {
        setDirection(Direction::STOP); // Останавливаем ксоникса
        isOnTheWater_ = false;         // Отмечаем что мы уже не на воде
        pModel_->tryToFillGrabbedArea(); // Пытаемся добавить территории
        // Последняя функция возвращает bool если захват территории успешен
        // Это нам понадобится для таймера
    }
    if(pModel_->getPointState(position_) == PointState::WATER) {
        isOnTheWater_ = true; // Отмечаем, что мы зашли на воду
        pModel_->changePointState(position_, PointState::TRACK); // Отмечаем пройденну точку как след
    }
}

void Xonix::setDirection(Direction dir)
{
    if(dir == direction_ && (static_cast<int>(dir) - static_cast<int>(direction_) == 2))
            return;
    else
        direction_ = dir;
}

Direction Xonix::getDirection() const
{
    return direction_;
}

QPoint Xonix::getPosition() const
{
    return position_;
}

int Xonix::getLivesNumber() const
{
    return livesNumber_;
}

bool Xonix::isOnTheWater() const
{
    return isOnTheWater_;
}

bool Xonix::isSelfCrossing() const
{
    return isSelfCrossing_;
}

void Xonix::decreaseLivesNumber()
{
    if(livesNumber_ > 0) --livesNumber_;
}

void Xonix::increaseLivesNumber()
{
    ++livesNumber_;
}
