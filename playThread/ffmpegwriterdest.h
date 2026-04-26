#ifndef FFMPEGWRITERDEST_H
#define FFMPEGWRITERDEST_H

#include <QObject>

#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include<QImage>
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
class FFmpegWriterDest : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegWriterDest(QObject *parent = nullptr);
    ~FFmpegWriterDest();

    // 初始化FFmpeg编码器和输出文件
    bool initialize(const QString &filename, int width, int height, int fps);

    // 添加视频帧到写入队列（线程安全）
    void addFrame(const QImage &image);

    // 停止录制并等待线程结束
    void stop();

protected:
    // 线程主运行函数（重写QThread）
    void run() override;

private:
    // 初始化FFmpeg相关资源
    bool initFFmpeg();

    // 清理FFmpeg资源
    void cleanup();

    // 将AVFrame写入文件
    bool writeFrame(AVFrame *frame);
    // 线程同步
    QMutex m_mutex;               // 保护帧队列的互斥锁
    QWaitCondition m_condition;   // 线程等待条件
    QQueue<QImage> m_frameQueue;  // 待编码的帧队列
    bool m_running = false;       // 线程运行标志

    // FFmpeg相关资源
    AVFormatContext *m_formatCtx = nullptr; // 输出文件上下文
    AVCodecContext *m_codecCtx = nullptr;   // 编码器上下文
    AVStream *m_stream = nullptr;           // 输出视频流
    SwsContext *m_swsCtx = nullptr;         // 图像转换上下文

    // 视频参数
    int m_width;        // 视频宽度
    int m_height;       // 视频高度
    int m_fps;          // 帧率
    QString m_filename; // 输出文件名
    int64_t m_frameCounter = 0; // 帧计数器（用于PTS）

    // 时间基准（用于时间戳计算）
    AVRational m_timeBase;

signals:

};

#endif // FFMPEGWRITERDEST_H
