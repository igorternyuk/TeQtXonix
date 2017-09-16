#ifndef XONIXMODEL_H
#define XONIXMODEL_H

#include <QObject>
#include <QBasicTimer>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QDebug>

#include "xonix.h"

class Ball;
class Cube;

enum class PointState {
    LAND, WATER, TRACK, BLOCKED
};

using Matrix = QVector< QVector< PointState> >;

class XonixModel : public QObject
{
    Q_OBJECT
public:
    explicit XonixModel(QObject *parent = nullptr);
    int getFieldWidth() const;
    int getFieldHeight() const;
    QPoint getXonixPosition() const;
    QList<QPoint> getBallsPositions() const;
    QList<QPoint> getCubesPositions() const;
    int getLevel() const;
    int getScore() const;
    int getXonixLivesNumber() const;
    int getElapsedTime() const;
    double getPercentage() const;
    bool isPointOutsideOfTheField(const QPoint &point) const;
    PointState getPointState(const QPoint &point) const;
    bool isXonixOnTheWater() const;
    bool isGameStarted() const;
    bool isGamePaused() const;
    bool isGameOver() const;
    void changePointState(const QPoint &point, const PointState &state);

signals:
    void updateView();
public slots:
    void newGame();
    void start();
    void pause();
    void turnXonix(Direction dir);
    void init();
    bool tryToFillGrabbedArea();
protected:
    void timerEvent(QTimerEvent*) override;
private:
    const int LAND_STRIP_WIDTH = 4;
    const int LAND_STRIP_HEIGHT = 4;
    const int FIELD_WIDTH = 50;
    const int FIELD_HEIGHT = 50;
    const int TIMER_TIMEOUT_TIME = 100;
    const int SCORE_INCREMENT = 5;
    const int NEXT_LEVEL_AREA_PERCENTAGE = 75;
    const int TIME_FOR_XONIX = 60000;
    const int LEVEL_FOR_ADDITIONAL_LIFE = 5;
    Xonix *pXonix_;
    QList<Ball*> balls_;
    QList<Cube*> cubes_;
    QBasicTimer timer_;
    Matrix field_;
    int initialWaterArea_ = (FIELD_WIDTH - 2 * LAND_STRIP_WIDTH) *
                            (FIELD_HEIGHT - 2 * LAND_STRIP_HEIGHT);
    int currentWaterArea_ = initialWaterArea_;
    int level_ = 1;
    int score_ = 0;
    bool isGameStarted_ = false;
    bool isGamePaused_ = false;
    bool isGameOver_ = false;
    int timeElapsed_ = TIME_FOR_XONIX;
    void increaseLevel();
    void clearTrack();
    void blockBallArea(const QPoint &point);
private slots:
    void addCube();
    void addBall();
};

#endif // XONIXMODEL_H
