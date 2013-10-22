#include "libcurlparser.h"

size_t libcurlParser::process(void* buffer,size_t size,size_t n,void *user)
{
    libcurlParser* parser = (libcurlParser*)user;
    parser->html += (char*)buffer;
    qDebug()<<"libcurlParser::process "<<size*n;
    parser->finishSlot();
    return size*n;
}

libcurlParser::libcurlParser(QObject *parent)
    : QThread(parent)
{
    connect(this,SIGNAL(finish()),this,SLOT(finishSlot()));
    curl = curl_easy_init();
}

libcurlParser::~libcurlParser()
{
    qDebug()<<"skjafkhajksfafhjkashjkfa";
    curl_easy_cleanup(curl);
}

void libcurlParser::initData(const QString &url)
{
    m_Url = url;
}

void libcurlParser::execute(const QString& url)
{
    html.clear();
    CURLcode return_code;
    curl_easy_setopt(curl,CURLOPT_URL,url.toUtf8().data());
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,&process);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,this);
    qDebug()<<"downloadFileLenth: ";
    //判断获取响应的http地址是否存在,若存在则返回200,400以上则为不存在,一般不存在为404错误
    int retcode = 0;
    return_code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
//    const char* sss = curl_easy_getinfo(curl,CURLINFO_EFFECTIVE_URL,&url);
    qDebug()<<"retcode: "<<retcode;
    start();
}

void libcurlParser::run()
{
    execute(m_Url);
    CURLcode code = curl_easy_perform(curl);
    std::cout<<"exe:"<<"code"<<code<<std::endl;
    // emit finish();
    std::cout<<curl_easy_strerror(code)<<std::endl;
    //    QTimer::singleShot(0, this, SLOT(timeOut()));
    exec();
}

void libcurlParser::finishSlot()
{
    std::cout<<"finishSlot()"<<std::endl;
    QFile file("htm.htm");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    file.write(html.toLatin1());
    file.close();
}

void libcurlParser::timeOut()
{
    emit finish();
}
