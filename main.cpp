#include "medienverwaltung.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Medienverwaltung w;
    w.show();
    return a.exec();
}
