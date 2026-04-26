#include "ffmpegwriterdest.h"
#include <QDebug>
FFmpegWriterDest::FFmpegWriterDest(QObject *parent) : QThread(parent)
{

}
FFmpegWriterDest::~FFmpegWriterDest()
{
    stop();    // 停止线程
    cleanup(); // 清理资源
}

/**
 * @brief 初始化编码器和输出文件
 * @param filename 输出MP4文件路径
 * @param width 视频宽度
 * @param height 视频高度
 * @param fps 帧率
 * @return 初始化是否成功
 */
bool FFmpegWriterDest::initialize(const QString &filename, int width, int height, int fps)
{
    m_filename = filename;
    m_width = width;
    m_height = height;
    m_fps = fps;

    return initFFmpeg(); // 初始化FFmpeg相关资源
}
bool FFmpegWriterDest::initFFmpeg()
{
    // 1. 创建输出格式上下文
    int ret = avformat_alloc_output_context2(&m_formatCtx, nullptr, nullptr, m_filename.toUtf8());
    if (ret < 0) {
        char errbuf[256];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning() << "Failed to create output context:" << errbuf;
        return false;
    }

    // 2. 查找编码器（尝试使用 libx264）
    AVCodec *codec = avcodec_find_encoder_by_name("libx264");
    if (!codec)
    {
        codec = avcodec_find_encoder(AV_CODEC_ID_H264); // 回退到默认 H.264 编码器
        if (!codec)
        {
            qWarning() << "H.264 encoder not found";
            return false;
        }
    }
    // 3. 创建编码器上下文
    m_codecCtx = avcodec_alloc_context3(codec);
    if (!m_codecCtx) {
        qWarning() << "Could not allocate video codec context";
        return false;
    }
    // 4. 配置编码参数
    m_codecCtx->bit_rate = 4000000;
    m_codecCtx->width = m_width;
    m_codecCtx->height = m_height;
    m_codecCtx->time_base = (AVRational){1, m_fps};
    m_codecCtx->framerate = (AVRational){m_fps, 1};
    m_codecCtx->gop_size = 10;
    m_codecCtx->max_b_frames = 1;
    m_codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    // 强制设置像素格式
    m_codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    // 添加这些关键参数
    av_opt_set(m_codecCtx->priv_data, "preset", "fast", 0);
    av_opt_set(m_codecCtx->priv_data, "tune", "zerolatency", 0);
    av_opt_set(m_codecCtx->priv_data, "profile", "baseline", 0);
    av_opt_set_int(m_codecCtx->priv_data, "crf", 23, 0);  // 质量参数(18-28)

    // 5. 打开编码器
    if (avcodec_open2(m_codecCtx, codec, nullptr) < 0)
    {
        char errbuf[256];
        av_strerror(ret, errbuf, sizeof(errbuf));
        qWarning() << "Could not open codec:" << errbuf;

        // 打印当前编码器设置
        qWarning() << "Codec settings:";
        qWarning() << "  width:" << m_codecCtx->width;
        qWarning() << "  height:" << m_codecCtx->height;
        qWarning() << "  pix_fmt:" << m_codecCtx->pix_fmt;
        qWarning() << "  time_base:" << m_codecCtx->time_base.num << "/" << m_codecCtx->time_base.den;
        return false;
    }

    // 6. 创建输出视频流
    m_stream = avformat_new_stream(m_formatCtx, codec);
    if (!m_stream) {
        qWarning() << "Failed allocating output stream";
        return false;
    }

    // 7. 配置流参数
    m_stream->time_base = m_codecCtx->time_base;
    avcodec_parameters_from_context(m_stream->codecpar, m_codecCtx);

    // 8. 打开输出文件
    if (!(m_formatCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&m_formatCtx->pb, m_filename.toUtf8().constData(), AVIO_FLAG_WRITE) < 0) {
            qWarning() << "Could not open output file";
            return false;
        }
    }

    // 9. 写入文件头
    if (avformat_write_header(m_formatCtx, nullptr) < 0) {
        qWarning() << "Error occurred when opening output file";
        return false;
    }

    // 10. 初始化图像转换上下文（RGB32转YUV420P）
    m_swsCtx = sws_getContext(
        m_width, m_height, AV_PIX_FMT_RGB32,  // 输入格式
        m_width, m_height, AV_PIX_FMT_YUV420P, // 输出格式
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    return true;
}
/**
 * @brief 添加视频帧到编码队列（线程安全）
 * @param image 要添加的视频帧（QImage格式）
 */
void FFmpegWriterDest::addFrame(const QImage &image)
{
    QMutexLocker locker(&m_mutex); // 自动加锁
    m_frameQueue.enqueue(image.copy()); // 复制图像数据（避免外部修改）
    m_condition.wakeOne(); // 唤醒可能正在等待的线程
}

/**
 * @brief 停止录制
 */
void FFmpegWriterDest::stop()
{
    m_running = false;       // 设置停止标志
    m_condition.wakeAll();   // 唤醒线程（如果正在等待）
    wait();                  // 等待线程结束
}
/**
 * @brief 线程主函数（不断从队列取帧并编码）
 */
void FFmpegWriterDest::run()
{
    m_running = true; // 设置运行标志

    // 1. 分配AVFrame用于存储转换后的帧
    AVFrame *frame = av_frame_alloc();
    frame->format = AV_PIX_FMT_YUV420P;
    frame->width = m_width;
    frame->height = m_height;
    av_frame_get_buffer(frame, 32); // 分配内存（32字节对齐）

    // 2. 主循环：处理帧队列
    while (m_running)
    {
        QImage qimage;
        // 2.1 从队列获取帧（线程安全）
        {
            QMutexLocker locker(&m_mutex);
            if (m_frameQueue.isEmpty())
            {
                m_condition.wait(&m_mutex); // 等待新帧
                continue;
            }
            qimage = m_frameQueue.dequeue(); // 取出最早的一帧
        }
        // 2.2 将QImage转换为AVFrame（RGB32转YUV420P）
        const uint8_t *srcData[1] = { qimage.bits() };  // 数据指针
        int srcLinesize[1] = { qimage.bytesPerLine() }; // 行大小

        sws_scale(m_swsCtx,
                 srcData, srcLinesize, 0, m_height, // 输入参数
                 frame->data, frame->linesize);      // 输出参数

        // 2.3 设置显示时间戳（PTS）
        frame->pts = m_frameCounter++;

        // 2.4 编码并写入帧
        if (!writeFrame(frame)) {
            qWarning() << "Failed to write frame";
        }
    }
    // 3. 释放AVFrame
    av_frame_free(&frame);

    // 4. 写入文件尾
    av_write_trailer(m_formatCtx);
}
/**
 * @brief 编码并写入一帧
 * @param frame 要写入的AVFrame
 * @return 是否成功
 */
bool FFmpegWriterDest::writeFrame(AVFrame *frame)
{
    AVPacket pkt = {0};
    av_init_packet(&pkt); // 初始化数据包

    // 1. 发送帧到编码器
    int ret = avcodec_send_frame(m_codecCtx, frame);
    if (ret < 0) {
        qWarning() << "Error sending frame to encoder";
        return false;
    }
    // 2. 接收编码后的数据包
        while (ret >= 0)
        {
            ret = avcodec_receive_packet(m_codecCtx, &pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break; // 需要更多输入或已结束
            } else if (ret < 0) {
                qWarning() << "Error during encoding";
                return false;
            }

            // 3. 调整时间戳
            av_packet_rescale_ts(&pkt, m_codecCtx->time_base, m_stream->time_base);
            pkt.stream_index = m_stream->index;

            // 4. 写入文件
            ret = av_interleaved_write_frame(m_formatCtx, &pkt);
            if (ret < 0) {
                qWarning() << "Error while writing video frame";
                return false;
            }

            av_packet_unref(&pkt); // 释放数据包
        }
    return true;
}
void FFmpegWriterDest::cleanup()
{
    // 1. 释放图像转换上下文
    if (m_swsCtx) {
        sws_freeContext(m_swsCtx);
        m_swsCtx = nullptr;
    }

    // 2. 释放编码器上下文
    if (m_codecCtx) {
        avcodec_free_context(&m_codecCtx);
        m_codecCtx = nullptr;
    }

    // 3. 释放输出文件上下文
    if (m_formatCtx) {
        if (!(m_formatCtx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&m_formatCtx->pb); // 关闭文件
        }
        avformat_free_context(m_formatCtx);
        m_formatCtx = nullptr;
    }
}

