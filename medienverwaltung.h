#ifndef MEDIENVERWALTUNG_H
#define MEDIENVERWALTUNG_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateEdit>
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
    void initGBAddMedia();
    void initGBSearch();
    void initDEBirthdate(QDateEdit* de);

    bool isValidEmail(QString email);

    void fillMediaTable(QMap<int,Media*> mediaData);
    void fillUserTable(QMap<int,User*> userData);

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
    void actionDelete_Triggered();
    void btn_addMedia_Clicked();
    void btn_registerUser_Clicked();
    void btn_deleteMedia_Clicked();
    void btn_deleteUser_Clicked();
    void btn_lendMedia_Clicked();
    void btn_returnMedia_Clicked();
    void btn_returnMedia_user_Clicked();
    void btn_searchMedia_Clicked();
    void btn_searchUser_Clicked();
    void btn_resetSearchMedia_Clicked();
    void btn_resetSearchUser_Clicked();
    void db_media_ItemChanged(QTableWidgetItem* item);
    void db_user_ItemChanged(QTableWidgetItem* item);
    void cB_mediaType_SelectionChanged();
    void dE_birthdate_DateChanged();
    void cB_returnMedia_user_SelectionChanged();
    void li_lentMedia_SelectionChanged();
    void db_user_SelectionChanged();
    void tab_view_CurrentChanged(int index);

public slots:
    void app_aboutToQuit();
};
#endif // MEDIENVERWALTUNG_H
