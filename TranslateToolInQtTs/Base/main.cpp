#include "wtf.h"
#include <QtWidgets/QApplication>
#include "QExcel.h"
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    wtf w;
    w.show();
    return a.exec();
}
