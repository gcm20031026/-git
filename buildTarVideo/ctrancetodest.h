#ifndef CTRANCETODEST_H
#define CTRANCETODEST_H
#include<QString>
#include<QThread>
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
class CTranceToDest : public QThread
{
public:
    // 构造转封装线程，指定源 H.264 文件和目标文件。
    CTranceToDest(QString fileName,QString fileH264);
    //注册
    void registerTrance();  // 注册 FFmpeg 相关组件。
    int openFileGetStream(); // 打开源文件并查找视频流。
    int initTrance();  // 初始化目标封装格式和输出流。
    void tranceToDest(); // 执行转封装过程。
    void generateDestFile(); // 生成最终目标视频文件。
    void setFlag(bool flag); // 设置线程转封装开关。
    void setOutputFile(QString fileName); // 设置输出文件路径。
private:
    AVFormatContext *pformat,*pformatdest;   //视频上下文封装结构体   源：pformat    目标：pformatdest
    int video_index;
    AVStream *newStream;  //目标文件的流
    AVPacket *pkt;      //码流数据
    int framecount;  //帧数
    QString fileH264;
    QString fileName;
    bool flag = false;
protected:
    void run() override;  // 转封装线程入口。
};

#endif // CTRANCETODEST_H
