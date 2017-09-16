#ifndef BALL_H
#define BALL_H

#include <QObject>
#include "enemy.h"

class XonixModel;
class Ball : public Enemy
{
    Q_OBJECT
public:
    explicit Ball(XonixModel *pModel, QObject *parent = nullptr);
    bool hasCollisionWithXonixOrTrack() const;
protected:
    void init() override;
    void updateVelocity() override;
};

#endif // BALL_H
