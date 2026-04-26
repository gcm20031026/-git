#ifndef FFMPEGTORGBKUN_H
#define FFMPEGTORGBKUN_H

#include <QDebug>
#include <QImage>
#include <QThread>
#include <QLabel>

class ffmpegToRGBKun:public QThread
{
    Q_OBJECT
private:
    const char* videoFile; //文件路径
    QLabel *videoWin; //用于标识要播放在哪里
    bool beginAndStopBool=true; //线程暂停信号
    bool deleteStopBool=false; //线程关闭信号
public:
    ffmpegToRGBKun(const char* videoFile);
    ffmpegToRGBKun(const char* videoFile, QLabel *videoWin);
    void run();
    void setbeginAndStopBool(bool BOOL);
    void setdeleteStopBool(bool BOOL);
    QLabel* getVideoWin(); //获取位置对象
signals:
    void toImg(QImage,QLabel*);
};

#endif // FFMPEGTORGBKUN_H
