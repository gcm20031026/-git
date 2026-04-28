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
    explicit ReplayWin(QWidget *parent = nullptr);
    void initialize_control();
    void addControl_control();
    void initialize_camera(int CameraNum);
    void addList_camera();
    void delete_camera(QPushButton *Button);
    void add_camera(QPushButton *Button);
    void judgmentClick(QPushButton *Button);
    void clearReplayViews();
    void layoutReplayViews();
    void refreshReplayRecordList();
    void playRecordAt(int index);
    void addList_WatchList_Btn();
    void paintEvent(QPaintEvent *event);
    void beginAndStop();
    void Redraw(QImage Redraw_img, QLabel *Redraw_nowview);

signals:
    void camerasNum_change(QList<QPushButton*>);

public slots:
    void showImage(QImage, QLabel*);
    void updateProgress(int value);
    void seekProgress();
    void updatePlaybackSpeed(int index);
    void refreshReplayRecordListSlot();
    void receiveRetrievalwin_saveFileNameAndcameraName(QStringList fileName, QStringList cameraName);
};

#endif // REPLAYWIN_H