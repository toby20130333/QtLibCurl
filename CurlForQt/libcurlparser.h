#ifndef LIBCURLPARSER_H
#define LIBCURLPARSER_H

#include <QThread>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include "./libcurl/include/curl/curl.h"
using namespace  std;
#include <iostream>

class libcurlParser : public QThread
{
    Q_OBJECT
public:
    static size_t process(void* buffer,size_t size,size_t n,void *user);
    QString html;
public:
    libcurlParser(QObject *parent= 0);
    ~libcurlParser();
    void initData(const QString&);

public:
    void execute(const QString& url);
public slots:
    void finishSlot();
    void timeOut();
signals:
    void finish();
protected:
    void run();
private:
    CURL*  curl;
    QString m_Url;
};

#endif // LIBCURLPARSER_H
