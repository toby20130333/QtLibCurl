#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "libcurlparser.h"
#include "./libcurl/include/curl/curl.h"
#include "downloadcontroler.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    libcurlParser* m_libCurl;
    DownloadControler* m_down;
    int down_file(const char* url, const char* down_file_name);
    static size_t process_data(void *buffer, size_t size, size_t nmemb, std::string& user_p);
};

#endif // WIDGET_H
