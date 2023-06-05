#ifndef MEDIENVERWALTUNG_H
#define MEDIENVERWALTUNG_H

#include <QMainWindow>
#include "administration.h"

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
    Administration* admin = new Administration();

    void initUi();
    void initMediaTable();
    void initUserTable();
    void initGBaddMedia();
    void initMessageBoxInformation(QString msg);
    int initMessageBoxQuestion(QString question, QString descr = "");

    void saveData();
    void loadData();
    void updateMediaUI();
    void updateUserUI();
    void updateMediaTable();
    void updateUserTable();
    void updateAllUsersSelection();
    void updateLendingSelection();
    void updateLentMediaList();
    void updateReturnableMediaSelection(int userId);

private slots:
    void actionSave_Triggered();
    void actionLoad_Triggered();
    void btn_addMedia_Clicked();
    void btn_registerUser_Clicked();
    void btn_lendMedia_Clicked();
    void btn_returnMedia_Clicked();
    void btn_returnMedia_user_Clicked();
    void cB_returnMedia_user_SelectionChanged();
    void db_user_SelectionChanged();
    void tab_view_CurrentChanged();
};
#endif // MEDIENVERWALTUNG_H
