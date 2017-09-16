#include "screen.h"
#include <QFont>

Screen::Screen(XonixModel *pModel, XonixController *pController, QWidget *parent) :
    QFrame(parent), pModel_(pModel), pController_(pController)
{
    this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);  // Убрать магические числа
}

void Screen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawGameInfo(painter);
    drawField(painter);
    drawXonix(painter);
    drawBalls(painter);
    drawCubes(painter);
    if(pModel_->isGameOver()) {
        drawGameOver(painter);
        return;
    }
    if(pModel_->isGamePaused())
        drawPause(painter);
}

void Screen::keyPressEvent(QKeyEvent *event)
{
     switch(event->key()) {
        case Qt::Key_Left :
            pController_->onLeftButtonPressed();
            break;
        case Qt::Key_Up :
            pController_->onUpButtonPressed();
            break;
        case Qt::Key_Right :
            pController_->onRightButtonPressed();
            break;
        case Qt::Key_Down :
            pController_->onDownButtonPressed();
            break;
        case Qt::Key_Space :
            pController_->onSpaceButoonPressed();
            break;
        case Qt::Key_N:
            pController_->onNewGameActionTriggered();
            break;
        case Qt::Key_Q:
            pController_->onExitActionTriggered();
            break;
        default:
            QFrame::keyPressEvent(event);
            break;
     }
}

void Screen::drawGameInfo(QPainter &painter)
{
    auto top = this->contentsRect().top();
    auto left = this->contentsRect().left();
    //Сначала рисуем фоновый прямоугольник для цифровой информации
    QRect rect(left, top + pModel_->getFieldHeight() * POINT_SIZE,
               INFO_TABLE_WIDTH, INFO_TABLE_HEIGHT);
    painter.fillRect(rect, WATER_COLOR);
    //Теперь выводим информацию
    //Формируем строку
    QString text = "Level:" + QString::number(pModel_->getLevel()) +
                   " Area:" + QString::number(pModel_->getPercentage(),'f',1) + "%" +
                   " Score:" + QString::number(pModel_->getScore()) +
                   " Lives:" + QString::number(pModel_->getXonixLivesNumber()) +
                   " Time " + QString::number(pModel_->getElapsedTime() / 1000) + "s";
    //Устанавливаем кисть
    painter.setBrush(QBrush(Qt::black));
    //Устанавливаем жирный шрифт
    QFont font("sans", 13);
    font.setBold(true);
    painter.setFont(font);
    //Определяем координату нижнего левого угла текста
    QPoint point(left + POINT_SIZE, top + 3 * POINT_SIZE +
                 pModel_->getFieldHeight() * POINT_SIZE);
    //Рисуем текст
    painter.drawText(point, text);
}

void Screen::drawField(QPainter &painter)
{
    auto top = this->contentsRect().top();
    auto left = this->contentsRect().left();
    for(auto y = 0; y < int(pModel_->getFieldHeight()); ++y)
        for(auto x = 0; x < int(pModel_->getFieldWidth()); ++x) {
            QRect rect(left + x * POINT_SIZE, top + y * POINT_SIZE, POINT_SIZE,POINT_SIZE);
            PointState state = pModel_->getPointState(QPoint(x,y));
            //Раскрашиваем поле в соответствии с состоянием клетки
            switch (state) {
                case PointState::LAND :
                    painter.fillRect(rect, QBrush(LAND_COLOR));
                    break;
                case PointState::WATER :
                    painter.fillRect(rect, QBrush(WATER_COLOR));
                    break;
                case PointState::TRACK:
                    painter.fillRect(rect, QBrush(TRACK_COLOR));
                    break;
                default:
                    break;
            }
        }
}

void Screen::drawBalls(QPainter &painter)
{
    auto top = this->contentsRect().top();
    auto left = this->contentsRect().left();
    auto balls = pModel_->getBallsPositions();
    for(const auto &ball : balls) {
        painter.setBrush(BALL_COLOR);
        QRect rect(left + ball.x() * POINT_SIZE,top + ball.y() * POINT_SIZE,
                   POINT_SIZE, POINT_SIZE);
        painter.drawEllipse(rect);
        painter.setBrush(QBrush(Qt::yellow));
        auto innerRadius = 3;
        painter.drawEllipse(QPoint(left + ball.x() * POINT_SIZE + POINT_SIZE / 2,
                            top + ball.y() * POINT_SIZE + POINT_SIZE / 2),
                            innerRadius, innerRadius);
    }
}

void Screen::drawCubes(QPainter &painter)
{
    auto cubes = pModel_->getCubesPositions();
    for(const auto &cube : cubes) {
        QRect outerRect(contentsRect().left() + cube.x() * POINT_SIZE,
                        contentsRect().top() + cube.y() * POINT_SIZE,
                        POINT_SIZE,POINT_SIZE);
        painter.setBrush(CUBE_COLOR);
        painter.drawRect(outerRect);
        const auto innerRectSide = 6;
        QRect innerRect(contentsRect().left() + cube.x() * POINT_SIZE + 2,
                        contentsRect().top() + cube.y() * POINT_SIZE + 2,
                        innerRectSide, innerRectSide);
        painter.setBrush(Qt::white);
        painter.drawRect(innerRect);
    }

}

void Screen::drawXonix(QPainter &painter)
{
    auto top = this->contentsRect().top();
    auto left = this->contentsRect().left();
    QRect outerRect(left + pModel_->getXonixPosition().x() * POINT_SIZE,
                    top + pModel_->getXonixPosition().y() * POINT_SIZE,
                    POINT_SIZE,POINT_SIZE);
    const auto innerRectSide = 6;
    QRect innerRect(left + pModel_->getXonixPosition().x() * POINT_SIZE + 2,
                    top + pModel_->getXonixPosition().y() * POINT_SIZE + 2,
                    innerRectSide, innerRectSide);

    if(pModel_->isXonixOnTheWater()) {
        painter.setBrush(XONIX_INTERNAL_COLOR);
        painter.drawRect(outerRect);
        painter.setBrush(XONIX_EXTERNAL_COLOR);
        painter.drawRect(innerRect);
    }
    else {
        painter.setBrush(XONIX_EXTERNAL_COLOR);
        painter.drawRect(outerRect);
        painter.setBrush(XONIX_INTERNAL_COLOR);
        painter.drawRect(innerRect);
    }
}

void Screen::drawGameOver(QPainter &painter)
{
    QPoint point(contentsRect().left() + TEXT_LEFT,
                contentsRect().top() + TEXT_TOP);
    drawText(painter, point, GAME_OVER_TEXT, TEXT_FONT_SIZE, Qt::black);
}

void Screen::drawPause(QPainter &painter)
{
    QPoint point(contentsRect().left() + TEXT_LEFT,
                contentsRect().top() + TEXT_TOP);
    drawText(painter, point, PAUSE_TEXT, TEXT_FONT_SIZE, Qt::darkGreen);
}

void Screen::drawText(QPainter &painter, const QPoint &leftBottom,
                      const QString &text, const int &fontSize, const QColor &color)
{
    painter.setPen(color);
    QFont font(TEXT_FONT_FAMILY, fontSize);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(leftBottom, text);
}
