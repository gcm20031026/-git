#include "ctrancetodest.h"
#include <QDebug>
CTranceToDest::CTranceToDest(QString fileName,QString fileH264)
{
    this->fileName = fileName;
    this->fileH264 = fileH264;
    registerTrance();
    video_index = -1;
    framecount = 0;

}

void CTranceToDest::registerTrance()
{
    //注册组件
    av_register_all();

}
/**打开h264文件，查找文件流  解码过程  ---h264文件--  avpacket  写入到文件中
 * @brief CTranceToDest::openFileGetStream
 * @return
 */
int CTranceToDest::openFileGetStream()  //解码过程
{
    //1、解码操作，输入源文件   源文件上下文封装结构体   源文件  准备工作
    pformat = avformat_alloc_context();//源文件 上下文封装结构体
    //1、打开视频
    int res = avformat_open_input(&pformat,fileH264.toUtf8(),nullptr,nullptr);
    if(res !=0 )
    {
        qDebug() << "avformat_open_input fail";
        return -1;
    }
    else {
        qDebug() << "avformat_open_input sucess";
    }
    //2、查找流媒体数据   //
    res = avformat_find_stream_info(pformat,nullptr);
    if(res !=0 )
    {
        qDebug() << "avformat_find_stream_info fail";
        return -1;
    }
    else {
        qDebug() << "avformat_find_stream_info sucess";
    }
    //3、查找流媒体数据
    for(int i=0;i<pformat->nb_streams;i++)  //nb_streams   视频   音频    字幕
    {
        if(pformat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)   //i = 0  视频流是保存在第一个元素
        {
            video_index = i;
            break;
        }
    }
    //
    if(video_index == -1)
    {
        qDebug() << "avformat_open_input find video fail";
        return -1;
    }
    else {
        qDebug() << "avformat_open_input find video sucess";
    }
    return 0;
}
/**初始化目标内容 test.mp4   编码过程
 * @brief CTranceToDest::initTrance
 * @return
 */
int CTranceToDest::initTrance()
{
    pkt = av_packet_alloc();  //初始化  开空间

    //1、猜测格式  h264
    AVOutputFormat *poutformat;  //输出格式
    poutformat = av_guess_format(nullptr,fileName.toUtf8(),nullptr);//生成文件的名字
    if(poutformat == nullptr)
    {
        qDebug() << "av_guess_format fail";
        return -1;
    }
    else {
        qDebug() << "av_guess_format sucess";
    }
    //2、输出文件（目标文件）封装格式上下文结构体设置输出格式
    pformatdest = avformat_alloc_context();   //目标文件上下文封装结构  新的视频
    pformatdest->oformat = poutformat;
    //3、打开文件流，
    int res = avio_open(&pformatdest->pb,fileName.toUtf8(),AVIO_FLAG_WRITE);//bin
    if(res == 0)
    {
        qDebug() << "avio_open sucess";
    }
    else {
        qDebug() << "avio_open fail";
        return -1;
    }
    //4、新建视频流
    newStream =  avformat_new_stream(pformatdest,nullptr);  //save.h264  --》 test.wmv
    if(nullptr != newStream)
    {
        qDebug() << "avformat_new_stream sucess";
    }
    else {
        qDebug() << "avformat_new_stream fail";
        return -1;
    }
    //5、 参数设置  codecpar:编解码器的参数
    res = avcodec_parameters_copy(newStream->codecpar,pformat->streams[video_index]->codecpar);
    if( res == 0)
    {
        qDebug() << "avcodec_parameters_copy sucess";
    }
    else {
        qDebug() << "avcodec_parameters_copy fail";
        return -1;
    }
    //6、写入头部文件信息
    res = avformat_write_header(pformatdest,nullptr);
    if( res == 0)
    {
        qDebug() << "avformat_write_header sucess";
    }
    else {
        qDebug() << "avformat_write_header fail";
        return -1;
    }
    return 0;
}
/**逐帧解码  转码
 * @brief CTranceToDest::tranceToDest
 */
void CTranceToDest::tranceToDest()   //pformat---save.h264 源   pformatdest---test.mp4 目标
{

    while(av_read_frame(pformat,pkt)==0)  // 解封装   码流数据放在avpacket   pformat：源 h264  pkt
    {
        if(pkt->stream_index != video_index) //如果不是视频流  习惯 video_index=0
        {
            continue;
        }
        framecount++;  //帧数 0  1
        //
        if(pkt->pts == AV_NOPTS_VALUE)  //pts是显示时间基
        {                                                                                                    //时间基的转换
            AVRational time_base1=pformat->streams[video_index]->time_base;// 源文件的时间基
            int64_t duration=(double)AV_TIME_BASE/
            av_q2d(pformat->streams[video_index]->r_frame_rate);  //帧率    1/9
            pkt->pts=(double)(framecount*duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
            pkt->dts=pkt->pts;
            pkt->duration=duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
        }

        else if(pkt->pts < pkt->dts)
        {
            continue;
        }

        //时间基的转换：参数 1：要换算的值；参数 2：原来的时间基，参数 3：现在的时间基/*  */
        //显示时间基的转化
        pkt->pts = av_rescale_q_rnd(pkt->pts, pformat->streams[video_index]->time_base,newStream->time_base, (AVRounding)(AV_ROUND_INF|AV_ROUND_PASS_MINMAX));
        //解码时间基的转换

        pkt->dts=av_rescale_q_rnd(pkt->dts,
                                    pformat->streams[video_index]->time_base,
                                    newStream->time_base,
                                    (AVRounding)(AV_ROUND_INF|AV_ROUND_PASS_MINMAX)
                              );
        //数据的时长，以所属媒体流的时间基准为单位 1s，未知则值为默认值0
        pkt->duration=av_rescale_q(pkt->duration, pformat->streams[video_index]->time_base, newStream->time_base);
        pkt->pos=-1;
        //标识，结合AV_PKT_FLAG使用，其中最低为1表示该数据是一个关键帧
        //这个标志用于指示当前数据包是否为关键帧（I帧
        pkt->flags|=AV_PKT_FLAG_KEY;
        //packet在stream的index位置
        pkt->stream_index=0;
        //写入数据到输出视频信息结构体汇总
        av_interleaved_write_frame(pformatdest,pkt);// 码流数据进行封装
        av_packet_unref(pkt);
    }
    //写入尾帧
    av_write_trailer(pformatdest);
    //关闭输出流
    avio_close(pformatdest->pb);
    //释放
    avformat_close_input(&pformat);
    av_free(pformat);
    av_free(pformatdest);

}

void CTranceToDest::generateDestFile()
{
    openFileGetStream();
    initTrance();
    tranceToDest();
}

void CTranceToDest::setFlag(bool flag)
{
    this->flag = flag;
}

void CTranceToDest::setOutputFile(QString fileName)
{
    this->fileName = fileName;
}

void CTranceToDest::run()
{
    while (true)
    {
        if(flag)
        {
            generateDestFile();
            flag= false;
        }
    }
}
