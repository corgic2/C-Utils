#include <QApplication>

#include "CursorAutoWidget.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    //  直接退出
    CursorAutoWidget w;

    // w.setWindowFlags(Qt::WindowStaysOnTopHint);
    w.show();
    return a.exec();
}