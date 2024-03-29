# Name: Simon Fliegel
# Matr. Nr: 53043

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    administration.cpp \
    book.cpp \
    cd.cpp \
    dvd.cpp \
    main.cpp \
    media.cpp \
    mediaTypes.cpp \
    medienverwaltung.cpp \
    user.cpp

HEADERS += \
    administration.h \
    book.h \
    cd.h \
    dvd.h \
    media.h \
    mediaTypes.h \
    medienverwaltung.h \
    user.h

FORMS += \
    medienverwaltung.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
