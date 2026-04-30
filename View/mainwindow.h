#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QIcon>
#include <QHBoxLayout> //水平布局
#include <QVBoxLayout> //垂直布局
#include <QGridLayout> //网格布局
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QCalendarWidget>
#include <QPixmap>
#include <QFont>
#include <QScrollArea>
#include<QList>
#include<view/plyaviewwidget.h>
#include <view/replaywin.h> //回放窗口
#include <view/retrievalwin.h> //检索窗口
#include <view/configurationwin.h> //配置窗口
#include<QVector>
#include <QPair>
#include <view/alarmwin.h>
class MainWindow : public QWidget
{
    Q_OBJECT
private:
    //窗口
    QWidget *PreviewWin=nullptr; //预览界面（用来跟踪判断是否处于预览主界面的，不用开空间）
    ReplayWin *replayWin; //回放窗口
    RetrievalWin *retrievalWin; //检索窗口
    ConfigurationWin *configurationWin; //配置窗口
    //窗口控件
    AlarmWin *alarmWin;
    QHBoxLayout *demoQHBoxLayout,*custom_titleLayout,*systemOperationLayout,*downMainLayout,*vedioWinLayout;
    QVBoxLayout *demoQVBoxLayout,*mainLayout,*listWinLayout;
    QGridLayout *demoQGridLayout;
    QWidget *demoQWidget,*custom_title_Widget,*downMain_Widget,*systemOperation_Widget,*listWin_Widget,*vedioWin_Widget;
    QPushButton *demoQPushButton,*miniminze_btn,*fullScreen_btn,*close_btn,*mainButton01,*mainButton02,*mainButton03,*mainButton04,*mainButton05;
    QLabel *demoQLabel,*logo;
    QLineEdit *demoQLineEdit;
    QPoint m_dragPosition; //鼠标位置
    bool fullScreen_bool=false; //是否全屏
    //界面控件
    QCalendarWidget *qCalendarWidgetTime;
    QHBoxLayout *PlaybackButtonLayout,*chanleHLayout;
    QVBoxLayout *scrollAreaWidgetLayout,*cameraListLayout;
    QGridLayout *ggg,*chanleViewLayout;
    QWidget *PlaybackButton_Widget,*scrollArea_Widget,*listItem_Widget,*chanl_Widget;
    QPushButton *regularPlayback_btn,*eventPlayback_btn;
    QScrollArea *cameraArea_scrollArea;
    QPushButton *chanel_Btn1,*chanel_Btn2,*chanel_Btn4,*chanel_Btn8,*chanel_Btn9,*chanel_Btn13,*chanel_Btn16;

    //测试用
    QPushButton *btn01,*btn02,*btn03,*btn04,*btn05,*btn06,*btn07,*btn08,*btn09,*btn10,*btn11,*btn12,*btn13,*btn14,*btn15,*btn16;
    QList<plyaViewWidget*>widgets;

    QPushButton* currentSelectedMainButton = nullptr;  // 跟踪当前选中的主按钮
    QWidget *trackingMainWidget=nullptr; // 跟踪当前界面

    QVector<QPair<int,int>>channel_size;//通道大小
public:
    // 构造主窗口并初始化系统各页面。
    explicit MainWindow(QWidget *parent = nullptr);
    // 初始化主窗口基础属性和无边框窗口行为。
    void initialize_window(); //初始化窗口
    // 将主窗口控件加入整体布局。
    void addControl_window(); //主窗口控件添加
    // 初始化导航栏、预览区、回放区等控件。
    void initialize_control(); //初始化控件
    // 组装主界面控件布局。
    void addControl_control(); //控件添加
    // 设置窗口初始位置和大小。
    void setWindowLocationAndSize(int,int,int,int); //设置本窗口位置和大小
    // 处理窗口拖动、按钮事件等全局事件过滤。
    bool eventFilter(QObject *obj, QEvent *event); //重写监控函数
    // 初始化 16 路预览通道控件。
    void initAddWidgets();//初始化16通道到链表
    // 隐藏所有预览通道。
    void hide_video_all();//隐藏所有通道
    // 设置主导航按钮的选中样式。
    void setMainButtonSelected(QPushButton* button, bool selected);
    // 根据导航按钮切换页面。
    void pageTtansition(QPushButton* Button); //页面切换
    // 返回导航按钮对应的功能页面。
    QWidget* buttonCorrespondsToInterface(QPushButton* Button); //返回按钮对应的界面
    // 初始化通道分屏模式列表。
    void initChanelSize();//初始化通道大小
    // 根据行列数计算需要显示的通道数量。
    int addjudetchannel(int row,int col);//判断x*x的通道
    // 在网格布局中替换指定通道控件。
    void replaceWidgetInGrid(QGridLayout* grid, QWidget* oldWidget, QWidget* newWidget);//切换设备
    // 连接主窗口按钮和通道控制信号。
    void btnConnect();
    void applyModernUi();
    // 窗口尺寸变化时重新调整预览通道大小。
    void resizeEvent(QResizeEvent *event); //窗口大小变化事件
    int currentRow = 2; // 当前通道行数
    int currentCol = 2; // 当前通道列数

signals:
    // 通知所有预览通道重新计算显示尺寸。
    void sentChangeChanleSize();
public slots:
    // 最小化主窗口。
    void miniminze(); //窗口最小化
    // 全屏或退出全屏。
    void fullScreen(); //窗口全屏
    // 切换预览分屏行列数。
    void changeCountTo(int row,int col);//转通道
    // 在两个通道之间切换设备显示。
    void switchDev(QString devName,QString targetDevName);//切换设备显示
    // 左侧摄像头按钮触发单通道显示。
    void singleShow();
    // 双击预览通道后切换为该通道的 1x1 显示。
    void showSingleChannel(int index);

};

#endif // MAINWINDOW_H
