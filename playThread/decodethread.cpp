#include "decodethread.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QDir>
#include "data/qmysqlite.h"
#include "data/userstorage.h"
extern QString savaFormat;
DecodeThread::DecodeThread(QString fileName, QString devName)
{
    this->fileName = fileName;
    this->devName = devName;
    initDecode();
    if (!decodeReady)
    {
        return;
    }
    this->detectFace = new openCVDetect();
    if (cameraInput)
    {
        file264 = QString("%1/%2.h264").arg(SavePath).arg(devName);
        file_h264 = new CEncodeToh264(file264);
        file_h264->getWidthAndHeight(Vcodecontext->width, Vcodecontext->height); // 必须先设置宽高
        file_h264->initEncode();
        file_h264->start();
        QString saveVideoFile = QString("%1/%2%3").arg(SavePath).arg(devName).arg(savaFormat);
        de = new CTranceToDest(saveVideoFile, file264);
        de->start();
    }
}

DecodeThread::~DecodeThread()
{
}
// 线程跑动解码播放
void DecodeThread::run()
{
    if (!decodeReady)
    {
        return;
    }
    // 7、读码流数据
    int num = 0;
    QString path;
    // 发送视频总时间
    AVRational timebase = VPContext->streams[index]->time_base;
    int duration = VPContext->streams[index]->duration;
    double durationInSeconds = duration * timebase.num / (double)timebase.den;
    emit this->sentvideoTime(durationInSeconds);

    const int frameDelayMs = cameraInput ? 10 : 33;
    while (1)
    {
        m_mutex.lock();
        if (m_paused)
        {
            m_pauseCondition.wait(&m_mutex); // 阻塞直到被唤醒
        }
        m_mutex.unlock();
        int readResult = av_read_frame(VPContext, packet);
        if (readResult == 0) // 将视频一帧一帧读出来，放在码流数据中。avpacket（码流数据） 解封装。
        {
            int ptr = -1;
            if (packet->stream_index == index) // 确保是视频的数据，因为里面也有可能有音频。 解码流数据。
            {
                // 解码视频数据包
                avcodec_decode_video2(Vcodecontext, picture, &ptr, packet); // 将 avpacket ---avframe
                if (ptr != 0)
                {
                    // sws_scale用于 图像缩放和像素格式转换
                    sws_scale(sws, (const uint8_t *const *)picture->data, picture->linesize,
                              0, Vcodecontext->height, pictureRGB->data, pictureRGB->linesize);
                    if (file_h264 != nullptr && swsYUV != nullptr && pictureYUV != nullptr)
                    {
                        // 只有真实摄像头录制时才转换 YUV，模拟视频只做预览。
                        sws_scale(swsYUV, (const uint8_t *const *)picture->data, picture->linesize,
                                  0, Vcodecontext->height, pictureYUV->data, pictureYUV->linesize);
                        file_h264->addFrame(pictureYUV);
                    }
                    QImage img = QImage((uchar *)bufferRGB, Vcodecontext->width, Vcodecontext->height, QImage::Format_RGB32);
                    if (detectFaceFlag)
                    {
                        bool runFaceDetection = false;
                        if (detectFaceFlag)
                        {
                            ++faceDetectFrameCount;
                            runFaceDetection = (faceDetectFrameCount == 1 || faceDetectFrameCount % 2 == 0);
                        }
                        else
                        {
                            faceDetectFrameCount = 0;
                        }

                        img = detectFace->detectAndDraw(img, detectFaceFlag, runFaceDetection);
                    }

                    emit this->sentImg(img);
                    UserStorage::ensureCurrentUserDirs();
                    QString file = QDir(UserStorage::pictureDir()).filePath(QString("%1.png").arg(this->picSrcName));
                    // 截图保存
                    if (savaflagScr == true)
                    {
                        qDebug() << "savaflagScr:" << savaflagScr;
                        img.save(file);
                        savaflagScr = false;
                    }

                    QThread::msleep(frameDelayMs);
                }
            }
            av_packet_unref(packet); // 释放packet引用
        }
        else if (!cameraInput)
        {
            av_seek_frame(VPContext, index, 0, AVSEEK_FLAG_BACKWARD);
            avcodec_flush_buffers(Vcodecontext);
            av_packet_unref(packet);
            QThread::msleep(frameDelayMs);
        }
        else
        {
            QThread::msleep(10);
        }
    }
}

// 初始化解码
void DecodeThread::initDecode()
{
    // 1注册组件
    av_register_all();
    avdevice_register_all(); // 注册摄像头
    // 视频封装上下文
    this->VPContext = nullptr;

    int res;
    //    //2打开视频
    //    int res = avformat_open_input(&VPContext,"Web Camera",nullptr,nullptr);
    //    if(res!=0)
    //    {
    //        qDebug() << "avformat_open_input error";
    //    }else qDebug()<<"avformat_open_input ok";

    /*
     * windows上打开摄像头
     * dshow：推流方式
     */
    cameraInput = fileName.startsWith("video=");
    AVInputFormat *fmt = cameraInput ? av_find_input_format("dshow") : nullptr;
    QByteArray inputName = cameraInput
            ? fileName.toUtf8()
            : QDir::toNativeSeparators(fileName).toLocal8Bit();

    res = avformat_open_input(&VPContext, inputName.constData(), fmt, nullptr);
    if (res != 0)
    {
        qDebug() << (cameraInput ? "摄像头打开失败" : "模拟视频打开失败") << fileName;
        return;
    }

    // 3找到流媒体
    res = avformat_find_stream_info(VPContext, nullptr);
    if (res < 0)
    {
        qDebug() << "avformat_find_stream_info error";
    }
    else
        qDebug() << "avformat_find_stream_info ok";
    // 流的类型 视频流 音频流 字幕流
    index = -1;
    for (int i = 0; i < VPContext->nb_streams; ++i)
    {
        // 视频流
        if (VPContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            index = i;
            break;
        }
    }
    // 没找到
    if (index == -1)
    {
        qDebug() << "not stream";
        return;
    }

    // 5放入编解码器上下文封装
    this->Vcodecontext = VPContext->streams[index]->codec;
    // 6找到流对应的编解码器
    this->vDecoder = avcodec_find_decoder(Vcodecontext->codec_id);
    if (nullptr == vDecoder)
    {
        qDebug() << "avcodec_find_decoder error";
    }
    else
        qDebug() << "avcodec_find_decoder ok";
    // 7打开编解码器
    res = avcodec_open2(Vcodecontext, vDecoder, nullptr);
    if (res < 0)
    {
        qDebug() << "avcodec_open2 error";
    }
    else
        qDebug() << "avcodec_open2 ok";
    // 8创建AVPacket AVPacket：解析文件时会将音/视频帧读入到packet中
    this->packet = nullptr;
    // 分配AVPacket结构体的内存,每一个packet大小（码流数据大小）
    packet = (AVPacket *)malloc(sizeof(AVPacket));
    // 每一帧图片的大小，解码器的宽度和高度
    int size = Vcodecontext->width * Vcodecontext->height;
    av_new_packet(packet, size); // packet包
    // AVFrame用于保存数据帧的数据结构，AVFrame 用于保存转换之后的帧
    this->picture = av_frame_alloc();    // 需要这个空间用来填充一帧AVFrame
    this->pictureRGB = av_frame_alloc(); // 这个空间用来填充一帧的RGB
    if (cameraInput)
    {
        // 初始化YUV帧。只有真实摄像头需要写入录制编码队列。
        this->pictureYUV = av_frame_alloc();
        if (!pictureYUV)
        {
            qDebug() << "Failed to allocate YUV frame!";
            return;
        }
        pictureYUV->format = AV_PIX_FMT_YUV420P;
        pictureYUV->width = Vcodecontext->width;
        pictureYUV->height = Vcodecontext->height;
        int numbytesYUV = avpicture_get_size(AV_PIX_FMT_YUV420P, Vcodecontext->width, Vcodecontext->height);
        this->bufferYUV = (uint8_t *)av_malloc(numbytesYUV * sizeof(uint8_t));
        if (!bufferYUV)
        {
            qDebug() << "Failed to allocate YUV buffer!";
            return;
        }

        avpicture_fill((AVPicture *)pictureYUV, bufferYUV, AV_PIX_FMT_YUV420P, Vcodecontext->width, Vcodecontext->height);
        this->swsYUV = sws_getContext(
            Vcodecontext->width, Vcodecontext->height, Vcodecontext->pix_fmt,
            Vcodecontext->width, Vcodecontext->height,
            AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
        if (!swsYUV)
        {
            qDebug() << "Failed to create YUV conversion context!";
            return;
        }
    }

    // 需要计算一下，填充一帧RGB格式需要多大空间
    int numbytes = avpicture_get_size(AV_PIX_FMT_RGB32, Vcodecontext->width, Vcodecontext->height);
    // 申请用来填充一帧RGB32图片需要的空间
    // 缓冲区分配内存
    this->bufferRGB = (uint8_t *)av_malloc(numbytes * sizeof(uint8_t));
    // 填充一张RGB32图片
    // 初始化缓冲区 类似于内存的memset-开辟完清理操作
    avpicture_fill((AVPicture *)pictureRGB, bufferRGB, AV_PIX_FMT_RGB32, Vcodecontext->width, Vcodecontext->height);
    // 转置规则
    this->sws = nullptr; // SwsContext:转换器
    /*srcW, srcH	输入图像的宽和高
    srcFormat	输入图像的像素格式（如 AV_PIX_FMT_YUV420P）
    dstW, dstH	输出图像的宽和高（可缩放）
    dstFormat	输出图像的像素格式（如 AV_PIX_FMT_RGB32）
    flags	缩放算法标志（如 SWS_BILINEAR）
    srcFilter, dstFilter	源/目标图像的滤波器（通常设为 NULL）
    param	算法参数（通常设为 NULL）*/
    sws = sws_getContext(
        Vcodecontext->width, Vcodecontext->height, Vcodecontext->pix_fmt,
        Vcodecontext->width, Vcodecontext->height, // 输出尺寸减小
        AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);
    decodeReady = sws != nullptr && (!cameraInput || swsYUV != nullptr);
}
// 跳转到具体的秒
void DecodeThread::seekToFrame(int seconds)
{

    static char errorString[AV_ERROR_MAX_STRING_SIZE]; // 静态数组，用于存储错误信息

    if (index < 0 || !VPContext->streams[index])
    {
        qDebug() << "Error: Invalid stream index";
        return;
    }

    AVStream *stream = VPContext->streams[index];
    // 计算目标时间戳（基于秒数和帧率）
    double fps = av_q2d(stream->avg_frame_rate);
    if (fps <= 0)
    {
        fps = 25.0; // 默认帧率（避免除零错误）
        qDebug() << "Warning: Using default frame rate of 25 fps";
    }
    int64_t timestamp = av_rescale_q(
        seconds * fps,             // 将秒数转换为帧数
        (AVRational){1, (int)fps}, // 帧数 → 时间戳
        stream->time_base);

    // 执行跳转（跳转到最近的关键帧）
    int ret = av_seek_frame(VPContext, index, timestamp, AVSEEK_FLAG_BACKWARD);
    if (ret < 0)
    {
        av_make_error_string(errorString, AV_ERROR_MAX_STRING_SIZE, ret);
        qDebug() << "Seek failed:" << errorString;
        return;
    }

    // 清空解码器缓存（必须！否则后续帧可能错乱）
    avcodec_flush_buffers(Vcodecontext);
    qDebug() << "Seek to second" << seconds << "successful";
}
// 添加设置音频解码器指针的方法
void DecodeThread::setAudioThread(AudioDecodeThread *audioThread)
{
    this->m_audio = audioThread;
}

// 播放状态改变
void DecodeThread::checkFlag(bool flag)
{
    this->flag = flag;
}
// 获取进度时间
void DecodeThread::getProgressTime(int value)
{

    this->targetFrame = value;
}
// 倍数转换
void DecodeThread::getSpeed(int speed)
{
    this->speedFrame = 25 / speed;
    qDebug() << speedFrame;
}

void DecodeThread::set_X_Y(int x, int y)
{
    this->x = x;
    this->y = y;
}

void DecodeThread::savaPictureScr(QString picSrcName)
{
    this->picSrcName = picSrcName;
    this->savaflagScr = true;
}

void DecodeThread::startStopDetectfaceSlot(bool detectFaceFlag)
{
    this->detectFaceFlag = detectFaceFlag;
    this->faceDetectFrameCount = 0;
}

void DecodeThread::saveVideos()
{
    if (de == nullptr)
    {
        qDebug() << "模拟视频通道不启动录制转码";
        emit alarmRaised(QString::fromUtf8(u8"低"), devName, QString::fromUtf8(u8"模拟视频不支持录像"));
        return;
    }

    QDir dir(SavePath);
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            emit alarmRaised(QString::fromUtf8(u8"高"), devName, QString::fromUtf8(u8"录像目录创建失败"));
            return;
        }
    }

    QString saveTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString fileTime = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString safeDevName = devName;
    safeDevName.replace("/", "_");
    safeDevName.replace("\\", "_");
    safeDevName.replace(":", "_");
    safeDevName.replace("*", "_");
    safeDevName.replace("?", "_");
    safeDevName.replace("\"", "_");
    safeDevName.replace("<", "_");
    safeDevName.replace(">", "_");
    safeDevName.replace("|", "_");

    QString saveVideoFile = dir.filePath(QString("%1_%2%3").arg(safeDevName).arg(fileTime).arg(savaFormat));
    de->setOutputFile(saveVideoFile);
    de->setFlag(true);

    QMySqlite *db = QMySqlite::getInstance(UserStorage::recordDbPath());
    if (!db->insertVideoRecord(devName, saveVideoFile, saveTime, savaFormat))
    {
        emit alarmRaised(QString::fromUtf8(u8"高"), devName, QString::fromUtf8(u8"录像写入数据库失败"));
    }
}

void DecodeThread::getTime(QString m_times)
{
    this->m_times = m_times;
}

void DecodeThread::pause()
{
    QMutexLocker locker(&m_mutex);
    m_paused = true;
}

void DecodeThread::resume()
{
    QMutexLocker locker(&m_mutex);
    if (m_paused)
    {
        m_paused = false;
        m_pauseCondition.wakeOne(); // 唤醒线程
    }
}

bool DecodeThread::isPaused()
{
    QMutexLocker locker(&m_mutex);
    return m_paused;
}
