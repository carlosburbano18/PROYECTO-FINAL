#include "panel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Inventario w;
    w.show();
    return a.exec();
}
