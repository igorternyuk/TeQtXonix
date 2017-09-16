#ifndef SCREEN_H
#define SCREEN_H

#include <QFrame>
#include <QKeyEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QString>

#include "xonixmodel.h"
#include "xonixcontroller.h"

class Screen : public QFrame
{
    Q_OBJECT
public:
    explicit Screen(XonixModel *pModel, XonixController *pController,
                    QWidget *parent = nullptr);
public slots:

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    const int POINT_SIZE = 10;
    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 560;
    const QColor LAND_COLOR = QColor(226,177,26);
    //const QColor LAND_COLOR = Qt::yellow;
    const QColor WATER_COLOR = QColor(17,117,185);
    //const QColor WATER_COLOR = Qt::cyan;
    const QColor TRACK_COLOR = Qt::red;
    const QColor CUBE_COLOR = Qt::red;
    const QColor BALL_COLOR = Qt::red;
    const QColor XONIX_INTERNAL_COLOR = Qt::black;
    const QColor XONIX_EXTERNAL_COLOR = Qt::white;
    const int TEXT_LEFT = 40;
    const int TEXT_TOP = 250;
    const int INFO_TABLE_WIDTH = 540;
    const int INFO_TABLE_HEIGHT = 50;
    const int TEXT_FONT_SIZE = 40;
    const QString TEXT_FONT_FAMILY = "sans";
    const QString GAME_OVER_TEXT = "GAME OVER!!!";
    const QString PAUSE_TEXT = "GAME PAUSED";
    XonixModel *pModel_;
    XonixController *pController_;
    void drawGameInfo(QPainter &painter);
    void drawField(QPainter &painter);
    void drawBalls(QPainter &painter);
    void drawCubes(QPainter &painter);
    void drawXonix(QPainter &painter);
    void drawGameOver(QPainter &painter);
    void drawPause(QPainter &painter);
    void drawText(QPainter &painter, const QPoint &leftBottom,
                  const QString &text, const int &fontSize, const QColor &color);
};

#endif // SCREEN_H
