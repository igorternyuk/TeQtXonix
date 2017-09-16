#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>

class XonixModel;
class Enemy : public QObject
{
    Q_OBJECT
public:
    explicit Enemy(XonixModel *pModel, QObject *parent = nullptr);
    void move();
    const QPoint getPosition() const;
    const QPoint getVelocity() const;
signals:
protected:
    virtual void init() = 0;
    virtual void updateVelocity() = 0;
    XonixModel *pModel_;
    QPoint position_;
    QPoint velocity_;
};

#endif // ENEMY_H
