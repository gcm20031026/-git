#include "cencodetoh264.h"
#include <QDebug>

CEncodeToh264::CEncodeToh264(QString fileName)
{
    //编码的初始化
    //1、注册组建
    av_register_all(); //注册组件  用来初始化编解码器
    this->fileName = fileName;
    this->pkt_index = 0;  //时间基做个初始化
    this->pkt = av_packet_alloc(); //开空间

}

void CEncodeToh264::initEncode()
{

    //1、注册组建
    av_register_all(); //注册组件  用来初始化编解码器
    this->fileName = fileName;
    this->pkt_index = 0;  //时间基做个初始化
    this->pkt = av_packet_alloc(); //开空间
    //1、猜测格式 参数1：null  参数2：写入猜想的文件名.h264  参数3：空值
    //AVOutputFormat 结构体 用户描述何存储音视频编码器的输出格式  代表一种特定的音视频的输出格式

    if(width <= 0 || height <= 0) {
        qDebug() << "Width and height must be set before initialization";
        return;
    }
    this->poutformat = av_guess_format(nullptr,fileName.toUtf8(),nullptr);  //通过猜想格式才会知道什么编码器
    if(nullptr == this->poutformat)
    {
        qDebug() << "av_guess_format fail";
    }
    else {
        qDebug() << "av_guess_format success";
    }
    //2、输出文件  封装上下文格式结构体设置输出格式   pformatcontext 上下文封装结构体
    this->pformatContext = nullptr;
    this->pformatContext = avformat_alloc_context();  //分配空间
    this->pformatContext->oformat = this->poutformat; //猜测的格式写到上下文封装结构体
    //3、打开文件流
    /*参数1：AVIOContext **s 结构体  用作提供音视频输入输出的操作 avio.h
     * 既可以用作读 用作写不同类型的媒体数据  文件  网络流
    */
    int res = avio_open(&pformatContext->pb,fileName.toUtf8(),AVIO_FLAG_WRITE);
    if(res < 0)
    {
        qDebug() << "avio_open fail";
    }
    else {
        qDebug() << "avio_open success";
    }
    //4、 新建视频流 AVStream *avformat_new_stream(AVFormatContext *s, const AVCodec *c);
    // 参数1：上下文封装结构体  参数2：一个指向已存在的编解码器的指针 nullptr  会自动尝试选择一个合适输出格式编码器
    this->newStream = avformat_new_stream(this->pformatContext,nullptr);
    if(nullptr == this->newStream)
    {
        qDebug() << "avformat_new_stream fail";
    }
    else {
        qDebug() << "avformat_new_stream success";
    }

    //5、获取编解码器的上下文结构体  设置编解码的上下文结构体
    this->pcoderContext = this->newStream->codec;
    this->pcoderContext->width = this->width;
    this->pcoderContext->height = this->height;
    this->pcoderContext->time_base = {1,25};   //时间基
    /*时间基 1个单位25帧  单位1s  1h   第一帧  0  1/25*1*2
     * pts显示时间戳：表示每一帧的播放时间点
     * dts解码时间戳：每一帧解码的时间点
     * pts=dts
   */
    this->pcoderContext->bit_rate = 4000000;  //码率 码率的大小也会决定视频的质量  视频单位时间内传输的数据量
    this->pcoderContext->framerate = {30,1}; //一个但是播放25帧
    this->pcoderContext->gop_size = 20; //group picture  照片组  每20帧有一个关键帧
    //6、影响清晰度的参数
    this->pcoderContext->qmax = 30;//压缩量化的技术最大值  压得越狠 视频文件越小
    this->pcoderContext->qmin = 10;//压缩量化的技术最小值
    this->pcoderContext->max_b_frames = 0;  //没有B帧  pts =dts
    this->pcoderContext->pix_fmt = AV_PIX_FMT_YUV420P;
    this->pcoderContext->codec_type = AVMEDIA_TYPE_VIDEO;
    this->pcoderContext->codec_id = poutformat->video_codec; //编码器的ID

    //7、查找编码器
    this->pencorder = avcodec_find_encoder(pcoderContext->codec_id);
    if(nullptr == this->pencorder)
    {
         qDebug() << "avcodec_find_encoder fail";
    }
    else {
        qDebug() << "avcodec_find_encoder success";
    }
    //8、打开编码器
    res = avcodec_open2(this->pcoderContext, this->pencorder, nullptr);
    if(res != 0)
    {
         qDebug() << "avcodec_open2 fail";
    }
    else {
        qDebug() << "avcodec_open2 success";
    }
    //9、写入头部信息
    res = avformat_write_header(this->pformatContext, nullptr);
    if(res != 0)
    {
         qDebug() << "avformat_write_header fail";
    }
    else {
        qDebug() << "avformat_write_header success";
    }

}
//avframe--->avpacket
//将解码avframe yuv420 像素数据 变成avpacket 要写到save2.h264中   avframe--->avpacket  encondeFrameToH264
int CEncodeToh264::encondeFrameToH264(AVFrame *pictureYUV) {

}

 void CEncodeToh264::writeTailer()
 {
     //11、写入尾帧
     av_write_trailer(pformatContext);
     // 释放帧列表
     for(AVFrame* frame : avframeList) {
         av_frame_free(&frame);
     }
     avframeList.clear();
     //关闭输入流
     avio_close(pformatContext->pb);
     avformat_free_context(pformatContext);
     av_packet_free(&this->pkt);

 }

 void CEncodeToh264::getWidthAndHeight(int width, int height)
 {
     this->width = width;
     this->height = height;

 }

 void CEncodeToh264::addFrame(AVFrame *frame)
 {
     QMutexLocker locker(&m_frameMutex);
     if (pictureYUV) av_frame_free(&pictureYUV);
     pictureYUV = av_frame_clone(frame); // 深拷贝帧数据
 }

 void CEncodeToh264::generDetsH264()
 {


 }

 // In the run() method:
 void CEncodeToh264::run()
 {
     while (!isInterruptionRequested())
     {
         AVFrame *frame = nullptr;
         {
             QMutexLocker locker(&m_frameMutex);
             if (!pictureYUV) {
                 QThread::msleep(1); // Reduce sleep time
                 continue;
             }
             frame = av_frame_clone(pictureYUV); // Clone while protected by mutex
             av_frame_free(&pictureYUV); // Free the original
         }

         if (!pcoderContext || !frame) {
             if (frame) av_frame_free(&frame);
             continue;
         }

         // Validate frame parameters
         if (frame->format != pcoderContext->pix_fmt ||
             frame->width != pcoderContext->width ||
             frame->height != pcoderContext->height) {
             qDebug() << "Frame format/size mismatch!";
             av_frame_free(&frame);
             continue;
         }

         frame->pts = pkt_index++;

         // Send frame to encoder
         int res = avcodec_send_frame(pcoderContext, frame);
         av_frame_free(&frame); // Free frame immediately after sending

         if (res < 0) {
             if (res != AVERROR(EAGAIN)) {
                 char errbuf[AV_ERROR_MAX_STRING_SIZE];
                 av_make_error_string(errbuf, sizeof(errbuf), res);
                 qDebug() << "avcodec_send_frame failed:" << errbuf;
             }
             continue;
         }

         // Receive packets
         while (true) {
             res = avcodec_receive_packet(pcoderContext, pkt);
             if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
                 break;
             }
             if (res < 0) {
                 qDebug() << "avcodec_receive_packet failed";
                 break;
             }

             if (av_interleaved_write_frame(pformatContext, pkt) < 0) {
                 qDebug() << "Failed to write packet";
             }
             av_packet_unref(pkt);
         }
     }
     this->writeTailer();
 }

 void CEncodeToh264::setFilename(QString fileName)
 {
     this->fileName = fileName;
 }
