#include "xonixcontroller.h"
#include "xonixmodel.h"
#include "xonixview.h"

XonixController::XonixController(XonixModel *pModel, XonixView *pView, QObject *parent) :
    QObject(parent), pModel_(pModel), pView_(pView)
{}

void XonixController::onLeftButtonPressed()
{
    pModel_->turnXonix(Direction::LEFT);
}

void XonixController::onUpButtonPressed()
{
    pModel_->turnXonix(Direction::UP);
}

void XonixController::onRightButtonPressed()
{
    pModel_->turnXonix(Direction::RIGHT);
}

void XonixController::onDownButtonPressed()
{
    pModel_->turnXonix(Direction::DOWN);
}

void XonixController::onSpaceButoonPressed()
{
    if(!pModel_->isGameStarted()) {
        pModel_->start();
        return;
    }
    if(pModel_->isGamePaused() && !pModel_->isGameOver())
        pModel_->start();
    else
        pModel_->pause();
}

void XonixController::onStartActionTriggered()
{
    pModel_->start();
}

void XonixController::onPauseActionTriggred()
{
    pModel_->pause();
}

void XonixController::onNewGameActionTriggered()
{
    pModel_->newGame();
}

void XonixController::onExitActionTriggered()
{
    pView_->close();
}

void XonixController::onInfoActionTriggered()
{

}
