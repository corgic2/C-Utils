#include <QApplication>
#include <GetComponentStyleSheet.h>
int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    GetComponentStyleSheet w;
    w.show();
    return a.exec();
}