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
    DecodeThread(QString fileName,QString devName);
    ~DecodeThread();
    void run();
    void initDecode();
    void seekToFrame(int seconds);//跳转帧
    // 添加设置音频解码器指针的方法
    void setAudioThread(AudioDecodeThread* audioThread);
    void pause();    // 暂停线程
    void resume();   // 恢复线程
    bool isPaused(); // 检查状态

public slots:
    void checkFlag(bool flag);
    void getProgressTime(int value);
    void getSpeed(int speed);//获取倍速
    void set_X_Y(int x,int y);
    void savaPictureScr(QString picName);//保存截图
    void startStopDetectfaceSlot(bool detectFaceFlag);
    void saveVideos();
    void getTime(QString m_times);

signals:
    void sentImg(QImage img);
    void sentvideoTime(double times);
    void sentProgressTime(double times);

private:
    AVFormatContext *VPContext;
    AVCodecContext *Vcodecontext;
    AVCodec *vDecoder;
    AVPacket *packet;
    AVFrame *picture;
    AVFrame *pictureRGB;
    uint8_t *bufferRGB;
    struct SwsContext *sws;
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
    int x=640,y=480;
    QMutex m_mutex;
    QWaitCondition m_pauseCondition;
    bool m_paused = false;//锁
    bool savaflagScr=false;
    QString picSrcName;//截图名字
    QString videosSavePath;
    CEncodeToh264*file_h264;
    openCVDetect* detectFace;//人脸检测
    bool detectFaceFlag=false;//是否开启人脸检测
    int id;
    CTranceToDest *de;
    QString m_times;
    QString devName;
    QString file264;
};

#endif // DECODETHREAD_H
