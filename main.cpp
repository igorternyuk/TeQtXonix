#include <QApplication>
#include <QDesktopWidget>

#include "xonixview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    XonixView view;
    QRect myScreenGeometry = QApplication::desktop()->geometry() ;
    int dx = (myScreenGeometry.width() - view.width()) / 2;
    int dy = (myScreenGeometry.height() - view.height()) / 2;
    view.move(dx, dy);
    view.show();

    return a.exec();
}
