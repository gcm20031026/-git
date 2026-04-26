#ifndef CENCODETOH264_H
#define CENCODETOH264_H
#include<QString>
#include<QList>
#include<QThread>
#include<QMutex>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavdevice/avdevice.h"
    #include "libavfilter/avfilter.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"
    #include "libswscale/swscale.h"
}

class CEncodeToh264 :public QThread
{
public:
    CEncodeToh264(QString fileName);
    //编码的初始化
    void initEncode();
    //将每一帧的像素数据AVFrame压缩成AVPacket
    int encondeFrameToH264(AVFrame *pictureYUV);  //不成功返回-1
    //写入尾帧
    void writeTailer();
    //获取宽度和高度
    void getWidthAndHeight(int width,int height);
    void addFrame(AVFrame *frame);
    //生成文件
    void generDetsH264();
    void run() override;
    void setFilename(QString fileName);
private:
    AVOutputFormat *poutformat;
    AVFormatContext *pformatContext;  //上下文封装结构体
    AVStream *newStream;  //新的视频流
    AVCodecContext *pcoderContext;  //编解码器
    AVCodec *pencorder; //编码器
    int pkt_index;//时间基位置
    AVPacket *pkt;  //放码流数据
    int width;
    int height;
    QString fileName;//文件路径和名字
    QList <AVFrame*>avframeList;
    AVFrame *pictureYUV=nullptr;
    QMutex m_frameMutex;
};

#endif // CENCODETOH264_H
