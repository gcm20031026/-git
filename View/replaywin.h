#ifndef REPLAYWIN_H
#define REPLAYWIN_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QCalendarWidget>
#include <QSlider>
#include <QList>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <playThread/ffmpegtorgbkun.h>
#include <QMessageBox>
#include <QDir>

class ReplayWin : public QWidget //回放窗口
{
    Q_OBJECT
private:
    QFont font01,font02;
    QHBoxLayout *mainLayout,*progressBarButtonLayout,*vedio_Widget_NOHLayout;
    QVBoxLayout *listWinLayout,*scrollAreaWidgetLayout,*cameraListLayout,*chanleViewLayout,*progressBarLayout,*vedio_Widget_NOVLayout;
    QGridLayout *vedioLayout;
    QWidget *listWin_Widget,*vedioWin_Widget,*scrollArea_Widget,*listItem_Widget,*vedio_Widget_NO,*vedio_Widget_NOWin,*vedio_Widget,*progressBar_Widget,*progressBarButton_Widget;
    QScrollArea *cameraArea_scrollArea;
    QPushButton *btn01,*beginAndStopBtn;
    QCalendarWidget *qCalendarWidgetTime;
    QSlider *progressBar;
    QList<QPushButton*>Buttons;
    QList<QWidget*>WatchList_QWidget;
    QList<QPushButton*>WatchList_QWidget_Buttons;
    QList<QLabel*>Videos;
    QLabel *noCameraLabel;
    QImage img;
    //ffmpegToRGBKun *pt;
    QList<ffmpegToRGBKun*> pts;
    QList<QLabel*>beginVideos;
    bool beginAndStopBool=false;
    QLabel *nowview;
public:
    explicit ReplayWin(QWidget *parent = nullptr);
    void initialize_control();
    void addControl_control();
    void initialize_camera(int CameraNum); //初始化摄像头
    void addList_camera(); //添加摄像头按钮
    void delete_camera(QPushButton *Button); //去除摄像头(自动释放控件内存)
    void add_camera(QPushButton *Button); //添加摄像头
    void judgmentClick(QPushButton *Button);
    void addList_WatchList_Btn(); //添加播放列表(切按钮)
    void paintEvent(QPaintEvent *event); //使用的是自己创建的this->img、this->nowview单缓冲空间(多个视频线程卡顿！！！可能出现绘制错乱！！！)
    void beginAndStop();
    void Redraw(QImage Redraw_img,QLabel *Redraw_nowview); //这个函数用来代替paintEvent(QPaintEvent *event)函数用于绘制图像，原因：\n
    //因为：如果想要同时播放多个图像就要创建多个线程，他们是参差不齐的发送数据，如果采用老方法，收到一帧的图像就只能等上一帧绘制完毕再绘制下一帧\n
    //而且每次接收到图像就得修改this->img和this->nowview这两个图像定位缓冲区，也只能等上一个缓冲区中数据绘制完毕才能下一个，这样就会非常卡顿，还可能出现图像绘制位置混淆的问题\n
    //对于多个图像，多线程图像也得使用多线程绘制函数，每次接收到数据就调用这个函数，而且函数多态的属性也使他不用等待上一个数据的处理即可再次调用，一个函数可以在同一时间重复调用并行处理\n
    //并且这个函数可以直接传入图片和位置参数，就不用每次修改缓冲区了，每个线程的每个属于都有独立的形参缓冲区，这样就可以流畅的播放多个视频了！！！\n
    //注意：这个重绘函数也得配合updata()函数使用，不然程序会卡死，不知道为什么？？？
signals:
    void camerasNum_change(QList<QPushButton*>);
public slots:
    void showImage(QImage,QLabel*); //接收图片
    void receiveRetrievalwin_saveFileNameAndcameraName(QStringList fileName,QStringList cameraName); //接收要播放的文件路径名和摄像头名
};

#endif // REPLAYWIN_H
