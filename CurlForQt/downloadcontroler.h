#ifndef DOWNLOADCONTROLER_H
#define DOWNLOADCONTROLER_H

#include <QThread>
#include <QFile>
#include <QDebug>

#include "./libcurl/include/curl/curl.h"

class DownloadControler : public QThread
{
    enum CurlCode{
        DS_FAILED = 0,
        DS_FINISHED,    /* 1 */
        DS_OK
      };
    Q_OBJECT
public:
    explicit DownloadControler(QObject *parent = 0);
    ~DownloadControler();

    void initData(const QString& downUrl);
    /* 得到本地文件大小的函数, 若不是续传则返回0, 否则返回指定路径地址的文件大小 */
    long getLocalFileLenth(const char* localPath);
    /* 得到远程文件的大小, 要下载的文件大小 */
    long getDownloadFileLenth(const char *url);
    /* scomoDownload回调的计算进度条的函数 */
    static void getProgressValue(const char* localSize, double dt, double dn, double ult, double uln,void *user);
    /* 直接进行下载的函数 */
     CurlCode scomoDownload(long timeout);
     /* downloadControl函数用来控制整个下载过程的节奏, 控制下载的次数, 每次等待的时间等 */
     void downloadControler();

public:
    //全局变量
    bool resumeDownload;		//是否需要下载的标记位
    long downloadFileLenth;			//需要下载的总大小, 远程文件的大小
    CURL *handle;
    QString mUrl;
signals:
    void signalDownloadProgress(qint64 cur,qint64 total);
    void downloadFinished(bool downStatus);
public slots:
protected:
    void run();
};

#endif // DOWNLOADCONTROLER_H
