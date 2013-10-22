#-------------------------------------------------
#
# Project created by QtCreator 2013-10-22T09:58:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CurlForQt
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    libcurlparser.cpp \
    downloadcontroler.cpp

HEADERS  += widget.h \
    libcurlparser.h \
    downloadcontroler.h

INCLUDEPATH += "D:/TobyYi/CurlForQt/libcurl/include/curl"
win32:LIBS += -L"D:/TobyYi/CurlForQt/libcurl/" -lcurl
