QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HospitalManagementSystem
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# Only do this if you want to standardize on a new Qt release.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    patient.cpp \
    hospital.cpp \
    doctor.cpp \
    nurse.cpp \
    hospitalsystem.cpp \
    pharmacy.cpp \
    pharmacysystem.cpp

HEADERS += \
    mainwindow.h \
    patient.h \
    hospital.h \
    doctor.h \
    nurse.h \
    hospitalsystem.h \
    pharmacy.h \
    pharmacysystem.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
