#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include "enemy.h"

class Cube : public Enemy
{
    Q_OBJECT
public:
    explicit Cube(XonixModel *pModel, QObject *parent = nullptr);
    bool hasCollisionWithXonix() const;
    void setRandomPosition();
protected:
    void init() override;
    void updateVelocity() override;
};

#endif // CUBE_H
