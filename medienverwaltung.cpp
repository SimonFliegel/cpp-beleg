#include "medienverwaltung.h"
#include "ui_medienverwaltung.h"

Medienverwaltung::Medienverwaltung(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Medienverwaltung)
{
    ui->setupUi(this);
}

Medienverwaltung::~Medienverwaltung()
{
    delete ui;
}

