#ifndef PLYAVIEWWIDGET_H
#define PLYAVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QVBoxLayout>
#include <QString>
#include <playThread/decodethread.h>
// #include<data/qmysqlite.h>
#include <QEvent>
#include <QMenu>
#include <QAction>
#include "Control/mycamera.h"
#include "playThread/ffmpegwriterdest.h"
#include "buildTarVideo/opencvdetect.h"
class plyaViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit plyaViewWidget(QWidget *parent, int id, QString devName);
    void setTimeLable();                      // 显示时间
    void createMenu();                        // 创建菜单
    void enterEvent(QEvent *event);           // 鼠标进入
    void leaveEvent(QEvent *event);           // 鼠标离开时隐藏提示
    void mousePressEvent(QMouseEvent *event); // 鼠标右击事件
    void showCamera();
    bool isCameraAvailable() const;
    ~plyaViewWidget();

public slots:
    void updateTime();       // 更新时间
    void getImg(QImage img); // 获取照片流
    void changeSize();
    void SlotMenuClicked(QAction *action);    // 右击菜单选择
    void addMenuSwitchItem();                 // 添加切换菜单项
    void SlotMenuDevClicked(QAction *action); // 点击切换的设备

signals:
    void sent_X_Y(int x, int y);
    void sentSwichDev(QString curDevName, QString targetDevName);
    void savePicture(QString picName);
    void startStopDetectfaceSignal(bool detectFaceFlag);
    void saveVideosSignal();
    void setTimeSignal(QString m_time);

private:
    QLabel *timeLabel, *videoPlay;
    QTimer *timer;
    QVBoxLayout *mainLayout, *videoLayout;
    QString devName; // 设备名字
    DecodeThread *video = nullptr;
    QWidget *videoArea;
    int x, y;
    // 右键菜单
    QMenu *menuRD;
    QMenu *changeMenu;
    QAction *m_pActionChange;
    QAction *m_pActionClose;
    QAction *m_pActionScreenshot;
    QAction *detectFace, *saveVideos;
    QString path = "./savaPicture"; // 保存路径
    QImage scrImg;                  // 截图照片
    bool playFlag = true;
    bool cameraAvailable = false;
    int id;
    MyCamera *myCamera;
    // 创建写入器
    FFmpegWriterDest *writer;
    openCVDetect *detFace;       // 人脸检测
    bool detectFaceFlag = false; // 是否开启人脸检测
};

#endif // PLYAVIEWWIDGET_H
