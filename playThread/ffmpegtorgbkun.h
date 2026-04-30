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
    // 构造回放解码线程，仅指定视频文件。
    ffmpegToRGBKun(const char *videoFile);
    // 构造回放解码线程，并绑定显示标签。
    ffmpegToRGBKun(const char *videoFile, QLabel *videoWin);
    void run(); // 解码线程入口，将视频帧转换为 QImage 输出。
    void setbeginAndStopBool(bool BOOL); // 设置播放/暂停状态。
    void setdeleteStopBool(bool BOOL); // 设置删除停止标志，用于关闭回放线程。
    void requestSeekPercent(int percent); // 请求按百分比跳转播放位置。
    void setPlaybackSpeed(double speed); // 设置回放倍速。
    QLabel* getVideoWin(); // 返回当前绑定的显示标签。
signals:
    void toImg(QImage,QLabel*); // 输出解码后的图像和目标显示标签。
    void progressChanged(int value); // 通知 UI 更新播放进度。
};

#endif // FFMPEGTORGBKUN_H
