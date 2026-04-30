#ifndef PLYAVIEWWIDGET_H
#define PLYAVIEWWIDGET_H

#include <QAction>
#include <QDateTime>
#include <QEvent>
#include <QLabel>
#include <QMenu>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include "Control/mycamera.h"
#include "buildTarVideo/opencvdetect.h"
#include "playThread/decodethread.h"
#include "playThread/ffmpegwriterdest.h"

class plyaViewWidget : public QWidget
{
    Q_OBJECT
public:
    // 构造单个视频预览通道控件。
    explicit plyaViewWidget(QWidget *parent, int id, QString devName);
    // 初始化并启动通道顶部时间显示。
    void setTimeLable();
    // 创建右键菜单。
    void createMenu();
    // 鼠标进入通道时高亮边框。
    void enterEvent(QEvent *event);
    // 鼠标离开通道时恢复边框。
    void leaveEvent(QEvent *event);
    // 处理右键菜单弹出。
    void mousePressEvent(QMouseEvent *event);
    // 双击通道时通知主窗口切换为单通道显示。
    void mouseDoubleClickEvent(QMouseEvent *event);
    // 根据通道编号打开真实摄像头或模拟视频。
    void showCamera();
    // 需要显示时再延迟启动预览，降低 16 通道启动压力。
    void startPreviewIfNeeded();
    // 返回当前通道是否有可用输入源。
    bool isCameraAvailable() const;
    // 释放通道相关资源。
    ~plyaViewWidget();

public slots:
    // 更新时间标签。
    void updateTime();
    // 接收解码线程图像并显示。
    void getImg(QImage img);
    // 根据主窗口当前布局调整视频显示尺寸。
    void changeSize();
    // 处理右键菜单动作。
    void SlotMenuClicked(QAction *action);
    // 添加可切换的摄像头菜单项。
    void addMenuSwitchItem();
    // 处理切换摄像头菜单动作。
    void SlotMenuDevClicked(QAction *action);
    // 同步人脸检测开关状态。
    void setFaceDetectEnabled(bool enabled);

signals:
    // 发送当前通道显示尺寸。
    void sent_X_Y(int x, int y);
    // 通知主窗口执行通道设备切换。
    void sentSwichDev(QString curDevName, QString targetDevName);
    // 请求保存当前截图。
    void savePicture(QString picName);
    // 请求解码线程开启或关闭人脸检测。
    void startStopDetectfaceSignal(bool detectFaceFlag);
    // 请求保存当前通道视频。
    void saveVideosSignal();
    // 传递录制时间字符串。
    void setTimeSignal(QString m_time);
    // 通道被双击时发出通道索引。
    void doubleClicked(int index);
    void alarmRaised(const QString &level, const QString &source, const QString &type);

private:
    QLabel *timeLabel, *videoPlay;
    QTimer *timer;
    QVBoxLayout *mainLayout, *videoLayout;
    QString devName;
    DecodeThread *video = nullptr;
    QWidget *videoArea;
    int x, y;
    QMenu *menuRD;
    QMenu *changeMenu;
    QAction *m_pActionChange;
    QAction *m_pActionClose;
    QAction *m_pActionScreenshot;
    QAction *detectFace, *saveVideos;
    QString path = "./savaPicture";
    QImage scrImg;
    bool playFlag = true;
    bool cameraAvailable = false;
    bool previewStarted = false;
    int id;
    MyCamera *myCamera;
    FFmpegWriterDest *writer;
    openCVDetect *detFace;
    bool detectFaceFlag = false;
};

#endif // PLYAVIEWWIDGET_H
