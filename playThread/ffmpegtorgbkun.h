#ifndef FFMPEGTORGBKUN_H
#define FFMPEGTORGBKUN_H

#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMutex>
#include <QString>
#include <QThread>

class ffmpegToRGBKun : public QThread
{
    Q_OBJECT
private:
    QString videoFile;
    QLabel *videoWin = nullptr;
    bool beginAndStopBool = true;
    bool deleteStopBool = false;
    QMutex seekMutex;
    int seekPercent = -1;
    double playbackSpeed = 1.0;
public:
    ffmpegToRGBKun(const char *videoFile);
    ffmpegToRGBKun(const char *videoFile, QLabel *videoWin);
    void run();
    void setbeginAndStopBool(bool BOOL);
    void setdeleteStopBool(bool BOOL);
    void requestSeekPercent(int percent);
    void setPlaybackSpeed(double speed);
    QLabel* getVideoWin();
signals:
    void toImg(QImage,QLabel*);
    void progressChanged(int value);
};

#endif // FFMPEGTORGBKUN_H
