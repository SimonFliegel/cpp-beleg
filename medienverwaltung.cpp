#include "medienverwaltung.h"
#include "ui_medienverwaltung.h"

#include <QtDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>

#include "mediaTypes.h"
#include "book.h"
#include "cd.h"
#include "dvd.h"

Medienverwaltung::Medienverwaltung(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Medienverwaltung)
{
    ui->setupUi(this);
    initUi();
}

Medienverwaltung::~Medienverwaltung()
{
    delete ui;
}

void Medienverwaltung::initUi() {
    initMediaTable();
    initUserTable();
    initGBaddMedia();
    ui->tab_view->setCurrentIndex(0);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(actionSave_Triggered()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(actionLoad_Triggered()));
    connect(ui->btn_addMedia, SIGNAL(released()), this, SLOT(btn_addMedia_Clicked()));
    connect(ui->btn_registerUser, SIGNAL(released()), this, SLOT(btn_registerUser_Clicked()));
    connect(ui->btn_lendMedia, SIGNAL(released()), this, SLOT(btn_lendMedia_Clicked()));
    connect(ui->btn_returnMedia, SIGNAL(released()), this, SLOT(btn_returnMedia_Clicked()));
    connect(ui->btn_returnMedia_user, SIGNAL(released()), this, SLOT(btn_returnMedia_user_Clicked()));
    connect(ui->cB_returnMedia_user, SIGNAL(currentIndexChanged(int)), this, SLOT(cB_returnMedia_user_SelectionChanged()));
    connect(ui->db_user, SIGNAL(itemSelectionChanged()), this, SLOT(db_user_SelectionChanged()));
    connect(ui->tab_view, SIGNAL(currentChanged(int)) ,this, SLOT(tab_view_CurrentChanged()));
}

void Medienverwaltung::initMediaTable()
{
    QTableWidget* table = ui->db_media;
    QStringList headers;
    headers << "ID" << "Typ" << "Titel" << "Vorname" << "Nachname";
    table->setColumnCount(5); // type, title, user-surname, user-name
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 300);
    table->setColumnWidth(3, 200);
    table->setColumnWidth(4, 200);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Medienverwaltung::initUserTable() {
    QTableWidget* table = ui->db_user;
    QStringList headers;
    headers << "ID" << "Vorname" << "Nachname";
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 400);
    table->setColumnWidth(2, 400);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Medienverwaltung::initGBaddMedia() {
    QStringList items;
    items << "Buch" << "CD" << "DVD";
    ui->cB_addMedia_type->addItems(items);
    ui->cB_addMedia_type->setCurrentIndex(0);
}

void Medienverwaltung::initMessageBoxInformation(QString info) {
    QMessageBox* mb = new QMessageBox();
    mb->setInformativeText(info);
    mb->setStandardButtons(QMessageBox::Ok);
    mb->setDefaultButton(QMessageBox::Ok);
    mb->exec();
}

int Medienverwaltung::initMessageBoxQuestion(QString question, QString descr) {
    QMessageBox* mb = new QMessageBox();
    mb->setInformativeText(question);
    mb->setText(descr);
    mb->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb->setDefaultButton(QMessageBox::No);
    return mb->exec();
}

void Medienverwaltung::saveData() {
//    int response;
//    int mediaCountFromFile = admin->getMediaCountFromFile();
//    int userCountFromFile = admin->getUserCountFromFile();
//    QString descr;

//    if (mediaCountFromFile > 0 && userCountFromFile > 0) {

//        descr = "Folgende Änderungen werden vorgenommen:<br>" +
//                QString::number(mediaCountFromFile) + " Medien und " +
//                QString::number(userCountFromFile) + " werden durch<br>" +
//                QString::number(admin->getMediaCount()) + " Medien und " +
//                QString::number(admin->getUserCount()) + " Nutzer überschrieben.";

//                response = initMessageBoxQuestion("Soll der aktuelle Stand wirklich gespeichert werden?", descr);
//        if (response == QMessageBox::No) {
//            return;
//        }
//    }
    admin->saveMedia();
    admin->saveUsers();

//    initMessageBoxInformation("Alle Daten wurden gespeichert.");
}

void Medienverwaltung::loadData() {
    if (admin->getMediaCountFromFile() == -1 && admin->getUserCountFromFile() == -1) {
        initMessageBoxInformation("Es existiert noch kein Speicherstand aus dem Daten geladen werden können.");
        return;
    }
    if (!admin->loadMedia()) {
        initMessageBoxInformation("Beim Laden der Medien ist ein Fehler aufgetreten.");
        return;
    }
    if (!admin->loadUsers()) {
        initMessageBoxInformation("Beim Laden der Nutzer ist ein Fehler aufgetreten.");
        return;
    }
    updateMediaUI();
    updateUserUI();
}

void Medienverwaltung::updateMediaUI() {
    updateMediaTable();
    updateLendingSelection();
}

void Medienverwaltung::updateUserUI() {
    updateUserTable();
    updateAllUsersSelection();
}

void Medienverwaltung::updateMediaTable() {
    QTableWidget* table = ui->db_media;
    QMap<int,Media*>& mediaList = admin->getMediaList();
    QList<int> mediaKeys = mediaList.keys();
    User* user;
    QComboBox* cB_type;
    QTableWidgetItem *id, *title, *surname, *name;
    QStringList cB_items;
    cB_items << "Buch" << "CD" << "DVD";
    table->clearSelection(); // to not cause selecito
    table->clearContents();
    table->setRowCount(mediaList.size());

    for (int i = 0; i < mediaList.size(); i++) {
        id = new QTableWidgetItem();
        id->setData(Qt::UserRole, mediaKeys.at(i));
        cB_type = new QComboBox();
        cB_type->addItems(cB_items);
        cB_type->setCurrentIndex(mediaList[i]->getType());
        title = new QTableWidgetItem(mediaList[i]->getTitle());
        if (mediaList[i]->getUserId() != -1) { // if media is lent...
            user = admin->getUser(mediaList[i]->getUserId());
            surname = new QTableWidgetItem(user->getSurname());
            name = new QTableWidgetItem(user->getName());
        } else {
            surname = new QTableWidgetItem();
            name = new QTableWidgetItem();
        }
        surname->setFlags(surname->flags() & ~Qt::ItemIsEditable);
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);

        table->setItem(i, 0, id); // hidden
        table->setCellWidget(i, 1, cB_type);
        table->setItem(i, 2, title);
        table->setItem(i, 3, surname);
        table->setItem(i, 4, name);
    }

}

void Medienverwaltung::updateUserTable() {
    QTableWidget* table = ui->db_user;
    QMap<int,User*>& userList = admin->getUserList();
    QList<int> userKeys = userList.keys();
    QTableWidgetItem *id, *surname, *name;
    table->clearSelection();
    table->clearContents();
    table->setRowCount(userList.size());

    for (int i = 0; i < userList.size(); i++) {
        id = new QTableWidgetItem();
        id->setData(Qt::UserRole, userKeys.at(i));
        surname = new QTableWidgetItem(userList[i]->getSurname());
        name = new QTableWidgetItem(userList[i]->getName());

        table->setItem(i, 0, id); // hidden
        table->setItem(i, 1, surname);
        table->setItem(i, 2, name);
    }
}

QString buildMediaItemString(Media* media) {
    QString mediaItemString;
    mediaItemString = QString::fromUtf8(typeNames[media->getType()]) + ", " + media->getTitle();
    return mediaItemString;
}

QString buildUserItemString(User* user) {
    QString userItemString;
    userItemString = user->getSurname() + " " + user->getName();
    return userItemString;
}

void Medienverwaltung::updateAllUsersSelection() {
    QComboBox* cB_user = ui->cB_lendMedia_user;
    QMap<int,User*>& userList = admin->getUserList();
    QList<int> userKeys = userList.keys();

    cB_user->clear();

    for (int i = 0; i < userList.size(); i++) {
        cB_user->addItem(buildUserItemString(userList[i]), userKeys.at(i));
    }
}

void Medienverwaltung::updateLendingSelection() {
    QComboBox* cB_media = ui->cB_lendMedia_media;
    QMap<int,Media*>& mediaList = admin->getMediaList();
    QList<int> mediaKeys = mediaList.keys();
    QComboBox* cB_user = ui->cB_returnMedia_user;
    QMap<int,User*>& userList = admin->getUserList();
    QList<int> userKeys = userList.keys();

    cB_media->clear();

    // available media
    for (int i = 0; i < mediaList.size(); i++) {
        if (mediaList[i]->getUserId() == -1) {
            cB_media->addItem(buildMediaItemString(mediaList[i]), mediaKeys.at(i)); // label + id
        }
    }

    cB_user->clear();

    // lending users
    for (int i = 0; i < userList.size(); i++) {
        if (!userList[i]->getLentMedia().isEmpty()) {
            cB_user->addItem(buildUserItemString(userList[i]), userKeys.at(i));
        }
    }
}

void Medienverwaltung::updateLentMediaList() {
    if (ui->db_user->selectedItems().isEmpty()) {
        return;
    }
    int selectedRow = ui->db_user->selectedItems().constFirst()->row(); // only works because of single selection
    QTableWidgetItem* idItem = ui->db_user->item(selectedRow, 0);
    QListWidget* list = ui->li_lentMedia;
    QListWidgetItem* item;
    int userId = idItem->data(Qt::UserRole).toInt();
    User* user = admin->getUser(userId);

    list->clear();

    for (int id : user->getLentMedia()) {
        item = new QListWidgetItem(buildMediaItemString(admin->getMedia(id)));
        item->setData(Qt::UserRole, id);
        list->addItem(item);
    }
}

// automatically called whenever lending user selection changes
void Medienverwaltung::updateReturnableMediaSelection(int userId) {
    QComboBox* cB_media = ui->cB_returnMedia_media;
    QMap<int,Media*>& mediaList = admin->getMediaList();
    Media* lentMedia;
    User* user = admin->getUser(userId);
    QList<int>& lentMediaIds = user->getLentMedia();

    cB_media->clear();

    for (int id : lentMediaIds) {
        lentMedia = mediaList.value(id);
        cB_media->addItem(buildMediaItemString(lentMedia), id);
    }
}


/********************************** private slots ******************************************/

void Medienverwaltung::actionSave_Triggered() {
    saveData();
}

void Medienverwaltung::actionLoad_Triggered() {

}

void Medienverwaltung::btn_addMedia_Clicked() {
    MediaType type = (MediaType)ui->cB_addMedia_type->currentIndex();
    QString title = ui->lE_addMedia_title->text();
    QString creator = ui->lE_addMedia_creator->text();
    Media* newMedia;

    switch (type) {
    case BOOK:
        newMedia = new Book(title, creator);
        break;
    case CD:
        newMedia = new Cd(title, creator);
        break;
    case DVD:
        newMedia = new Dvd(title, creator);
        break;
    default:
        newMedia = new Media(title); // CUSTOM
        break;
    }

    admin->addMedia(newMedia);
    updateMediaUI();
}

void Medienverwaltung::btn_registerUser_Clicked() {
    QString surname = ui->lE_registerUser_surname->text();
    QString name = ui->lE_registerUser_name->text();
    User* newUser = new User(name, surname);
    admin->registerUser(newUser);
    updateUserUI();
    ui->lE_registerUser_surname->clear();
    ui->lE_registerUser_name->clear();
}

void Medienverwaltung::btn_lendMedia_Clicked() {
    int mediaId = ui->cB_lendMedia_media->currentData().toInt();
    int userId = ui->cB_lendMedia_user->currentData().toInt();
    admin->lendMedia(mediaId, userId);
    updateMediaUI();
}

void Medienverwaltung::btn_returnMedia_Clicked() {
    int mediaId = ui->cB_returnMedia_media->currentData().toInt();
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    if (!admin->returnMedia(mediaId, userId)) {
        qDebug() << "returning media failed";
    }
    updateMediaUI();
}

void Medienverwaltung::btn_returnMedia_user_Clicked() {
    int selectedRow = ui->db_user->selectedItems().constFirst()->row();
    QTableWidgetItem* idItem = ui->db_user->item(selectedRow, 0);
    int userId = idItem->data(Qt::UserRole).toInt();
    QList<QListWidgetItem*> selectedItems = ui->li_lentMedia->selectedItems();
    int selectedMediaId;

    for (QListWidgetItem* item : selectedItems) {
        selectedMediaId = item->data(Qt::UserRole).toInt();
        if (!admin->returnMedia(selectedMediaId, userId)) {
            qDebug() << "returning media failed with: " << item->text();
        }
    }
    updateMediaUI();
    updateLentMediaList();
}

void Medienverwaltung::cB_returnMedia_user_SelectionChanged() {
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    updateReturnableMediaSelection(userId);
}

void Medienverwaltung::db_user_SelectionChanged() {
    updateLentMediaList();
}

void Medienverwaltung::tab_view_CurrentChanged() {
    if (ui->tab_view->currentIndex() == 0) { // media
        ui->db_media->clearSelection();
    } else {                                 // user
        ui->db_user->clearSelection();
        ui->li_lentMedia->clear();
    }
}





