#include "xonixview.h"

XonixView::XonixView(QWidget *parent) : QMainWindow(parent)
{
    pModel_ = new XonixModel(this);
    pController_ = new XonixController(pModel_, this, this);
    pScreen_ = new Screen(pModel_, pController_, this);
    QMenu *actionMenu = this->menuBar()->addMenu("&Actions");
    QAction *newGameAction = actionMenu->addAction("&NewGame");
    QAction *startGameAction = actionMenu->addAction("&Start");
    QAction *pauseGameAction = actionMenu->addAction("&Pause");
    QAction *exitGameAction = actionMenu->addAction("&Exit");
    QMenu *infoMenu = this->menuBar()->addMenu("&About");
    QAction *infoAction = infoMenu->addAction("&Info");
    connect(exitGameAction, &QAction::triggered, [&](){close();});
    connect(infoAction, &QAction::triggered, [&](){showInfoAboutGame();});
    connect(newGameAction, SIGNAL(triggered(bool)), pController_, SLOT(onNewGameActionTriggered()));
    connect(startGameAction, SIGNAL(triggered(bool)), pController_, SLOT(onStartActionTriggered()));
    connect(pauseGameAction, SIGNAL(triggered(bool)), pController_, SLOT(onPauseActionTriggred()));
    connect(pModel_, SIGNAL(updateView()), this, SLOT(update()));
    this->setWindowTitle(WINDOW_TITLE);
    this->setContentsMargins(WINDOW_CONTENT_MARGIN,0,WINDOW_CONTENT_MARGIN,WINDOW_CONTENT_MARGIN);
    this->setCentralWidget(pScreen_);
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void XonixView::update()
{
    pScreen_->update();
}

void XonixView::showInfoAboutGame()
{
    QMessageBox msg;
    msg.setText(INFO_ABOUT_XONIX);
    msg.exec();
}
