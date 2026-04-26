#include "replaywin.h"
#include "math.h"
ReplayWin::ReplayWin(QWidget *parent) : QWidget(parent)
{
    //this->pt= new ffmpegToRGBKun("savaVideos/FaHai.mp4"); //bin下面的目录文件一定要加，不然找不到视频文件
    initialize_control();
    addControl_control();
    //connect(this->pt,SIGNAL(toImg(QImage,QLabel*)),this,SLOT(showImage(QImage,QLabel*)));
    //pt->setbeginAndStopBool(false); //暂停
    //pt->start();
}

void ReplayWin::initialize_control()
{
    //设置字体font01
    this->font01.setFamily("宋体");
    this->font01.setPointSize(12);
    //设置字体font02
    this->font02.setFamily("宋体");
    this->font02.setPointSize(25);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //隐藏系统边框
    this->mainLayout = new QHBoxLayout(); //下方主界面的布局
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);
    this->listWin_Widget = new QWidget(); //左边列表界面
    this->listWin_Widget->setFixedWidth(300);
    this->listWin_Widget->setStyleSheet(
                "background-color: #383838;"
                );
    this->listWinLayout = new QVBoxLayout(); //左边列表布局
    this->listWinLayout->setMargin(0);
    this->listWinLayout->setSpacing(0);
    this->scrollArea_Widget = new QWidget(); //放置不同摄像头滑块的界面
    this->scrollAreaWidgetLayout = new QVBoxLayout(); //放置滑块的布局
    this->scrollAreaWidgetLayout->setMargin(0);
    this->scrollAreaWidgetLayout->setSpacing(0);
    this->cameraArea_scrollArea = new QScrollArea(); //不同摄像头滑块
    this->cameraArea_scrollArea->setWidgetResizable(true);
    this->cameraArea_scrollArea->setStyleSheet(
                "QScrollBar:vertical {"
                "background: #383838;"
                "width: 6px;"
                "}"
                "QScrollBar::handle:vertical {"
                "background: #bfbfbf;"
                "min-height: 10px;"
                "border-radius: 3px;"
                "}"
                "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                "height: 0px;"
                "}"
                "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                "background: none;"
                "}"
                );
    this->listItem_Widget = new QWidget(); //滑块内列表项界面
    this->cameraListLayout = new QVBoxLayout(); //摄像头列表布局
    //-------------先测试，后面使用函数添加摄像头(配置)01-------------
    //初始化摄像头
    initialize_camera(20);
    //日历
    this->qCalendarWidgetTime = new QCalendarWidget();
    this->vedioWin_Widget = new QWidget(); //右边视频界面
    this->vedioWin_Widget->setStyleSheet(
                "background-color: black;"
                );
    this->chanleViewLayout = new QVBoxLayout(); //右边视频界面布局
    this->chanleViewLayout->setMargin(0);
    this->chanleViewLayout->setSpacing(0);
    this->vedio_Widget_NO = new QWidget(); //视频窗口背景版窗体无摄像头时
    this->vedio_Widget_NOVLayout = new QVBoxLayout(); //无摄像头提示垂直布局
    this->vedio_Widget_NOVLayout->setMargin(0);
    this->vedio_Widget_NOVLayout->setSpacing(0);
    this->vedio_Widget_NOWin = new QWidget(); //无摄像头提示依赖窗体
    this->noCameraLabel =new QLabel("当前未选择回放，点击左侧筛选摄像头回放！\n         (前往检索窗口配置回放)"); //无摄像头提示
    this->noCameraLabel->setFont(this->font02);
    this->noCameraLabel->setStyleSheet(
                "QLabel {"
                "color: white;"
                "}"
                );
    this->vedio_Widget_NOHLayout = new QHBoxLayout(); //无摄像头提示水平布局
    this->vedio_Widget_NOHLayout->setMargin(0);
    this->vedio_Widget_NOHLayout->setSpacing(0);
    this->vedio_Widget = new QWidget(); //视频窗口背景版窗体
    this->vedio_Widget->hide(); //开始没有添加摄像头，先隐藏
    this->vedioLayout = new QGridLayout(); //视频布局
    this->vedioLayout->setMargin(0);
    this->vedioLayout->setSpacing(0);
    this->progressBar_Widget = new QWidget(); //进度条窗口
    this->progressBar_Widget->setFixedHeight(50);
    this->progressBar_Widget->setStyleSheet(
                "background-color: #383838;"
                );
    this->progressBarLayout = new QVBoxLayout(); //进度条布局
    this->progressBarLayout->setMargin(0);
    this->progressBarLayout->setSpacing(0);
    this->progressBar = new QSlider(Qt::Horizontal); //进度条
    this->progressBar->setValue(10);  // 设置进度为10%
    this->progressBar->setRange(0, 100); // 进度范围 0~100
    this->progressBar->setStyleSheet(
                "QSlider {"
                "border: none;"
                "}"
                );
    this->progressBarButton_Widget = new QWidget(); //进度条按钮窗口
    this->progressBarButton_Widget->setFixedHeight(35);
    this->progressBarButton_Widget->setStyleSheet(
                "background-color: #888888;"
                );
    this->progressBarButtonLayout = new QHBoxLayout(); //进度条按钮窗口布局
    this->progressBarButtonLayout->setMargin(0);
    this->progressBarButtonLayout->setSpacing(0);
    this->beginAndStopBtn = new QPushButton(); //播放暂停按钮
    this->beginAndStopBtn->setFixedSize(35,35);
    this->beginAndStopBtn->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "background-image:url(:/image/UI/begin_2.png)"
                "}"
                "QPushButton:hover {"
                "background-image:url(:/image/UI/begin.png)"
                "}"
                "QPushButton:pressed {"
                "background-image:url(:/image/UI/begin_1.png)"
                "}"
                "}"
                );
    connect(this->beginAndStopBtn, &QPushButton::clicked, this, [this]()
    {
        beginAndStop();
    });
}

void ReplayWin::addControl_control()
{
    this->setLayout(this->mainLayout);
    this->mainLayout->addWidget(this->listWin_Widget);
    this->listWin_Widget->setLayout(this->listWinLayout);
    this->listWinLayout->addWidget(this->scrollArea_Widget);
    this->scrollArea_Widget->setLayout(this->scrollAreaWidgetLayout);
    this->scrollAreaWidgetLayout->addWidget(this->cameraArea_scrollArea);
    this->cameraArea_scrollArea->setWidget(this->listItem_Widget);
    this->listItem_Widget->setLayout(this->cameraListLayout);
    //-------------先测试，后面使用函数添加摄像头(配置)01-------------
    //添加摄像头
    addList_camera();
    //----------------------------------------------------------
    this->listWinLayout->addWidget(this->qCalendarWidgetTime);
    this->mainLayout->addWidget(this->vedioWin_Widget);
    this->vedioWin_Widget->setLayout(this->chanleViewLayout);
    this->chanleViewLayout->addWidget(this->vedio_Widget_NO); //添加无回放提示窗体
    this->vedio_Widget_NO->setLayout(this->vedio_Widget_NOVLayout);
    this->vedio_Widget_NOVLayout->addStretch(0);
    this->vedio_Widget_NOVLayout->addWidget(this->vedio_Widget_NOWin);
    this->vedio_Widget_NOVLayout->addStretch(0);
    this->vedio_Widget_NOWin->setLayout(this->vedio_Widget_NOHLayout);
    this->vedio_Widget_NOHLayout->addStretch(0);
    this->vedio_Widget_NOHLayout->addWidget(this->noCameraLabel);
    this->vedio_Widget_NOHLayout->addStretch(0);
    this->chanleViewLayout->addWidget(this->vedio_Widget); //添加有回放窗体
    this->vedio_Widget->setLayout(this->vedioLayout);
    this->chanleViewLayout->addWidget(this->progressBar_Widget);
    this->progressBar_Widget->setLayout(this->progressBarLayout);
    this->progressBarLayout->addWidget(this->progressBar);
    this->progressBarLayout->addWidget(this->progressBarButton_Widget);
    this->progressBarButton_Widget->setLayout(this->progressBarButtonLayout);
    this->progressBarButtonLayout->addStretch(0);
    this->progressBarButtonLayout->addWidget(this->beginAndStopBtn);
    this->progressBarButtonLayout->addStretch(0);
}

void ReplayWin::initialize_camera(int CameraNum)
{
    for(int i=0;i<CameraNum;i++)
    {
        QPushButton *Button = new QPushButton("摄像头"+QString::number(i));
        Button->setProperty("buttonId", false);
        Button->setFixedHeight(35);
        Button->setFont(this->font01);
        Button->setStyleSheet(
                    "QPushButton {"
                    "border: 1px solid gray;"
                    "border-radius: 8px;"
                    "color: white;"
                    "}"
                    "QPushButton:hover {"
                    "border-radius: 8px;"
                    "background-color: #787878"
                    "}"
                    );
        Buttons.push_back(Button);
        connect(Button, &QPushButton::clicked, this, [this,Button]()
        {
            judgmentClick(Button);
        });
    }
}

void ReplayWin::addList_camera()
{
    for(int i=0;i<Buttons.size();i++)
    {
        this->cameraListLayout->addWidget(Buttons.at(i));
    }
}

void ReplayWin::delete_camera(QPushButton *Button)
{
    delete WatchList_QWidget.at(WatchList_QWidget_Buttons.indexOf(Button));
    WatchList_QWidget.removeAt(WatchList_QWidget_Buttons.indexOf(Button));
    Videos.removeAt(WatchList_QWidget_Buttons.indexOf(Button));
    WatchList_QWidget_Buttons.removeOne(Button);
    //qDebug()<<WatchList_QWidget_Buttons.size()<<WatchList_QWidget.size();
    if(WatchList_QWidget.size()==0)
    {
        this->vedio_Widget->hide();
        this->vedio_Widget_NO->show();
    }
}

void ReplayWin::add_camera(QPushButton *Button)
{
    QWidget *cameraWin = new QWidget();
    QWidget *cameraWinUP = new QWidget(cameraWin);
    QWidget *cameraWinTitle = new QWidget(cameraWin);
    cameraWinTitle->setFixedHeight(35);
    cameraWinTitle->setStyleSheet(
                "background-color: #989898;"
                );
    QVBoxLayout *cameraWinLayout = new QVBoxLayout(cameraWin);
    cameraWinLayout->setMargin(0);
    cameraWinLayout->setSpacing(0);
    cameraWin->setLayout(cameraWinLayout);
    cameraWinLayout->addWidget(cameraWinUP);
    cameraWinLayout->addWidget(cameraWinTitle);
    QHBoxLayout *cameraWinUPLayout = new QHBoxLayout(cameraWinUP);
    cameraWinUP->setLayout(cameraWinUPLayout);
    QLabel *cameraView = new QLabel();
    cameraView->setStyleSheet("border: 2px solid #989898;");
    cameraWinUPLayout->addWidget(cameraView);
    QHBoxLayout *cameraTitleLayout = new QHBoxLayout(cameraWinTitle);
    cameraWinTitle->setLayout(cameraTitleLayout);
    QLabel *cameraTitle = new QLabel(Button->text(),cameraWinTitle);
    cameraTitle->setFont(this->font01);
    cameraTitle->setStyleSheet(
                "QLabel {"
                "color: white;"
                "}"
                );
    cameraTitleLayout->addStretch(0);
    cameraTitleLayout->addWidget(cameraTitle);
    cameraTitleLayout->addStretch(0);
    WatchList_QWidget.push_back(cameraWin);
    WatchList_QWidget_Buttons.push_back(Button);
    Videos.push_back(cameraView);
    this->vedio_Widget_NO->hide();
    this->vedio_Widget->show();
}

void ReplayWin::judgmentClick(QPushButton *Button)
{
    if(beginAndStopBool) //还在播放时(未暂停)不能切回放，如果切掉的是正在播放的视频对象，那么播放的setPixmap函数就会找不到对象卡死！！！
    {
        QMessageBox::information(this,"请先暂停回放","还在播放时(未暂停)不能切回放，如果切掉的是正在播放的视频对象，那么播放的setPixmap函数就会找不到对象卡死！！！\n改进：切回放时自动暂停。","好的");
        return;
    }
    if(Button->property("buttonId").toInt())
    {
        Button->setProperty("buttonId", false);
        Button->setStyleSheet(
                    "QPushButton {"
                    "border: 1px solid gray;"
                    "border-radius: 8px;"
                    "color: white;"
                    "}"
                    "QPushButton:hover {"
                    "border-radius: 8px;"
                    "background-color: #787878"
                    "}"
                    );
        //抬起
        delete_camera(Button);
    }
    else
    {
        Button->setProperty("buttonId", true);
        Button->setStyleSheet(
                    "QPushButton {"
                    "border: 1px solid gray;"
                    "border-radius: 8px;"
                    "color: white;"
                    "background-color: #989898"
                    "}"
                    );
        add_camera(Button); //按下
    }
    addList_WatchList_Btn();
}

void ReplayWin::addList_WatchList_Btn() //(切按钮)
{
    qDebug()<<"In ReplayWin of Videos.size():"<<Videos.size();
    for(int i=0;i<Videos.size();i++)//先将图片置空
    {
        Videos.at(i)->setPixmap(QPixmap()); //将释放放在切按钮这里，这样暂停也能看到图片，而且置空后就不会把刚释放的控件撑大。(重点)
        Videos.at(i)->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)); //先暂停并置空后再释放
    }
    int WatchList_QWidget_W = static_cast<int>(ceil(sqrt(WatchList_QWidget.count())));
    int WatchList_QWidget_H = static_cast<int>(ceil(WatchList_QWidget.count()/static_cast<double>(WatchList_QWidget_W)));
    //在窗口加入摄像头回放
    int index=0;
    for(int i=0;i<WatchList_QWidget_H;i++)
    {
        for(int j=0;j<WatchList_QWidget_W;j++)
        {
            this->vedioLayout->addWidget(WatchList_QWidget.at(index),i,j);
            index++;
            if(index==WatchList_QWidget.count())
            {
                break;
            }
        }
    }
    camerasNum_change(WatchList_QWidget_Buttons); //像检索窗口发送已添加的按钮列表
}

void ReplayWin::paintEvent(QPaintEvent *event)
{/*
    if(beginAndStopBool)
    {
        // 计算缩放比例（以宽度为基准）
        qreal scaleRatio = static_cast<qreal>(this->nowview->width()) / this->img.width();
        int scaledHeight = static_cast<int>(this->img.height() * scaleRatio);
        // 缩放图片（按宽度缩放，高度按比例计算）
        QPixmap scaledPixmap = QPixmap::fromImage(this->img).scaled(
                    this->nowview->width(), scaledHeight,  // 宽度与 QLabel 一致，高度按比例缩放
                    Qt::IgnoreAspectRatio,        // 忽略宽高比（因为我们手动计算了高度）
                    Qt::SmoothTransformation      // 平滑缩放
                    );
        // 创建一个新的 QPixmap，用于居中显示图片
        QPixmap finalPixmap(this->nowview->size());
        finalPixmap.fill(Qt::transparent);  // 透明背景（实际显示时会被 QLabel 的背景色覆盖）
        // 将缩放后的图片绘制到 finalPixmap 的中心
        QPainter painter(&finalPixmap);
        painter.drawPixmap(
                    (this->nowview->width() - scaledPixmap.width()) / 2,  // 水平居中
                    (this->nowview->height() - scaledPixmap.height()) / 2, // 垂直居中
                    scaledPixmap
                    );
        painter.end();
        // 设置最终的图片到 QLabel
        this->nowview->setPixmap(finalPixmap);
    }*/
}

void ReplayWin::beginAndStop()
{
    if(beginAndStopBool)
    {
        //pt->setbeginAndStopBool(false); //暂停
        for(int i=0;i<pts.size();i++)
        {
            pts.at(i)->setbeginAndStopBool(false);
        }
        beginAndStopBool=false;
        this->beginAndStopBtn->setStyleSheet(
                    "QPushButton {"
                    "border: none;"
                    "background-image:url(:/image/UI/begin_2.png)"
                    "}"
                    "QPushButton:hover {"
                    "background-image:url(:/image/UI/begin.png)"
                    "}"
                    "QPushButton:pressed {"
                    "background-image:url(:/image/UI/begin_1.png)"
                    "}"
                    "}"
                    );
    }
    else
    {
        if(Videos.size()==0)
        {
            QMessageBox::information(this,"请选择摄像头","当前还未选择回放摄像头！","好的");
            return;
        }
        for(int i=0;i<Videos.size();i++) //先固定再播放
        {
            Videos.at(i)->setFixedSize(Videos.at(i)->size());
        }
        //pt->setbeginAndStopBool(true); //播放
        for(int i=0;i<pts.size();i++)
        {
            pts.at(i)->setbeginAndStopBool(true);
        }
        beginAndStopBool=true;
        this->beginAndStopBtn->setStyleSheet(
                    "QPushButton {"
                    "border: none;"
                    "background-image:url(:/image/UI/stop_2.png)"
                    "}"
                    "QPushButton:hover {"
                    "background-image:url(:/image/UI/stop.png)"
                    "}"
                    "QPushButton:pressed {"
                    "background-image:url(:/image/UI/stop_1.png)"
                    "}"
                    "}"
                    );
    }
}

void ReplayWin::Redraw(QImage Redraw_img,QLabel *Redraw_nowview)
{
    if(beginAndStopBool)
    {
        // 计算缩放比例（以宽度为基准）
        qreal scaleRatio = static_cast<qreal>(Redraw_nowview->width()) / Redraw_img.width();
        int scaledHeight = static_cast<int>(Redraw_img.height() * scaleRatio);
        // 缩放图片（按宽度缩放，高度按比例计算）
        QPixmap scaledPixmap = QPixmap::fromImage(Redraw_img).scaled(
                    Redraw_nowview->width(), scaledHeight,  // 宽度与 QLabel 一致，高度按比例缩放
                    Qt::IgnoreAspectRatio,        // 忽略宽高比（因为我们手动计算了高度）
                    Qt::SmoothTransformation      // 平滑缩放
                    );
        // 创建一个新的 QPixmap，用于居中显示图片
        QPixmap finalPixmap(Redraw_nowview->size());
        finalPixmap.fill(Qt::transparent);  // 透明背景（实际显示时会被 QLabel 的背景色覆盖）
        // 将缩放后的图片绘制到 finalPixmap 的中心
        QPainter painter(&finalPixmap);
        painter.drawPixmap(
                    (Redraw_nowview->width() - scaledPixmap.width()) / 2,  // 水平居中
                    (Redraw_nowview->height() - scaledPixmap.height()) / 2, // 垂直居中
                    scaledPixmap
                    );
        painter.end();
        // 设置最终的图片到 QLabel
        Redraw_nowview->setPixmap(finalPixmap);
    }
}

void ReplayWin::showImage(QImage img,QLabel* nowview)
{
    if(!img.isNull())
    {
        //this->img=img;
        //this->nowview=nowview;
        this->update(); //出发重绘
        //下面使用自定义的重绘函数
        Redraw(img,nowview); //自定义函数也得配合updata()函数！！！
    }
}

void ReplayWin::receiveRetrievalwin_saveFileNameAndcameraName(QStringList fileName, QStringList cameraName)
{
    //qDebug()<<"成功接收！！！";
    qDebug()<<"U:pts.size():"<<pts.size();
    for(int i=0;i<pts.size();i++)
    {
        pts.at(i)->setbeginAndStopBool(true); //这个很重要
        pts.at(i)->setdeleteStopBool(true);  // 设置标志位
        pts.at(i)->wait();  // 等待线程结束（阻塞直到线程退出）
        delete pts.at(i);  // 安全删除线程对象
    }
    pts.clear();
    qDebug()<<"D:pts.size():"<<pts.size();
    beginVideos.clear();
    //开始添加进程
    for(int i=0;i<cameraName.size();i++)
    {
        for(int j=0;j<Videos.size();j++)
        {
            if(WatchList_QWidget_Buttons.at(j)->text()==cameraName.at(i))
            {
                beginVideos.push_back(Videos.at(j));
            }
        }
    }
    for(int i=0;i<fileName.size();i++)
    {
        ffmpegToRGBKun *apt = new ffmpegToRGBKun(fileName.at(i).toUtf8().constData(),beginVideos.at(i));
        connect(apt,SIGNAL(toImg(QImage,QLabel*)),this,SLOT(showImage(QImage,QLabel*)));
        apt->start();
        apt->setbeginAndStopBool(false);
        pts.push_back(apt);
    }
    if(beginAndStopBool) //判断是否还处于播放状态
    {
        beginAndStopBool=false;
        this->beginAndStopBtn->setStyleSheet(
                    "QPushButton {"
                    "border: none;"
                    "background-image:url(:/image/UI/begin_2.png)"
                    "}"
                    "QPushButton:hover {"
                    "background-image:url(:/image/UI/begin.png)"
                    "}"
                    "QPushButton:pressed {"
                    "background-image:url(:/image/UI/begin_1.png)"
                    "}"
                    "}"
                    );
    }
}
