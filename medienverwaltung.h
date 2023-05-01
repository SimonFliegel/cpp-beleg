#ifndef MEDIENVERWALTUNG_H
#define MEDIENVERWALTUNG_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Medienverwaltung; }
QT_END_NAMESPACE

class Medienverwaltung : public QMainWindow
{
    Q_OBJECT

public:
    Medienverwaltung(QWidget *parent = nullptr);
    ~Medienverwaltung();

private:
    Ui::Medienverwaltung *ui;
};
#endif // MEDIENVERWALTUNG_H
