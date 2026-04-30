#ifndef AUDIODECODETHREAD_H
#define AUDIODECODETHREAD_H
#include<QString>
#include<QThread>

#include <QIODevice>
#include<QElapsedTimer>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavdevice/avdevice.h>  // 添加这行
}
class AudioDecodeThread:public QThread
{
public:
    // 构造音频解码线程。
    AudioDecodeThread();
    void initDecode(); // 初始化音频解码器、重采样器等资源。

    void run() override; // 音频解码线程入口。
    void correctAudioDelay(int64_t pts); // 根据视频 PTS 校正音频播放延迟。
    // 获取音频时钟的方法
    double getAudioClock(); // 获取当前音频时钟。
    void cleanup(); // 清理解码相关资源。
    void resetDecoder(); // 重置音频解码器状态。
public slots:
    void checkFlag(bool flag); // 接收播放/暂停状态。
private:
    AVFormatContext *VPContext;
    AVCodecContext *Vcodecontext;
    AVCodec *vDecoder;
    AVPacket *packet;
    SwrContext *m_swrContext = nullptr;
    int index;

    QIODevice *m_audioIO = nullptr;

    QElapsedTimer m_playbackTimer;
    int64_t m_audioPts = AV_NOPTS_VALUE;
    int64_t m_lastPts = AV_NOPTS_VALUE;
    double m_audioClock = 0;
    bool flag;

};

#endif // AUDIODECODETHREAD_H
