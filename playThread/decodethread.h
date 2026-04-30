#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include<QThread>
#include<QImage>
#include<playThread/audiodecodethread.h>
#include<QString>
#include <QMutex>
#include <QWaitCondition>
#include"buildTarVideo/cencodetoh264.h"
#include"buildTarVideo/opencvdetect.h"
#include"Control/pathwitheditbutton.h"
#include"buildTarVideo/ctrancetodest.h"
#include"Control/formatoption.h"
extern QString SavePath;  //
extern "C"
{
    #include "libavcodec/avcodec.h"       //编解码库
    #include "libavdevice/avdevice.h"     //输入输出设备，读取摄像头的
    #include "libavfilter/avfilter.h"     //音视频滤镜库，进行音视频处理和编辑
    #include "libavformat/avformat.h"     //文件格式和协议库
    #include "libavutil/avutil.h"         //音视频处理
    #include "libswresample/swresample.h" //音频重采样
    #include "libswscale/swscale.h"       //图像进行格式转换
}
class DecodeThread:public QThread
{
    Q_OBJECT
public:
    // 构造视频解码线程，fileName 可为摄像头或视频文件。
    DecodeThread(QString fileName,QString devName);
    ~DecodeThread(); // 停止线程并释放 FFmpeg 解码资源。
    void run(); // 视频解码线程入口，持续读取帧并发送给界面。
    void initDecode(); // 初始化 FFmpeg 输入、解码器和图像转换资源。
    void seekToFrame(int seconds);// 跳转到指定秒数附近的帧。
    // 添加设置音频解码器指针的方法
    void setAudioThread(AudioDecodeThread* audioThread); // 绑定音频解码线程。
    void pause();    // 暂停解码线程。
    void resume();   // 恢复解码线程。
    bool isPaused(); // 检查当前是否暂停。

public slots:
    void checkFlag(bool flag); // 接收播放状态标志。
    void getProgressTime(int value); // 根据进度条位置执行跳转。
    void getSpeed(int speed);// 设置播放倍速。
    void set_X_Y(int x,int y); // 设置输出图像尺寸。
    void savaPictureScr(QString picName);// 保存当前帧截图。
    void startStopDetectfaceSlot(bool detectFaceFlag); // 开启或关闭人脸检测。
    void saveVideos(); // 开始保存当前视频。
    void getTime(QString m_times); // 接收录制时间字符串。

signals:
    void sentImg(QImage img); // 输出解码后的图像帧。
    void sentvideoTime(double times); // 通知视频总时长或当前时长。
    void sentProgressTime(double times); // 通知播放进度。

    void alarmRaised(const QString &level, const QString &source, const QString &type);
private:
    AVFormatContext *VPContext = nullptr;
    AVCodecContext *Vcodecontext = nullptr;
    AVCodec *vDecoder = nullptr;
    AVPacket *packet = nullptr;
    AVFrame *picture = nullptr;
    AVFrame *pictureRGB = nullptr;
    uint8_t *bufferRGB = nullptr;
    struct SwsContext *sws = nullptr;
    int index;
    bool flag;
    int targetFrame = -1;
    int speedFrame =25;//默认一配速25帧
    AudioDecodeThread *m_audio=nullptr;
    QElapsedTimer m_playbackTimer;
    AVFrame* pictureYUV;  // 用于存储YUV格式的帧
    uint8_t* bufferYUV;   // YUV帧缓冲区
    SwsContext* swsYUV;   // 用于YUV转换的上下文
    QString fileName;
    bool cameraInput = false;
    bool decodeReady = false;
    int x=640,y=480;
    QMutex m_mutex;
    QWaitCondition m_pauseCondition;
    bool m_paused = false;//锁
    bool savaflagScr=false;
    QString picSrcName;//截图名字
    QString videosSavePath;
    CEncodeToh264 *file_h264 = nullptr;
    openCVDetect* detectFace;//人脸检测
    bool detectFaceFlag=false;//是否开启人脸检测
    int faceDetectFrameCount = 0;
    int id;
    CTranceToDest *de = nullptr;
    QString m_times;
    QString devName;
    QString file264;
};

#endif // DECODETHREAD_H
