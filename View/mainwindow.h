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
    QHBoxLayout *demoQHBoxLayout,*custom_titleLayout,*systemOperationLayout,*downMainLayout,*vedioWinLayout;
    QVBoxLayout *demoQVBoxLayout,*mainLayout,*listWinLayout;
    QGridLayout *demoQGridLayout;
    QWidget *demoQWidget,*custom_title_Widget,*downMain_Widget,*systemOperation_Widget,*listWin_Widget,*vedioWin_Widget;
    QPushButton *demoQPushButton,*miniminze_btn,*fullScreen_btn,*close_btn,*mainButton01,*mainButton02,*mainButton03,*mainButton04;
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
    QPushButton *btn01,*btn02,*btn03,*btn04,*btn05,*btn06,*btn07,*btn08,*btn09,*btn10,*btn11;
    QList<plyaViewWidget*>widgets;

    QPushButton* currentSelectedMainButton = nullptr;  // 跟踪当前选中的主按钮
    QWidget *trackingMainWidget=nullptr; // 跟踪当前界面

    QVector<QPair<int,int>>channel_size;//通道大小
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void initialize_window(); //初始化窗口
    void addControl_window(); //主窗口控件添加
    void initialize_control(); //初始化控件
    void addControl_control(); //控件添加
    void setWindowLocationAndSize(int,int,int,int); //设置本窗口位置和大小
    bool eventFilter(QObject *obj, QEvent *event); //重写监控函数
    void initAddWidgets();//初始化16通道到链表
    void hide_video_all();//隐藏所有通道
    void setMainButtonSelected(QPushButton* button, bool selected);
    void pageTtansition(QPushButton* Button); //页面切换
    QWidget* buttonCorrespondsToInterface(QPushButton* Button); //返回按钮对应的界面
    void initChanelSize();//初始化通道大小
    int addjudetchannel(int row,int col);//判断x*x的通道
    void replaceWidgetInGrid(QGridLayout* grid, QWidget* oldWidget, QWidget* newWidget);//切换设备
    void btnConnect();
    void resizeEvent(QResizeEvent *event); //窗口大小变化事件
    int currentRow = 2; // 当前通道行数
    int currentCol = 2; // 当前通道列数

signals:
    void sentChangeChanleSize();
public slots:
    void miniminze(); //窗口最小化
    void fullScreen(); //窗口全屏
    void changeCountTo(int row,int col);//转通道
    void switchDev(QString devName,QString targetDevName);//切换设备显示
    void singleShow();

};

#endif // MAINWINDOW_H
