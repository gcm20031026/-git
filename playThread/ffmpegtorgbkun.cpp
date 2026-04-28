#include "ffmpegtorgbkun.h"
#include <QDir>
#include <QMutexLocker>
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

ffmpegToRGBKun::ffmpegToRGBKun(const char* videoFile)
{
    this->videoFile = QString::fromUtf8(videoFile);
}

ffmpegToRGBKun::ffmpegToRGBKun(const char* videoFile, QLabel *videoWin)
{
    this->videoFile = QString::fromUtf8(videoFile);
    this->videoWin=videoWin;
}

void ffmpegToRGBKun::run()
{
    AVFormatContext *pformatContext=nullptr;
    QByteArray videoFileBytes = QDir::toNativeSeparators(videoFile).toLocal8Bit();
    int res = avformat_open_input(&pformatContext, videoFileBytes.constData(), nullptr, nullptr);
    if(res==0)
    {
        qDebug() << "avformat_open_input OK!!!!!!!!!!!!!!!!!";
    }
    else
    {
        qDebug() << "avformat_open_input fail!!!!!!!!!!!!!!!";
        return;
    }

    int index_video=-1; //初始值-1说明没找到
    res = avformat_find_stream_info(pformatContext,nullptr); //查找是否有流
    if(res == 0)
    {
        for(int i=0;i<pformatContext->nb_streams;i++) //找出想要的流
        {
            if(pformatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) //找出流的编码器类型是视频编码器
            {
                //说明找到视频
                index_video = i; //记录视频流在哪个流
                qDebug()<<"index_video="<<index_video;
                break;
            }
        }
    }
    else
    {
        qDebug()<<"avformat_find_stream_info fail!";
        avformat_close_input(&pformatContext);
        return;
    }
    if(index_video < 0)
    {
        qDebug()<<"no video stream!";
        avformat_close_input(&pformatContext);
        return;
    }

    AVCodecParameters *pCodecParameters = pformatContext->streams[index_video]->codecpar;
    AVStream *videoStream = pformatContext->streams[index_video];
    double fps = av_q2d(videoStream->avg_frame_rate);
    if (fps <= 0.0)
    {
        fps = av_q2d(videoStream->r_frame_rate);
    }
    int frameIntervalMs = fps > 0.0 ? static_cast<int>(1000.0 / fps) : 40;
    if (frameIntervalMs < 1)
    {
        frameIntervalMs = 1;
    }
    if (frameIntervalMs > 200)
    {
        frameIntervalMs = 200;
    }

    qint64 durationMs = 0;
    if (videoStream->duration > 0)
    {
        durationMs = static_cast<qint64>(videoStream->duration * av_q2d(videoStream->time_base) * 1000.0);
    }
    if (durationMs <= 0 && pformatContext->duration > 0)
    {
        durationMs = pformatContext->duration / 1000;
    }
    AVCodec *decorder = avcodec_find_decoder(pCodecParameters->codec_id);
    if(nullptr == decorder)
    {
        qDebug()<<"avcodec_find_decoder fail!";
        avformat_close_input(&pformatContext);
        return;
    }
    else
    {
        qDebug()<<"avcodec_find_decoder OK!";
    }
    AVCodecContext *pcodecContext = nullptr;//编解码器上下文封装结构体
    pcodecContext = avcodec_alloc_context3(decorder);
    avcodec_parameters_to_context(pcodecContext, pCodecParameters);

    res=avcodec_open2(pcodecContext,decorder,nullptr);
    if(res==0)
    {
        qDebug() << "avcodec_open2 OK!";
    }
    else
    {
        qDebug() << "avcodec_open2 fail!";
        avcodec_free_context(&pcodecContext);
        avformat_close_input(&pformatContext);
        return;
    }

    AVPacket *pkt = av_packet_alloc();
    AVFrame *picture=av_frame_alloc();
    AVFrame *pictureRGB = av_frame_alloc();
    int numbytes = avpicture_get_size(AV_PIX_FMT_RGB32,pcodecContext->width,pcodecContext->height);
    uint8_t *buffer = (uint8_t*)av_malloc(numbytes*sizeof(uint8_t));
    struct SwsContext * sws = sws_getContext(
                pcodecContext->width,pcodecContext->height,
                pcodecContext->pix_fmt,
                pcodecContext->width,pcodecContext->height,
                AV_PIX_FMT_RGB32,SWS_BICUBIC,
                nullptr,nullptr,nullptr
                );
    if (pkt == nullptr || picture == nullptr || pictureRGB == nullptr || buffer == nullptr || sws == nullptr || numbytes <= 0)
    {
        qDebug() << "ffmpeg replay alloc fail!";
        if (sws != nullptr)
        {
            sws_freeContext(sws);
        }
        av_free(buffer);
        av_frame_free(&pictureRGB);
        av_frame_free(&picture);
        av_packet_free(&pkt);
        avcodec_free_context(&pcodecContext);
        avformat_close_input(&pformatContext);
        return;
    }
    avpicture_fill(
                (AVPicture*)pictureRGB,
                buffer,
                AV_PIX_FMT_RGB32,
                pcodecContext->width,
                pcodecContext->height
                );
    //初始化每一帧图片的大小
    qint64 lastPtsMs = -1;
    bool showFrameAfterSeek = false;
    while(av_read_frame(pformatContext,pkt)==0)
    {
        int requestedSeekPercent = -1;
        {
            QMutexLocker locker(&seekMutex);
            requestedSeekPercent = seekPercent;
            if (seekPercent >= 0)
            {
                seekPercent = -1;
            }
        }
        if (requestedSeekPercent >= 0 && durationMs > 0)
        {
            int percent = requestedSeekPercent;
            qint64 targetMs = (durationMs * percent) / 100;
            qint64 targetTimestamp = static_cast<qint64>((targetMs / 1000.0) / av_q2d(videoStream->time_base));
            if (av_seek_frame(pformatContext, index_video, targetTimestamp, AVSEEK_FLAG_BACKWARD) >= 0)
            {
                avcodec_flush_buffers(pcodecContext);
                lastPtsMs = -1;
                showFrameAfterSeek = true;
                emit progressChanged(percent);
            }
            av_packet_unref(pkt);
            continue;
        }
        if(deleteStopBool)
        {
            av_packet_unref(pkt);
            break; //结束线程
        }
        if (pkt->stream_index == index_video)
        {
            if (avcodec_send_packet(pcodecContext, pkt) < 0)
            {
                av_packet_unref(pkt);
                continue;
            }
            if (avcodec_receive_frame(pcodecContext, picture) < 0)
            {
                av_packet_unref(pkt);
                continue;
            }
            sws_scale(
                        sws,
                        picture->data,
                        picture->linesize,
                        0,
                        picture->height,
                        pictureRGB->data,
                        pictureRGB->linesize
                        );
            QImage img = QImage(
                        (uchar*)buffer,
                        pcodecContext->width,
                        pcodecContext->height,
                        QImage::Format_RGB32
                        );
            qint64 currentPtsMs = lastPtsMs < 0 ? 0 : lastPtsMs + frameIntervalMs;
            if (picture->best_effort_timestamp != AV_NOPTS_VALUE)
            {
                currentPtsMs = static_cast<qint64>(picture->best_effort_timestamp * av_q2d(videoStream->time_base) * 1000.0);
            }

            int sleepMs = frameIntervalMs;
            if (lastPtsMs >= 0 && currentPtsMs > lastPtsMs)
            {
                sleepMs = static_cast<int>(currentPtsMs - lastPtsMs);
                if (sleepMs < 1)
                {
                    sleepMs = 1;
                }
                if (sleepMs > 200)
                {
                    sleepMs = 200;
                }
            }
            lastPtsMs = currentPtsMs;

            while(beginAndStopBool==false && !deleteStopBool && !showFrameAfterSeek)
            {
                bool hasSeekRequest = false;
                {
                    QMutexLocker locker(&seekMutex);
                    hasSeekRequest = seekPercent >= 0;
                }
                if (hasSeekRequest)
                {
                    break;
                }
                msleep(10);
            } //暂停
            bool hasSeekRequest = false;
            {
                QMutexLocker locker(&seekMutex);
                hasSeekRequest = seekPercent >= 0;
            }
            if (hasSeekRequest)
            {
                av_packet_unref(pkt);
                continue;
            }
            if (deleteStopBool)
            {
                av_packet_unref(pkt);
                break;
            }
            //通过信号发送图片
            toImg(img,videoWin);
            emit toImg(img.copy(), videoWin);
            showFrameAfterSeek = false;
            if (durationMs > 0)
            {
                int progress = static_cast<int>((currentPtsMs * 100) / durationMs);
                if (progress < 0)
                {
                    progress = 0;
                }
                if (progress > 100)
                {
                    progress = 100;
                }
                emit progressChanged(progress);
            }
            double speed = 1.0;
            {
                QMutexLocker locker(&seekMutex);
                speed = playbackSpeed;
            }
            int adjustedSleepMs = static_cast<int>(sleepMs / speed);
            if (adjustedSleepMs < 1)
            {
                adjustedSleepMs = 1;
            }
            msleep(adjustedSleepMs);
            av_packet_unref(pkt);
        }
        else
        {
            av_packet_unref(pkt);
        }
    }

    if (sws != nullptr)
    {
        sws_freeContext(sws);
    }
    av_free(buffer);
    av_frame_free(&pictureRGB);
    av_frame_free(&picture);
    av_packet_free(&pkt);
    avcodec_free_context(&pcodecContext);
    avformat_close_input(&pformatContext);
    if (!deleteStopBool)
    {
        emit progressChanged(100);
    }
}

void ffmpegToRGBKun::setbeginAndStopBool(bool BOOL)
{
    this->beginAndStopBool=BOOL;
}

void ffmpegToRGBKun::setdeleteStopBool(bool BOOL)
{
    this->deleteStopBool=BOOL;
}

void ffmpegToRGBKun::requestSeekPercent(int percent)
{
    if (percent < 0)
    {
        percent = 0;
    }
    if (percent > 100)
    {
        percent = 100;
    }
    QMutexLocker locker(&seekMutex);
    seekPercent = percent;
}

void ffmpegToRGBKun::setPlaybackSpeed(double speed)
{
    if (speed < 0.25)
    {
        speed = 0.25;
    }
    if (speed > 4.0)
    {
        speed = 4.0;
    }
    QMutexLocker locker(&seekMutex);
    playbackSpeed = speed;
}

QLabel *ffmpegToRGBKun::getVideoWin()
{
    return this->videoWin;
}

