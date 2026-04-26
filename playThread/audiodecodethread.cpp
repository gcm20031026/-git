#include "audiodecodethread.h"
#include<QDebug>
AudioDecodeThread::AudioDecodeThread()
{

    initDecode();

}

void AudioDecodeThread::initDecode()
{
    //1注册组件
    //av_register_all();
    avdevice_register_all();//注册摄像头
    //视频封装上下文
    this->VPContext=nullptr;

    //2打开视频
    int res = avformat_open_input(&VPContext,"Web Camera",nullptr,nullptr);
    if(res!=0)
    {
        qDebug() << "avformat_open_input error";
    }else qDebug()<<"avformat_open_input ok";
    //3找到流媒体
    res = avformat_find_stream_info(VPContext,nullptr);
    if(res <0)
    {
        qDebug()<<"avformat_find_stream_info error";
    }else qDebug() << "avformat_find_stream_info ok";
    index=-1;
    for(int i=0;i<VPContext->nb_streams;++i)
    {
        //音频流
        if(VPContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
        {
            index=i;
            break;
        }
    }
    //没找到
    if(index==-1)
    {
        qDebug()<<"not stream";
    }

    //5放入编解码器上下文封装
    this->Vcodecontext = VPContext->streams[index]->codec;
    //6找到流对应的编解码器
    this->vDecoder = avcodec_find_decoder(Vcodecontext->codec_id);
    if(nullptr==vDecoder)
    {
        qDebug() << "avcodec_find_decoder error";
    }else  qDebug() << "avcodec_find_decoder ok";
    //7打开编解码器
    res = avcodec_open2(Vcodecontext,vDecoder,nullptr);
    if(res<0)
    {
        qDebug() <<"avcodec_open2 error";
    }else  qDebug() << "avcodec_open2 ok";
    m_swrContext = swr_alloc();
    av_opt_set_int(m_swrContext, "in_channel_layout", Vcodecontext->channel_layout, 0);
    av_opt_set_int(m_swrContext, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(m_swrContext, "in_sample_rate", Vcodecontext->sample_rate, 0);
    av_opt_set_int(m_swrContext, "out_sample_rate", 44100, 0);
    av_opt_set_sample_fmt(m_swrContext, "in_sample_fmt", Vcodecontext->sample_fmt, 0);
    av_opt_set_sample_fmt(m_swrContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    swr_init(m_swrContext);
}



void AudioDecodeThread::run()
{
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();

    while(true) {
        // 重置解码器状态
        resetDecoder();

        while(av_read_frame(VPContext, packet) >= 0)
        {
            if(packet->stream_index == index)
            {
                // 发送数据包给解码器
                if(avcodec_send_packet(Vcodecontext, packet) < 0)
                {
                    qWarning() << "Error sending packet for decoding";
                    av_packet_unref(packet);
                    continue;
                }

                // 更新音频时钟
                if(packet->pts != AV_NOPTS_VALUE)
                {
                    correctAudioDelay(packet->pts);
                }

                // 接收解码帧
                while(avcodec_receive_frame(Vcodecontext, frame) >= 0)
                {
                    uint8_t *output = nullptr;
                    av_samples_alloc(&output, nullptr, 2, frame->nb_samples,
                                    AV_SAMPLE_FMT_S16, 0);
                    int out_samples = swr_convert(m_swrContext, &output, frame->nb_samples,
                                                 (const uint8_t **)frame->data, frame->nb_samples);

                    if(out_samples > 0 && m_audioIO && m_audioIO->isOpen())
                    {
                        int data_size = av_samples_get_buffer_size(nullptr, 2, out_samples,
                                                                 AV_SAMPLE_FMT_S16, 1);
                        m_audioIO->write((const char *)output, data_size);

                    }

                    av_freep(&output);
                    av_frame_unref(frame);
                }
            }
            av_packet_unref(packet);
        }

        // 文件读完或暂停时清理
        if(flag)
        {
            // 重置文件读取位置
            av_seek_frame(VPContext, index, 0, AVSEEK_FLAG_BACKWARD);
            avcodec_flush_buffers(Vcodecontext);
        }
    }

    // 最终清理
    cleanup();
    av_frame_free(&frame);
    av_packet_free(&packet);
}

//延迟
void AudioDecodeThread::correctAudioDelay(int64_t pts)
{
    if (pts == AV_NOPTS_VALUE) return;

    if (m_lastPts != AV_NOPTS_VALUE)
    {
        double delay = (pts - m_lastPts) * av_q2d(VPContext->streams[index]->time_base);
        m_audioClock += delay;

        double diff = m_audioClock - m_playbackTimer.elapsed() / 1000.0;

        if (diff > 0.05) {
            QThread::msleep(static_cast<int>(diff * 1000));
        } else if (diff < -0.1) {
            // 追赶策略
            m_audioClock = m_playbackTimer.elapsed() / 1000.0;
        }
    }
    m_lastPts = pts;

}

// 获取音频时钟的方法
double AudioDecodeThread::getAudioClock()
{
    return this->m_audioClock;
}

void AudioDecodeThread::cleanup()
{
    if(m_audioIO) {
        m_audioIO->reset();
        m_audioIO = nullptr;
    }
}

void AudioDecodeThread::resetDecoder()
{
    avcodec_flush_buffers(Vcodecontext);
    m_audioClock = 0;
    m_lastPts = AV_NOPTS_VALUE;
    m_playbackTimer.restart();
}

void AudioDecodeThread::checkFlag(bool flag)
{
     this->flag = flag;

}
