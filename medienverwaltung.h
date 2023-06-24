#ifndef MEDIENVERWALTUNG_H
#define MEDIENVERWALTUNG_H

#include <QMainWindow>
#include <QTableWidget>
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

    void saveData();
    void loadData();

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
    void btn_addMedia_Clicked();
    void btn_registerUser_Clicked();
    void btn_deleteMedia_Clicked();
    void btn_deleteUser_Clicked();
    void btn_lendMedia_Clicked();
    void btn_returnMedia_Clicked();
    void btn_returnMedia_user_Clicked();
    void db_media_columnHeader_Clicked(int colIndex);
    void db_user_columnHeader_Clicked(int colIndex);
    void db_media_ItemChanged(QTableWidgetItem* item);
    void db_user_ItemChanged(QTableWidgetItem* item);
    void cB_mediaType_SelectionChanged();
    void cB_returnMedia_user_SelectionChanged();
    void db_user_SelectionChanged();
    void tab_view_CurrentChanged(int index);
};
#endif // MEDIENVERWALTUNG_H
