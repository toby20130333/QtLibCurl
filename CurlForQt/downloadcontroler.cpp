#include "downloadcontroler.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

DownloadControler::DownloadControler(QObject *parent) :
    QThread(parent),resumeDownload(false),downloadFileLenth(0)
{
    connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
}

DownloadControler::~DownloadControler()
{
    curl_easy_cleanup(handle);
}

void DownloadControler::initData(const QString &downUrl)
{
    mUrl = downUrl;
}

long DownloadControler::getLocalFileLenth(const char *localPath)
{
    if (!resumeDownload){
        return 0;
    }
    //return fs_open(localPath).fs_size();
}

long DownloadControler::getDownloadFileLenth(const char *url)
{
    long downloadFileLenth = 0;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只需要header头
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //不需要body
    if (curl_easy_perform(handle) == CURLE_OK) {
        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
    } else {
        downloadFileLenth = -1;
    }
    return downloadFileLenth;
}

void DownloadControler::getProgressValue(const char *localSize, double dt, double dn, double ult, double uln,void *user)
{
    DownloadControler* down = (DownloadControler*)user;
    double showTotal, showNow;
    showTotal = down->downloadFileLenth;
    int localNow = atoi (localSize);
    showNow = localNow + dn;
    qDebug()<<showNow<<": "<<showTotal;
    emit down->signalDownloadProgress(showNow,showTotal);
}
DownloadControler::CurlCode DownloadControler::scomoDownload(long timeout)
{
    long localFileLenth = getLocalFileLenth("./xx.zip");
    const char *localFileLenthStr;
    FILE *fp = fopen("data.html", "ab+");
    //sprintf(localFileLenthStr, %ld, localFileLenth);
    curl_easy_setopt(handle, CURLOPT_URL, mUrl.toUtf8().data());
    curl_easy_setopt(handle, CURLOPT_HEADER, 0);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
    //        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &writeDataCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(handle, CURLOPT_RESUME_FROM_LARGE, localFileLenth);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, getProgressValue);
    curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, localFileLenthStr);
    if (curl_easy_perform(handle)) {
        resumeDownload = true;
        return DS_FAILED;
    } else {
        resumeDownload = false;
        return DS_FINISHED;
    }
}

void DownloadControler::downloadControler()
{
    /* downloadControl函数用来控制整个下载过程的节奏, 控制下载的次数, 每次等待的时间等 */
    downloadFileLenth = getDownloadFileLenth(mUrl.toUtf8().data());			//下载前得到要下载的文件大小赋值给全局变量
    qDebug()<<"downloadFileLenth:"<<downloadFileLenth;
    int times = 605;						//600次*50ms=5分钟, 以此确保5分钟内的重试次数, 而5次是正常下载的中断次数, 意思即是5次内能正常完成下载.
    int count = 0;
    int timeout = 30;
    CurlCode dstatus = DS_FAILED;
    while (count++ < times){
        dstatus = scomoDownload(timeout);
        qDebug()<<"while......"<<dstatus;
        if (dstatus == DS_FINISHED){
            break;
        }
        sleep(10);				//每次下载中间间隔500毫秒
    }
    resumeDownload = false;				//不管下载成功或失败, 完成while循环后将标志回位
    if (dstatus == DS_FINISHED) {
        qDebug()<<"DS_FINISHED:";				//执行软件安装的操作…
        emit downloadFinished(false);
    }
    //SAFE_DELETE(localFile);				//流程最后要确保本地文件删除
}

void DownloadControler::run()
{
    downloadControler();
    this->exec();
}
