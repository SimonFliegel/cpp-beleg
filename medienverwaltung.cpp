/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#include "medienverwaltung.h"
#include "ui_medienverwaltung.h"

#include <QtDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QShortcut>

#include "mediaTypes.h"
#include "book.h"
#include "cd.h"
#include "dvd.h"

bool isChanged = false;

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
    initGBAddMedia();
    initGBSearch();
    initDEBirthdate(ui->dE_registerUser_birthdate);
    ui->btn_returnMedia_user->setEnabled(false);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(actionSave_Triggered()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(actionLoad_Triggered()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(actionDelete_Triggered()));
    connect(ui->btn_addMedia, SIGNAL(released()), this, SLOT(btn_addMedia_Clicked()));
    connect(ui->btn_registerUser, SIGNAL(released()), this, SLOT(btn_registerUser_Clicked()));
    connect(ui->btn_lendMedia, SIGNAL(released()), this, SLOT(btn_lendMedia_Clicked()));
    connect(ui->btn_returnMedia, SIGNAL(released()), this, SLOT(btn_returnMedia_Clicked()));
    connect(ui->btn_returnMedia_user, SIGNAL(released()), this, SLOT(btn_returnMedia_user_Clicked()));
    connect(ui->btn_searchMedia, SIGNAL(released()), this, SLOT(btn_searchMedia_Clicked()));
    connect(ui->btn_searchUser, SIGNAL(released()), this, SLOT(btn_searchUser_Clicked()));
    connect(ui->btn_resetSearchUser, SIGNAL(released()), this, SLOT(btn_resetSearchUser_Clicked()));
    connect(ui->btn_resetSearchMedia, SIGNAL(released()), this, SLOT(btn_resetSearchMedia_Clicked()));
    connect(ui->db_media, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(db_media_ItemChanged(QTableWidgetItem*)));
    connect(ui->db_user, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(db_user_ItemChanged(QTableWidgetItem*)));
    connect(ui->cB_returnMedia_user, SIGNAL(currentIndexChanged(int)), this, SLOT(cB_returnMedia_user_SelectionChanged()));
    connect(ui->li_lentMedia, SIGNAL(itemSelectionChanged()), this, SLOT(li_lentMedia_SelectionChanged()));
    connect(ui->db_user, SIGNAL(itemSelectionChanged()), this, SLOT(db_user_SelectionChanged()));
    connect(ui->tab_view, SIGNAL(currentChanged(int)) ,this, SLOT(tab_view_CurrentChanged(int)));
}

void Medienverwaltung::initMediaTable()
{
    QTableWidget* table = ui->db_media;
    QStringList headers;
    headers << "ID" << "Typ" << "Titel" << "Autor/Interpret/Regisseur" << "Vorname" << "Nachname" << "Löschen";
    table->setColumnCount(7); // type, title, creator, user-surname, user-name, delete-button
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 60);
    table->setColumnWidth(2, 250);
    table->setColumnWidth(3, 200);
    table->setColumnWidth(4, 174);
    table->setColumnWidth(5, 174);
    table->setColumnWidth(6, 50);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Medienverwaltung::initUserTable() {
    QTableWidget* table = ui->db_user;
    QStringList headers;
    headers << "ID" << "Vorname" << "Nachname" << "Email" << "Geburtsdatum" << "Löschen";
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels(headers);
    table->setColumnHidden(0, true); // id column
    table->setColumnWidth(1, 200);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 320);
    table->setColumnWidth(4, 150);
    table->setColumnWidth(5, 50);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Medienverwaltung::initGBAddMedia() {
    QStringList items;
    items << "Buch" << "CD" << "DVD";
    ui->cB_addMedia_type->addItems(items);
    ui->cB_addMedia_type->setCurrentIndex(0);
}

void Medienverwaltung::initGBSearch() {
    ui->tab_view->setCurrentIndex(0);
    QStringList items;
    items << "Buch" << "CD" << "DVD";
    ui->cB_searchMedia_type->addItems(items);
    ui->btn_resetSearchMedia->setEnabled(false);
    ui->btn_resetSearchUser->setEnabled(false);
}

void Medienverwaltung::initDEBirthdate(QDateEdit* de) {
    de->setMinimumDate(QDate(1900, 1, 1));
    de->setMaximumDate(QDate::currentDate());
}

bool Medienverwaltung::isValidEmail(QString email) {
    static QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    QRegularExpressionMatch match = emailRegex.match(email);

    if (match.hasMatch() && match.captured(0) == email) {
        return true;
    } else {
        return false;
    }
}

void Medienverwaltung::fillMediaTable(QMap<int,Media*> mediaData) {
    QTableWidget* table = ui->db_media;
    User* user;
    QComboBox* cB_type;
    QPushButton* btn_delete;
    QIcon icon(":/img/deleteItem.png");
    QTableWidgetItem *id, *title, *creator, *surname, *name;
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
        creator = new QTableWidgetItem();

        switch(it.value()->getType()) {
        case BOOK:
            creator->setText(static_cast<Book*>(it.value())->getAuthor());
            break;
        case CD:
            creator->setText(static_cast<Cd*>(it.value())->getInterpret());
            break;
        case DVD:
            creator->setText(static_cast<Dvd*>(it.value())->getDirector());
            break;
        }

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
        table->setItem(rowCount, 3, creator);
        table->setItem(rowCount, 4, surname);
        table->setItem(rowCount, 5, name);
        table->setCellWidget(rowCount, 6, btn_delete);
        table->blockSignals(false);

        rowCount++;
    }
}

void Medienverwaltung::fillUserTable(QMap<int,User*> userData) {
    QTableWidget* table = ui->db_user;
    QPushButton* btn_delete;
    QDateEdit* dE_birthdate;
    QIcon icon(":/img/deleteItem.png");
    QTableWidgetItem *id, *surname, *name, *email;
    table->clearSelection();
    table->clearContents();
    table->setRowCount(userData.size());

    QMap<int,User*>::const_iterator it;
    int rowCount = 0;

    for (it = userData.constBegin(); it != userData.constEnd(); ++it) {
        id = new QTableWidgetItem();
        id->setData(Qt::UserRole, it.key());

        name = new QTableWidgetItem(it.value()->getName());
        surname = new QTableWidgetItem(it.value()->getSurname());
        email = new QTableWidgetItem(it.value()->getEmail());

        dE_birthdate = new QDateEdit();
        dE_birthdate->setObjectName(QString("dE_birthdate_%1").arg(rowCount));
        initDEBirthdate(dE_birthdate);
        dE_birthdate->setDate(it.value()->getBirthdate());
        connect(dE_birthdate, SIGNAL(dateChanged(QDate)), this, SLOT(dE_birthdate_DateChanged()));

        btn_delete = new QPushButton();
        btn_delete->setIcon(icon);
        btn_delete->setObjectName(QString("btn_deleteUser_%1").arg(rowCount));
        btn_delete->setEnabled(it.value()->getLentMedia().isEmpty());
        connect(btn_delete, SIGNAL(released()), this, SLOT(btn_deleteUser_Clicked()));

        table->blockSignals(true);
        table->setItem(rowCount, 0, id); // hidden
        table->setItem(rowCount, 1, name);
        table->setItem(rowCount, 2, surname);
        table->setItem(rowCount, 3, email);
        table->setCellWidget(rowCount, 4, dE_birthdate);
        table->setCellWidget(rowCount, 5, btn_delete);
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
    userItemString = user->getName() + " " + user->getSurname();
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

    // FIXME: causes segfault?
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

void Medienverwaltung::actionLoad_Triggered() {
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

void Medienverwaltung::actionDelete_Triggered() {
    int response = QMessageBox::question(this, "Löschen", "Sollen wirklich alle Daten gelöscht werden?");
    if (response == QMessageBox::No) {
        return;
    }
    admin->clearMedia();
    admin->clearUsers();
    updateMediaUI();
    updateUserUI();
}

void Medienverwaltung::btn_addMedia_Clicked() {
    MediaType type = (MediaType)ui->cB_addMedia_type->currentIndex();
    QString title = ui->lE_addMedia_title->text();
    QString creator = ui->lE_addMedia_creator->text();

    if (title.isEmpty()) {
        QMessageBox::information(this, "Information", "Angabe zu Titel benötigt!");
        return;
    }

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
    }

    admin->addMedia(newMedia);
    updateMediaUI();
    isChanged = true;
}

void Medienverwaltung::btn_registerUser_Clicked() {
    QString name = ui->lE_registerUser_name->text();
    QString surname = ui->lE_registerUser_surname->text();
    QString email = ui->lE_registerUser_email->text();

    if (name.isEmpty() || surname.isEmpty() || email.isEmpty()) {
        QMessageBox::information(this, "Information", "Angaben zu Vorname, Name, Email und Geburtsdatum benötigt!");
        return;
    }

    if (!isValidEmail(email)) {
        QMessageBox::information(this, "Information", "Keine gültige Email-Adresse!");
        ui->lE_registerUser_email->clear();
        return;
    }

    QDate birthdate = ui->dE_registerUser_birthdate->date();
    User* newUser = new User(name, surname, email, birthdate);
    admin->registerUser(newUser);
    updateUserUI();
    ui->lE_registerUser_surname->clear();
    ui->lE_registerUser_name->clear();
    ui->lE_registerUser_email->clear();
    ui->dE_registerUser_birthdate->setDate(ui->dE_registerUser_birthdate->minimumDate());
    isChanged = true;
}

void Medienverwaltung::btn_deleteMedia_Clicked() {
    QPushButton* btn_sender = qobject_cast<QPushButton*>(sender());
    QString btnName = btn_sender->objectName();
    int btnRow;
    QTableWidgetItem* idItem;
    int mediaId;

    btnRow = btnName.split('_').last().toInt(); // Buttons have object names "btn_deleteMedia_rowIndex"
    idItem = ui->db_media->item(btnRow, 0);
    mediaId = idItem->data(Qt::UserRole).toInt();
    if (admin->deleteMedia(mediaId)) {
        updateMediaUI();
    } else {
        QMessageBox::critical(this, "Fehler", "Beim Löschen des Mediums:<br>"
                                                  + buildMediaItemString(admin->getMedia(mediaId))
                                                  + "<br>ist ein Fehler aufgetreten");
    }
    isChanged = true;
}

void Medienverwaltung::btn_deleteUser_Clicked() {
    QPushButton* btn_sender = qobject_cast<QPushButton*>(sender());
    QString btnName = btn_sender->objectName();
    int btnRow;
    QTableWidgetItem* idItem;
    int userId;
    User* user;

    btnRow = btnName.split('_').last().toInt(); // Buttons have object names "btn_deleteUser[RowIndex]"
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
    isChanged = true;
}

void Medienverwaltung::btn_lendMedia_Clicked() {
    int mediaId = ui->cB_lendMedia_media->currentData().toInt();
    int userId = ui->cB_lendMedia_user->currentData().toInt();
    admin->lendMedia(mediaId, userId);
    updateMediaUI();
    updateUserTable(); // update delete-button-status
    isChanged = true;
}

void Medienverwaltung::btn_returnMedia_Clicked() {
    int mediaId = ui->cB_returnMedia_media->currentData().toInt();
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    if (!admin->returnMedia(mediaId, userId)) {
        qDebug() << "returning media failed";
    }
    updateMediaUI();
    updateUserTable();
    isChanged = true;
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
    isChanged = true;
}

void Medienverwaltung::btn_searchMedia_Clicked() {
    MediaType type = (MediaType)ui->cB_searchMedia_type->currentIndex();
    QString title = ui->lE_searchMedia_title->text();
    QString creator = ui->lE_searchMedia_creator->text();
    fillMediaTable(admin->searchMedia(type, title, creator));
    ui->btn_resetSearchMedia->setEnabled(true);
}

void Medienverwaltung::btn_searchUser_Clicked() {
    QString surname = ui->lE_searchUser_surname->text();
    QString name = ui->lE_searchUser_name->text();
    if (surname.isEmpty() && name.isEmpty()) return;
    fillUserTable(admin->searchUsers(surname, name));
    ui->btn_resetSearchUser->setEnabled(true);
}

void Medienverwaltung::btn_resetSearchMedia_Clicked() {
    ui->cB_searchMedia_type->setCurrentIndex(0);
    ui->lE_searchMedia_title->clear();
    ui->lE_searchMedia_creator->clear();
    fillMediaTable(admin->getMediaList());
    ui->btn_resetSearchMedia->setEnabled(false);
}

void Medienverwaltung::btn_resetSearchUser_Clicked() {
    ui->lE_searchUser_surname->clear();
    ui->lE_searchUser_name->clear();
    fillUserTable(admin->getUserList());
    ui->btn_resetSearchUser->setEnabled(false);
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
        }
    }
    updateMediaUI();
    isChanged = true;
}

void Medienverwaltung::db_user_ItemChanged(QTableWidgetItem* item) {
    QTableWidget* table = ui->db_user;
    QTableWidgetItem* idItem = table->item(item->row(), 0);
    int userId = idItem->data(Qt::UserRole).toInt();
    User* modifiedUser = admin->getUser(userId);

    switch(item->column()) {
    case 1: // name
        modifiedUser->setName(item->text());
        break;
    case 2: // surname
        modifiedUser->setSurname(item->text());
        break;
    case 3: // email
        if (isValidEmail(item->text())) {
            modifiedUser->setEmail(item->text());
            break;
        } else {
            QMessageBox::information(this, "Information", "Keine gültige Email-Adresse!");
            break;
        }
    }
    updateUserUI();
    isChanged = true;
}

void Medienverwaltung::cB_mediaType_SelectionChanged() {
    QComboBox* cB_type = qobject_cast<QComboBox*>(sender());
    QString cBName = cB_type->objectName();
    MediaType type = (MediaType)cB_type->currentIndex();
    int cBRow = cBName.split('_').last().toInt(); // "cB_media_rowIndex"
    QTableWidgetItem* idItem = ui->db_media->item(cBRow, 0);
    int mediaId = idItem->data(Qt::UserRole).toInt();
    admin->updateMediaType(mediaId, type);
    updateMediaUI();
    isChanged = true;
}

void Medienverwaltung::dE_birthdate_DateChanged() {
    QDateEdit* dE_birthdate = qobject_cast<QDateEdit*>(sender());
    QString dEName = dE_birthdate->objectName();
    QDate birthdate = dE_birthdate->date();
    int dERow = dEName.split('_').last().toInt(); // "dE_birthdate_rowIndex"
    QTableWidgetItem* idItem = ui->db_user->item(dERow, 0);
    int userId = idItem->data(Qt::UserRole).toInt();
    User* user = admin->getUser(userId);
    user->setBirthdate(birthdate);
    updateUserUI();
    isChanged = true;
}

void Medienverwaltung::cB_returnMedia_user_SelectionChanged() {
    int userId = ui->cB_returnMedia_user->currentData().toInt();
    updateReturnableMediaSelection(userId);
}

void Medienverwaltung::li_lentMedia_SelectionChanged() {
    QListWidget* li_lentMedia = qobject_cast<QListWidget*>(sender());
    if (li_lentMedia->selectedItems().size() > 0) {
        ui->btn_returnMedia_user->setEnabled(true);
    } else {
        ui->btn_returnMedia_user->setEnabled(false);
    }
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

/************************************* public slots *****************************************/

void Medienverwaltung::app_aboutToQuit() {
    if (isChanged) {
        int response = QMessageBox::question(this, "Speichern", "Sollen die Änderungen gespeichert werden?");
        if (response == QMessageBox::Yes) {
            actionSave_Triggered();
        }
    }

}
