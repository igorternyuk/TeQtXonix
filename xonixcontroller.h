#ifndef XONIXCONTROLLER_H
#define XONIXCONTROLLER_H

#include <QObject>

class XonixModel;
class XonixView;

class XonixController : public QObject
{
    Q_OBJECT
public:
    explicit XonixController(XonixModel *pModel, XonixView *pView,
             QObject *parent = nullptr);
public slots:
    void onLeftButtonPressed();
    void onUpButtonPressed();
    void onRightButtonPressed();
    void onDownButtonPressed();
    void onSpaceButoonPressed();
    void onStartActionTriggered();
    void onPauseActionTriggred();
    void onNewGameActionTriggered();
    void onExitActionTriggered();
    void onInfoActionTriggered();
private:
    XonixModel *pModel_;
    XonixView *pView_;
};

#endif // XONIXCONTROLLER_H
