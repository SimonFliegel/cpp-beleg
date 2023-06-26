#include "medienverwaltung.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Medienverwaltung w;
    QObject::connect(&a , SIGNAL(aboutToQuit()), &w, SLOT(app_aboutToQuit()));
    w.show();
    return a.exec();
}
