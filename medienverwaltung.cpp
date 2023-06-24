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
    connect(ui->db_media->horizontalHeader(), &QHeaderView::sectionClicked, this, &Medienverwaltung::db_media_columnHeader_Clicked);
    connect(ui->db_media, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(db_media_ItemChanged(QTableWidgetItem*)));
    connect(ui->db_user, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(db_user_ItemChanged(QTableWidgetItem*)));
    connect(ui->cB_returnMedia_user, SIGNAL(currentIndexChanged(int)), this, SLOT(cB_returnMedia_user_SelectionChanged()));
    connect(ui->db_user, SIGNAL(itemSelectionChanged()), this, SLOT(db_user_SelectionChanged()));
    connect(ui->tab_view, SIGNAL(currentChanged(int)) ,this, SLOT(tab_view_CurrentChanged(int)));
}

void Medienverwaltung::initMediaTable()
{
    QTableWidget* table = ui->db_media;
    QStringList headers;
    headers << "ID" << "Typ" << "Titel" << "Vorname" << "Nachname" << "Löschen";
    table->setColumnCount(6); // type, title, user-surname, user-name
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 100);
    table->setColumnWidth(2, 300);
    table->setColumnWidth(3, 175);
    table->setColumnWidth(4, 175);
    table->setColumnWidth(5, 50);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Medienverwaltung::initUserTable() {
    QTableWidget* table = ui->db_user;
    QStringList headers;
    headers << "ID" << "Vorname" << "Nachname";
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 375);
    table->setColumnWidth(2, 375);
    table->setColumnWidth(3, 50);
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

void Medienverwaltung::saveData() {
    int response;
    int mediaCountFromFile = admin->getMediaCountFromFile();
    int userCountFromFile = admin->getUserCountFromFile();
    QString descr;

    if (mediaCountFromFile > 0 && userCountFromFile > 0) {

        descr = "Folgende Änderungen werden vorgenommen:<br><br>" +
                QString::number(mediaCountFromFile) + " Medien und " +
                QString::number(userCountFromFile) + " Nutzer werden durch<br>" +
                QString::number(admin->getMediaCount()) + " Medien und " +
                QString::number(admin->getUserCount()) + " Nutzer überschrieben.";

        response = QMessageBox::question(this, "Speichern", descr);
        if (response == QMessageBox::No) {
            return;
        }
    }
    if (!admin->saveMedia()) {
        QMessageBox::critical(this, "Fehler", "Beim Speichern der Medien ist ein Fehler aufgetreten!");
    }
    if (!admin->saveUsers()) {
        QMessageBox::critical(this, "Fehler", "Beim Speichern der Nutzer ist ein Fehler aufgetreten!");
    }
}

void Medienverwaltung::loadData() {
    if (admin->getMediaCountFromFile() == -1 && admin->getUserCountFromFile() == -1) {
        QMessageBox::about(this, "Information", "Es existiert noch kein Speicherstand aus dem Daten geladen werden können.");
        return;
    }
    if (!admin->loadMedia()) {
        QMessageBox::critical(this, "Fehler", "Beim Laden der Medien ist ein Fehler aufgetreten.");
        return;
    }
    if (!admin->loadUsers()) {
        QMessageBox::critical(this, "Fehler", "Beim Laden der Nutzer ist ein Fehler aufgetreten.");
        return;
    }
    updateMediaUI();
    updateUserUI();
}

void Medienverwaltung::fillMediaTable(QMap<int,Media*> mediaData) {
    QTableWidget* table = ui->db_media;
    User* user;
    QComboBox* cB_type;
    QPushButton* btn_delete;
    QIcon icon(":/img/deleteItem.png");
    QTableWidgetItem *id, *title, *surname, *name;
    QStringList cB_items;

    cB_items << "Buch" << "CD" << "DVD";
    table->clearSelection(); // to not cause selecito
    table->clearContents();
    table->setRowCount(mediaData.size());

    QMap<int,Media*>::const_iterator it;
    int rowCount = 0;

    for (it = mediaData.constBegin(); it != mediaData.constEnd(); ++it) {
        id = new QTableWidgetItem();
        id->setData(Qt::UserRole, it.key());

        cB_type = new QComboBox();
        cB_type->setObjectName(QString("cB_mediaType_%1").arg(rowCount));
        cB_type->addItems(cB_items);
        cB_type->setCurrentIndex(it.value()->getType());
        connect(cB_type, SIGNAL(currentIndexChanged(int)), this, SLOT(cB_mediaType_SelectionChanged()));

        title = new QTableWidgetItem(it.value()->getTitle());

        if (it.value()->getUserId() != -1) { // if media is lent...
            user = admin->getUser(it.value()->getUserId());
            surname = new QTableWidgetItem(user->getSurname());
            name = new QTableWidgetItem(user->getName());
        } else {
            surname = new QTableWidgetItem();
            name = new QTableWidgetItem();
        }
        surname->setFlags(surname->flags() & ~Qt::ItemIsEditable);
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);

        btn_delete = new QPushButton();
        btn_delete->setIcon(icon);
        btn_delete->setObjectName(QString("btn_deleteMedia_%1").arg(rowCount));
        btn_delete->setEnabled(it.value()->getUserId() == -1);
        connect(btn_delete, SIGNAL(released()), this, SLOT(btn_deleteMedia_Clicked()));

        table->blockSignals(true);
        table->setItem(rowCount, 0, id); // hidden
        table->setCellWidget(rowCount, 1, cB_type);
        table->setItem(rowCount, 2, title);
        table->setItem(rowCount, 3, surname);
        table->setItem(rowCount, 4, name);
        table->setCellWidget(rowCount, 5, btn_delete);
        table->blockSignals(false);

        rowCount++;
    }
}

void Medienverwaltung::fillUserTable(QMap<int,User*> userData) {
    QTableWidget* table = ui->db_user;
    QPushButton* btn_delete;
    QIcon icon(":/img/deleteItem.png");
    QTableWidgetItem *id, *surname, *name;
    table->clearSelection();
    table->clearContents();
    table->setRowCount(userData.size());

    QMap<int,User*>::const_iterator it;
    int rowCount = 0;

    for (it = userData.constBegin(); it != userData.constEnd(); ++it) {
        id = new QTableWidgetItem();
        id->setData(Qt::UserRole, it.key());

        surname = new QTableWidgetItem(it.value()->getSurname());
        name = new QTableWidgetItem(it.value()->getName());

        btn_delete = new QPushButton();
        btn_delete->setIcon(icon);
        btn_delete->setObjectName(QString("btn_deleteUser_%1").arg(rowCount));
        btn_delete->setEnabled(it.value()->getLentMedia().isEmpty());
        connect(btn_delete, SIGNAL(released()), this, SLOT(btn_deleteUser_Clicked()));

        table->blockSignals(true);
        table->setItem(rowCount, 0, id); // hidden
        table->setItem(rowCount, 1, surname);
        table->setItem(rowCount, 2, name);
        table->setCellWidget(rowCount, 3, btn_delete);
        table->blockSignals(false);

        rowCount++;
    }
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
    fillMediaTable(admin->getMediaList());
}

void Medienverwaltung::updateUserTable() {
    fillUserTable(admin->getUserList());
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

    cB_user->clear();

    QMap<int,User*>::const_iterator it;

    for (it = userList.constBegin(); it != userList.constEnd(); ++it) {
        cB_user->addItem(buildUserItemString(it.value()), it.key());
    }
}

void Medienverwaltung::updateLendingSelection() {
    QComboBox* cB_media = ui->cB_lendMedia_media;
    QMap<int,Media*>& mediaList = admin->getMediaList();
    QComboBox* cB_user = ui->cB_returnMedia_user;
    QMap<int,User*>& userList = admin->getUserList();

    cB_media->clear();

    QMap<int,Media*>::const_iterator it_media;

    // available media
    for (it_media = mediaList.constBegin(); it_media != mediaList.constEnd(); ++it_media) {
        if (it_media.value()->getUserId() == -1) {
            cB_media->addItem(buildMediaItemString(it_media.value()), it_media.key()); // label + id
        }
    }

    cB_user->clear();

    QMap<int,User*>::const_iterator it_user;

    // lending users
    for (it_user = userList.constBegin(); it_user != userList.constEnd(); ++it_user) {
        if (!it_user.value()->getLentMedia().isEmpty()) {
            cB_user->addItem(buildUserItemString(it_user.value()), it_user.key());
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
    loadData();
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

void Medienverwaltung::btn_deleteMedia_Clicked() {
    QPushButton* btn_sender = qobject_cast<QPushButton*>(sender());
    QString btnName = btn_sender->objectName();
    int btnRow;
    QTableWidgetItem* idItem;
    int mediaId;

    btnRow = btnName.mid(btnName.lastIndexOf('_') + 1).toInt(); // Buttons have object names "btn_deleteMedia_rowIndex"
    idItem = ui->db_media->item(btnRow, 0);
    mediaId = idItem->data(Qt::UserRole).toInt();
    if (admin->deleteMedia(mediaId)) {
        updateMediaUI();
    } else {
        QMessageBox::critical(this, "Fehler", "Beim Löschen des Mediums:<br>"
                                                  + buildMediaItemString(admin->getMedia(mediaId))
                                                  + "<br>ist ein Fehler aufgetreten");
    }

}

void Medienverwaltung::btn_deleteUser_Clicked() {
    QPushButton* btn_sender = qobject_cast<QPushButton*>(sender());
    QString btnName = btn_sender->objectName();
    int btnRow;
    QTableWidgetItem* idItem;
    int userId;
    User* user;

    btnRow = btnName.mid(btnName.lastIndexOf('_') + 1).toInt(); // Buttons have object names "btn_deleteUser[RowIndex]"
    idItem = ui->db_media->item(btnRow, 0);
    userId = idItem->data(Qt::UserRole).toInt();
    if (admin->deleteUser(userId)) {
        updateUserUI();
    } else {
        user = admin->getUser(userId);
        QMessageBox::critical(this, "Fehler", "Beim Löschen des Nutzers:<br>"
                                                  + user->getSurname() + " " + user->getName()
                                                  + "<br>ist ein Fehler aufgetreten");
    }

}

void Medienverwaltung::btn_lendMedia_Clicked() {
    int mediaId = ui->cB_lendMedia_media->currentData().toInt();
    int userId = ui->cB_lendMedia_user->currentData().toInt();
    admin->lendMedia(mediaId, userId);
    updateMediaUI();
    updateUserTable(); // update delete-button-status
}

void Medienverwaltung::btn_returnMedia_Clicked() {
    int mediaId = ui->cB_returnMedia_media->currentData().toInt();
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    if (!admin->returnMedia(mediaId, userId)) {
        qDebug() << "returning media failed";
    }
    updateMediaUI();
    updateUserTable();
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
    updateUserTable();
    updateLentMediaList();
}

void Medienverwaltung::db_media_columnHeader_Clicked(int colIndex) {
    switch (colIndex) {
    case 1: // type
        admin->sortMediaListByType();
        break;
    case 2: // title
        admin->sortMediaListByTitle();
        break;
    case 4: // surname
        break;
    case 5: // name
        break;
    }

    fillMediaTable(admin->getMediaList());
}

void Medienverwaltung::db_user_columnHeader_Clicked(int colIndex) {
    QMap<int,User*> sortedUsers;

    switch (colIndex) {
    case 1: // surname
        break;
    case 2: // name
        break;
    }

    fillUserTable(sortedUsers);
}

void Medienverwaltung::db_media_ItemChanged(QTableWidgetItem* item) {
    QTableWidget* table = ui->db_media;
    QTableWidgetItem* idItem = table->item(item->row(), 0);
    int mediaId = idItem->data(Qt::UserRole).toInt();
    Media* modifiedMedia = admin->getMedia(mediaId);

    switch (item->column()) {
    case 2: // title
        modifiedMedia->setTitle(item->text());
        break;
    case 3: // creator
        switch (modifiedMedia->getType()) {
        case BOOK:
            static_cast<Book*>(modifiedMedia)->setAuthor(item->text());
            break;
        case CD:
            static_cast<Cd*>(modifiedMedia)->setInterpret(item->text());
            break;
        case DVD:
            static_cast<Dvd*>(modifiedMedia)->setDirector(item->text());
            break;
        default: // doesn't exist
            break;
        }
    }
    updateMediaUI();
}

void Medienverwaltung::db_user_ItemChanged(QTableWidgetItem* item) {
    QTableWidget* table = ui->db_user;
    QTableWidgetItem* idItem = table->item(item->row(), 0);
    int userId = idItem->data(Qt::UserRole).toInt();
    User* modifiedUser = admin->getUser(userId);

    switch(item->column()) {
    case 1: // surname
        modifiedUser->setSurname(item->text());
        break;
    case 2: // name
        modifiedUser->setName(item->text());
        break;
    }
    updateUserUI();
}

void Medienverwaltung::cB_mediaType_SelectionChanged() {
    QComboBox* cB_type = qobject_cast<QComboBox*>(sender());
    QString cBName = cB_type->objectName();
    MediaType type = (MediaType)cB_type->currentIndex();
    int cBRow = cBName.mid(cBName.indexOf('_') + 1).toInt(); // "cB_media_rowIndex"
    QTableWidgetItem* idItem = ui->db_media->item(cBRow, 0);
    int mediaId = idItem->data(Qt::UserRole).toInt();
    admin->updateMediaType(mediaId, type);
    updateMediaUI();
}

void Medienverwaltung::cB_returnMedia_user_SelectionChanged() {
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    updateReturnableMediaSelection(userId);
}

void Medienverwaltung::db_user_SelectionChanged() {
    updateLentMediaList();
}

void Medienverwaltung::tab_view_CurrentChanged(int index) {
    if (index == 0) { // media
        ui->db_media->clearSelection();
    } else {          // user
        ui->db_user->clearSelection();
        ui->li_lentMedia->clear();
    }
}





