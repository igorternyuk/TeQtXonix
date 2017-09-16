
#include "xonixmodel.h"
#include "ball.h"
#include "cube.h"


XonixModel::XonixModel(QObject *parent) :
    QObject(parent)
{
    //Делаем необходимый размер двухмерного вектора, который представляет поле
    field_.resize(FIELD_HEIGHT);
    for(auto &row : field_)
        row.resize(FIELD_WIDTH);
    //cubes_.push_back(new Cube(this, this));
    init();
    //Создаем объект ксоникса
    pXonix_ = new Xonix(this);
    //Создаем шарик
    addBall();
    //Создаем кубик
    addCube();
}

void XonixModel::turnXonix(Direction dir)
{
    pXonix_->setDirection(dir);
}

int XonixModel::getFieldWidth() const
{
    return FIELD_WIDTH;
}

int XonixModel::getFieldHeight() const
{
    return FIELD_HEIGHT;
}

QPoint XonixModel::getXonixPosition() const
{
    return pXonix_->getPosition();
}

int XonixModel::getXonixLivesNumber() const
{
    return pXonix_->getLivesNumber();
}

int XonixModel::getElapsedTime() const
{
    return timeElapsed_;
}

bool XonixModel::isGameStarted() const
{
    return isGameStarted_;
}

bool XonixModel::isGameOver() const
{
    return isGameOver_;
}

bool XonixModel::isGamePaused() const
{
    return isGamePaused_;
}

int XonixModel::getLevel() const
{
    return level_;
}

int XonixModel::getScore() const
{
    return score_;
}

double XonixModel::getPercentage() const
{
    //Подсчет процента захваченой ксониксом территории
    return (initialWaterArea_- currentWaterArea_ ) / static_cast<double>(initialWaterArea_) * 100;
}

QList<QPoint> XonixModel::getBallsPositions() const
{
    QList<QPoint> ballPositions;
    for(auto ball : balls_)
        ballPositions.push_back(ball->getPosition());
    return ballPositions;
}

QList<QPoint> XonixModel::getCubesPositions() const
{
    QList<QPoint> cubesPositions;
    for(auto cube : cubes_)
        cubesPositions.push_back(cube->getPosition());
    return cubesPositions;
}

void XonixModel::changePointState(const QPoint &point, const PointState &state)
{
     field_[point.y()][point.x()] = state;
}

PointState XonixModel::getPointState(const QPoint &point) const
{
    return field_[point.y()][point.x()];
}

bool XonixModel::isXonixOnTheWater() const
{
    return pXonix_->isOnTheWater();
}

void XonixModel::newGame()
{
   if(timer_.isActive())
       timer_.stop();
   isGameStarted_ = false;
   isGamePaused_ = false;
   isGameOver_ = false;
   score_ = 0;
   level_ = 1;
   timeElapsed_ = TIME_FOR_XONIX;
   this->init();
   pXonix_->init();
   balls_.clear();
   cubes_.clear();
   addBall();
   addCube();
   emit updateView();
}

void XonixModel::start()
{
    if(!isGameStarted_)
        isGameStarted_ = true;
    isGamePaused_ = false;
    timer_.start(TIMER_TIMEOUT_TIME, this);
}

void XonixModel::pause()
{
    isGamePaused_ = true;
    timer_.stop();
    emit updateView();
}

void XonixModel::timerEvent(QTimerEvent*)
{
    timeElapsed_ -= TIMER_TIMEOUT_TIME;
    if(isGamePaused_) return;
    //Сначала говорим всем елементам двигаться
    pXonix_->move();
    for(const auto &ball : balls_) {
        ball->move();
    }
    for(const auto &cube : cubes_) {
        cube->move();
    }
    // Проверяем не пересек ли ксоникс свой след
    auto hasXonixCrossedItself = pXonix_->isSelfCrossing();
    // Проверяем столкновения с шариками
    auto hasXonixCollisionsWithBalls = false;
    for(const auto &ball : balls_) {
        //ball->move();
        if(ball->hasCollisionWithXonixOrTrack()) {
            hasXonixCollisionsWithBalls = true;
            break;
        }
    }
    auto hasXonixCollisionsWithCubes = false;
    for(const auto &cube : cubes_) {
        if(cube->hasCollisionWithXonix()) {
            hasXonixCollisionsWithCubes = true;
            break;
        }
    }
    // Проверяем столкновения ксоникса с кубиками
    if(hasXonixCrossedItself || hasXonixCollisionsWithBalls ||
       hasXonixCollisionsWithCubes) {
        pXonix_->decreaseLivesNumber();
        // Если у ксоникса кочились жизни останавливаем таймер
        if(pXonix_->getLivesNumber() > 0) {
            pXonix_->init();
            clearTrack();
        }
        else {
            isGameOver_ = true;
            timer_.stop();
            emit updateView();
            return;
        }
    }
    if(timeElapsed_ <= 0) {
        // Если ксоникс не завоевал 75% территории за 60 секунд
        // То добавляем еще один кубик
        addCube();
        timeElapsed_ = TIME_FOR_XONIX;
    }
    if(getPercentage() >= NEXT_LEVEL_AREA_PERCENTAGE) {
        //Если ксоникс завоевал >75% территории меньше чем за 60 секунд
        //То снова даем ему 60 секунд
        timeElapsed_ = TIME_FOR_XONIX;
        this->init();
        increaseLevel();
        //На кажом уровне кратном 5 ксониксу добавляется дополнительная жизнь
        if(level_ >= LEVEL_FOR_ADDITIONAL_LIFE && level_ % LEVEL_FOR_ADDITIONAL_LIFE == 0)
            pXonix_->increaseLivesNumber();
        pXonix_->init();
        for(auto &cube : cubes_)
            cube->setRandomPosition();
        addBall();
    }
    emit updateView();
}

void XonixModel::init()
{
    // Заполняем поле
    for(auto y = 0; y < FIELD_HEIGHT; ++y) {
        for(auto x = 0; x < FIELD_WIDTH; ++x) {
            bool isOnTheWater = (x >= LAND_STRIP_WIDTH) &&
                    (x <= FIELD_WIDTH - 1 - LAND_STRIP_WIDTH) &&
                    (y >= LAND_STRIP_HEIGHT) &&
                    (y <= FIELD_HEIGHT - 1 - LAND_STRIP_HEIGHT);
            field_[y][x] = (isOnTheWater) ? PointState::WATER : PointState::LAND;
        }
    }
    currentWaterArea_ = initialWaterArea_;
}

void XonixModel::clearTrack()
{
    // Очищает след ксоникса
    for(auto y = 0; y < FIELD_HEIGHT; ++y) {
        for(auto x = 0; x < FIELD_WIDTH; ++x) {
            if(field_[y][x] == PointState::TRACK)
                field_[y][x] = PointState::WATER;
        }
    }
}

void XonixModel::blockBallArea(const QPoint &point)
{
    // Метод рекурсивно блокирует от перекрашивания в сушу часть территории
    // где находятся шарики - наподобие циклического открытия ячеек в сапере
    auto x = point.x();
    auto  y = point.y();
    auto isCurPointOutSide = isPointOutsideOfTheField(point);
    if(isCurPointOutSide || !(field_[y][x] == PointState::WATER))
        return;
    field_[y][x] =  PointState::BLOCKED;
    for(auto dy = -1; dy <= 1; ++dy) {
        for(auto dx = -1; dx <= 1; ++dx) {
            blockBallArea(QPoint(x + dx, y + dy));
        }
    }
}

bool XonixModel::isPointOutsideOfTheField(const QPoint &point) const
{
    return point.x() < 0 || point.x() > FIELD_WIDTH - 1 ||
            point.y() < 0 || point.y() > FIELD_HEIGHT - 1;
}

void XonixModel::addCube()
{
    // Здесь добавляем сухопутного врага ксониксу
    Cube *cube = new Cube(this, this);
    cube->setRandomPosition();
    cubes_.push_back(cube);
}

void XonixModel::addBall()
{
    balls_.push_back(new Ball(this, this));
}

void XonixModel::increaseLevel()
{
    ++level_;
}

bool XonixModel::tryToFillGrabbedArea()
{
    auto newWaterArea = 0;
    // Перебираем все мячики и блокируем их территорию
    for (auto &ball : balls_) blockBallArea(ball->getPosition());
    //Try to fill grabbed area
    for(auto y = 0; y < FIELD_HEIGHT; ++y) {
        for(auto x = 0; x < FIELD_HEIGHT; ++x) {
            if(field_[y][x] == PointState::TRACK ||
                field_[y][x] == PointState::WATER) {
                changePointState(QPoint(x,y), PointState::LAND);
                score_ += SCORE_INCREMENT;
            }
            //Заблокированным клеткам возвращем состояние - "вода"
            // и попутно подсчитываем площать водной территории по клеткам
            if(field_[y][x] ==  PointState::BLOCKED) {
                changePointState(QPoint(x,y), PointState::WATER);
                ++newWaterArea;
            }
        }
    }
    //Возвращает true если захваченой площади прибавилось и false если ничего не захватили
    if(newWaterArea != currentWaterArea_) {
        currentWaterArea_ = newWaterArea;
        return true;
    }
    else
        return false;
}

