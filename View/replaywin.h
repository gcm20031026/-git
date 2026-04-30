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
#include <QComboBox>
#include <QSlider>
#include <QList>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <QDir>
#include <playThread/ffmpegtorgbkun.h>
#include "data/qmysqlite.h"

class ReplayWin : public QWidget
{
    Q_OBJECT
private:
    QFont font01, font02;
    QHBoxLayout *mainLayout, *progressBarButtonLayout, *vedio_Widget_NOHLayout;
    QVBoxLayout *listWinLayout, *scrollAreaWidgetLayout, *cameraListLayout, *chanleViewLayout, *progressBarLayout, *vedio_Widget_NOVLayout;
    QGridLayout *vedioLayout;
    QWidget *listWin_Widget, *vedioWin_Widget, *scrollArea_Widget, *listItem_Widget, *vedio_Widget_NO, *vedio_Widget_NOWin, *vedio_Widget, *progressBar_Widget, *progressBarButton_Widget;
    QScrollArea *cameraArea_scrollArea;
    QPushButton *btn01, *beginAndStopBtn;
    QComboBox *speedComboBox;
    QCalendarWidget *qCalendarWidgetTime;
    QSlider *progressBar;
    QList<QPushButton*> Buttons;
    QList<QWidget*> WatchList_QWidget;
    QList<QPushButton*> WatchList_QWidget_Buttons;
    QList<QLabel*> Videos;
    QLabel *noCameraLabel;
    QLabel *recordListHintLabel;
    QImage img;
    QList<ffmpegToRGBKun*> pts;
    QList<QLabel*> beginVideos;
    QStringList currentReplayFileName;
    QStringList currentReplayCameraName;
    QVector<VideoRecord> replayRecordList;
    bool beginAndStopBool = false;
    bool replayFinished = false;
    bool progressSliderPressed = false;
    double replaySpeed = 1.0;
    QLabel *nowview;

public:
    // 构造录像回放页面。
    explicit ReplayWin(QWidget *parent = nullptr);
    // 初始化回放页控件。
    void initialize_control();
    // 组装回放页控件布局。
    void addControl_control();
    void applyModernUi();
    // 根据摄像头数量初始化左侧摄像头列表。
    void initialize_camera(int CameraNum);
    // 将摄像头列表添加到滚动区域。
    void addList_camera();
    // 从回放选择列表中移除摄像头。
    void delete_camera(QPushButton *Button);
    // 将摄像头加入回放选择列表。
    void add_camera(QPushButton *Button);
    // 判断摄像头按钮点击后应添加还是移除。
    void judgmentClick(QPushButton *Button);
    // 清空当前回放画面和播放线程。
    void clearReplayViews();
    // 根据当前录像数量重新布局回放画面。
    void layoutReplayViews();
    // 从数据库刷新录像记录列表。
    void refreshReplayRecordList();
    // 播放指定索引的录像记录。
    void playRecordAt(int index);
    // 生成已选择摄像头的按钮列表。
    void addList_WatchList_Btn();
    // 绘制回放页自定义背景或辅助内容。
    void paintEvent(QPaintEvent *event);
    // 控制回放开始或暂停。
    void beginAndStop();
    // 将回放线程输出的图像绘制到指定标签。
    void Redraw(QImage Redraw_img, QLabel *Redraw_nowview);

signals:
    // 摄像头选择数量变化时通知检索页面。
    void camerasNum_change(QList<QPushButton*>);

public slots:
    // 接收播放线程输出图像并刷新显示。
    void showImage(QImage, QLabel*);
    // 根据播放进度更新滑块。
    void updateProgress(int value);
    // 用户拖动进度条后执行跳转。
    void seekProgress();
    // 根据倍速下拉框更新播放速度。
    void updatePlaybackSpeed(int index);
    // 槽函数形式刷新录像记录列表。
    void refreshReplayRecordListSlot();
    // 接收检索页传来的文件名和摄像头名并进入回放。
    void receiveRetrievalwin_saveFileNameAndcameraName(QStringList fileName, QStringList cameraName);
};

#endif // REPLAYWIN_H
