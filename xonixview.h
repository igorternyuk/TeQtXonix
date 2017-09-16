#ifndef XONIXVIEW_H
#define XONIXVIEW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "screen.h"
#include "xonixmodel.h"
#include "xonixcontroller.h"

class XonixView : public QMainWindow
{
    Q_OBJECT
public:
    explicit XonixView(QWidget *parent = nullptr);
public slots:
    void update();
    void showInfoAboutGame();
private:
    const QString WINDOW_TITLE = "XONIX";
    const QString INFO_ABOUT_XONIX = "Info about xonix";
    const int WINDOW_WIDTH = 510;
    const int WINDOW_HEIGHT = 575;
    const int WINDOW_CONTENT_MARGIN = 5;
    XonixModel *pModel_;
    XonixController *pController_;
    Screen *pScreen_;
};

#endif // XONIXVIEW_H
