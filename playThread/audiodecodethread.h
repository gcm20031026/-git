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
    AudioDecodeThread();
    void initDecode();

    void run() override;
    void correctAudioDelay(int64_t pts);
    // 获取音频时钟的方法
    double getAudioClock();
    void cleanup();
    void resetDecoder();
public slots:
    void checkFlag(bool flag);
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
