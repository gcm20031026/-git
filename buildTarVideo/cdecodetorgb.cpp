#include "cdecodetorgb.h"
#include<QDebug>
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
CDecodeToRGB::CDecodeToRGB()
{


    //1注册组件
    av_register_all();
    //视频封装上下文
    AVFormatContext *VPContext=nullptr;
    //2打开视频
    int res = avformat_open_input(&VPContext,"./image/xiong.mp4",nullptr,nullptr);
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
    //流的类型 视频流 音频流 字幕流
    int index=-1;
    for(int i=0;i<VPContext->nb_streams;++i)
    {
        //视频流
        if(VPContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
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
    AVCodecContext *Vcodecontext = VPContext->streams[index]->codec;
    //6找到流对应的编解码器
    AVCodec *vDecoder = avcodec_find_decoder(Vcodecontext->codec_id);
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
    //8创建AVPacket AVPacket：解析文件时会将音/视频帧读入到packet中
    AVPacket *packet = nullptr;
    //分配AVPacket结构体的内存,每一个packet大小（码流数据大小）
    packet = (AVPacket*)malloc(sizeof(AVPacket));
    //每一帧图片的大小，解码器的宽度和高度
    int size = Vcodecontext->width * Vcodecontext->height;
    av_new_packet(packet,size); //packet包
    //AVFrame用于保存数据帧的数据结构，AVFrame 用于保存转换之后的帧
    AVFrame *picture; //picture图片
    AVFrame *pictureRGB;
    picture = av_frame_alloc();//需要这个空间用来填充一帧AVFrame
    pictureRGB = av_frame_alloc();//这个空间用来填充一帧的RGB
    //需要计算一下，填充一帧RGB格式需要多大空间
    int numbytes = avpicture_get_size(AV_PIX_FMT_RGB32,Vcodecontext->width,Vcodecontext->height);
    //申请用来填充一帧RGB32图片需要的空间
    //缓冲区分配内存
    uint8_t *bufferRGB = (uint8_t * )av_malloc(numbytes*sizeof(uint8_t));
    //填充一张RGB32图片
    //初始化缓冲区 类似于内存的memset-开辟完清理操作
    avpicture_fill((AVPicture*)pictureRGB,bufferRGB,AV_PIX_FMT_RGB32,Vcodecontext->width,Vcodecontext->height);
    //转置规则
    struct SwsContext *sws = nullptr; //SwsContext:转换器
    /*srcW, srcH	输入图像的宽和高
    srcFormat	输入图像的像素格式（如 AV_PIX_FMT_YUV420P）
    dstW, dstH	输出图像的宽和高（可缩放）
    dstFormat	输出图像的像素格式（如 AV_PIX_FMT_RGB32）
    flags	缩放算法标志（如 SWS_BILINEAR）
    srcFilter, dstFilter	源/目标图像的滤波器（通常设为 NULL）
    param	算法参数（通常设为 NULL）*/
    sws = sws_getContext(
    Vcodecontext->width, Vcodecontext->height, Vcodecontext->pix_fmt,Vcodecontext->width, Vcodecontext->height,
                AV_PIX_FMT_RGB32,SWS_BICUBIC,nullptr, nullptr, nullptr);
    //7、读码流数据
    int num =0;
    QString path;
    while(av_read_frame(VPContext,packet)==0)//将视频一帧一帧读出来，放在码流数据中。avpacket（码流数据） 解封装。
    {
        int ptr = -1;
        if(packet->stream_index == index)//确保是视频的数据，因为里面也有可能有音频。 解码流数据。
         {
            //解码视频数据包
            avcodec_decode_video2(Vcodecontext,picture,&ptr,packet);//将 avpacket ---avframe
            if(ptr!=0)
            {
                //sws_scale用于 图像缩放和像素格式转换
                /*
                    * int sws_scale(
                    struct SwsContext *c,                     // 转换上下文（由 sws_getContext() 创建）
                    const uint8_t *const srcSlice[],         // 输入图像数据（源帧的 data 数组）
                    const int srcStride[],                   // 输入图像的行跨度（源帧的 linesize）
                    int srcSliceY,                           // 起始行（通常为 0）
                    int srcSliceH,                           // 处理的行数（通常为图像高度）
                    uint8_t *const dst[],                    // 输出图像数据（目标帧的 data 数组）
                    const int dstStride[]                    // 输出图像的行跨度（目标帧的 linesize）
                );*/
                sws_scale(sws, (const uint8_t* const*)picture->data,picture->linesize,0,Vcodecontext->height,pictureRGB->data,pictureRGB->linesize);
                QImage img = QImage((uchar*)bufferRGB,Vcodecontext->width,Vcodecontext->height,QImage::Format_RGB32);
                num++;
               path = QString("D:/dfrt_Qt/uiwork/uiwork/videoPlayer/testPicture/xiong%1.jpg").arg(num);
                img.save(path);

            }
        }
    }


}
