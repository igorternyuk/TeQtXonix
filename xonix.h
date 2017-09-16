#ifndef XONIX_H
#define XONIX_H

#include <QObject>
#include <QPoint>
#include <QList>

class XonixModel;

enum class Direction {
    STOP = -10, LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3
};

class Xonix : public QObject
{
    Q_OBJECT
public:
    explicit Xonix(XonixModel *pModel, QObject *parent = nullptr);
    void init();
    void move();
    void setDirection(Direction dir);
    Direction getDirection() const;
    QPoint getPosition() const;
    int getLivesNumber() const;
    bool isOnTheWater() const;
    bool isSelfCrossing() const;
    void decreaseLivesNumber();
    void increaseLivesNumber();
private:
    const int INITIAL_LIVES_NUMBER = 3;
    XonixModel *pModel_;
    QPoint position_;     // Положение ксоникса
    Direction direction_; // Направление ксоникса
    bool isOnTheWater_;
    bool isSelfCrossing_;
    int livesNumber_ = INITIAL_LIVES_NUMBER;
};

#endif // XONIX_H
