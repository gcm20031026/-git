#include "mainwindow.h"
#include<QDebug>
#include <QStyle>
#include <QTimer>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initialize_window();
    addControl_window();
    initialize_control();
    addControl_control();
    this->custom_title_Widget->installEventFilter(this); //安装自定义标题的监控
    initChanelSize();//初始化大小
    initAddWidgets();//初始化通道
    hide_video_all();//隐藏所有通道
    //开始默认4通道
    this->changeCountTo(1,2);
    //开始默认预览界面
    setMainButtonSelected(mainButton01, true);
    this->trackingMainWidget=this->PreviewWin;
    connect(this->miniminze_btn,SIGNAL(clicked()),this,SLOT(miniminze()));
    connect(this->fullScreen_btn,SIGNAL(clicked()),this,SLOT(fullScreen()));
    connect(this->close_btn,SIGNAL(clicked()),this,SLOT(close()));
    // 连接按钮和槽函数
    connect(this->chanel_Btn1, &QPushButton::clicked, this, [this]() { this->changeCountTo(1, 1); emit this->sentChangeChanleSize(); });
       connect(this->chanel_Btn2, &QPushButton::clicked, this, [this]() { this->changeCountTo(1, 2); emit this->sentChangeChanleSize();});
       connect(this->chanel_Btn4, &QPushButton::clicked, this, [this]() { this->changeCountTo(2, 2); emit this->sentChangeChanleSize();});
       connect(this->chanel_Btn8, &QPushButton::clicked, this, [this]() { this->changeCountTo(2, 4); emit this->sentChangeChanleSize();});
       connect(this->chanel_Btn9, &QPushButton::clicked, this, [this]() { this->changeCountTo(3, 3); emit this->sentChangeChanleSize();});
       connect(this->chanel_Btn16, &QPushButton::clicked, this, [this]() { this->changeCountTo(4, 4); emit this->sentChangeChanleSize();});

   connect(this->replayWin,SIGNAL(camerasNum_change(QList<QPushButton*>)),this->retrievalWin,SLOT(camerasNum_change_retrievalwin(QList<QPushButton*>))); //回放界面和检索界面的通信
   connect(this->retrievalWin,SIGNAL(to_save(QStringList,QStringList)),this->replayWin,SLOT(receiveRetrievalwin_saveFileNameAndcameraName(QStringList,QStringList))); //检索界面和回放界面的通信

   //循环连接所有 widgets
   for (int i = 0; i < widgets.size(); ++i)
   {
       connect(this, &MainWindow::sentChangeChanleSize, widgets.at(i), &plyaViewWidget::changeSize);
       //connect( widgets.at(i), &plyaViewWidget::sentSwichDev,this, &MainWindow::switchDev);
   }
   btnConnect();
    //点击浏览 回放 检索 配置 实现功能
    connect(this->mainButton01, &QPushButton::clicked, this, [this]() {
        setMainButtonSelected(mainButton01, true);
        //功能
        pageTtansition(mainButton01);
    });
    connect(this->mainButton02, &QPushButton::clicked, this, [this]() {
        setMainButtonSelected(mainButton02, true);
        //功能
        pageTtansition(mainButton02);
    });
    connect(this->mainButton03, &QPushButton::clicked, this, [this]() {
        setMainButtonSelected(mainButton03, true);
        //功能
        pageTtansition(mainButton03);
    });
    connect(this->mainButton04, &QPushButton::clicked, this, [this]() {
        setMainButtonSelected(mainButton04, true);
        //功能
        pageTtansition(mainButton04);
    });

}

void MainWindow::initialize_window()
{
    //设置字体font_mainTop01
    QFont font_mainTop01;
    font_mainTop01.setFamily("黑体");
    font_mainTop01.setBold(true);
    font_mainTop01.setPointSize(16);
    //设置icon
    this->setWindowIcon(QIcon(QPixmap(":/image/UI/icon.png")));
    //窗口初始化
    this->setWindowLocationAndSize(800,300,1200,700); //窗口位置大小
    this->setWindowFlags(Qt::FramelessWindowHint); //隐藏系统边框
    this->setStyleSheet("background-color:#FFFFFF;"); //设置窗口背景
    //this->setWindowIcon(QIcon("icon01.png")); //icon设置
    //系统控件初始化
    this->mainLayout = new QVBoxLayout(); //主窗口布局
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);
    this->custom_title_Widget = new QWidget(); //自定义标题栏
    this->custom_title_Widget->setFixedHeight(80); //固定高度
    //QSS
    this->custom_title_Widget->setStyleSheet(
                "background-color: #505050;"
                );
    this->custom_titleLayout = new QHBoxLayout(); //自定义标题栏布局
    this->custom_titleLayout->setMargin(0);
    this->custom_titleLayout->setSpacing(0);
    this->logo = new QLabel(); //主界面logo
    this->logo->setFixedSize(300,80);
    QPixmap logoPixmap(":/image/Logo/logo.png");
    this->logo->setPixmap(logoPixmap.scaled(300, 80, Qt::KeepAspectRatio));
    this->mainButton01 = new QPushButton();
    this->mainButton01->setFixedSize(150,80);
    this->mainButton01->setText("预览"); //设置按钮的文本
    this->mainButton01->setFont(font_mainTop01); //设置字体
    this->mainButton01->setIcon(QIcon(":/image/UI/Camera.png")); //设置按钮的图标
    this->mainButton01->setIconSize(QSize(35,35)); //设置图标的大小
    this->mainButton01->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "color: white;"
                "text-align: left;"
                "padding-left: 15px;"
                "}"
                "QPushButton:hover {"
                "background-color: #686868;"
                "}"
                "QPushButton:pressed {"
                "background-color: #383838;"
                "}"
                "QPushButton[selected=true] {"
                "background-color: #383838;" // 选中状态更深的颜色
                "}"
                "QPushButton::icon {"
                "margin-right: 25px;"
                "}"
                );
    this->mainButton02 = new QPushButton();
    this->mainButton02->setFixedSize(150,80);
    this->mainButton02->setText("回放"); //设置按钮的文本
    this->mainButton02->setFont(font_mainTop01); //设置字体
    this->mainButton02->setIcon(QIcon(":/image/UI/Replay.png")); //设置按钮的图标
    this->mainButton02->setIconSize(QSize(35,35)); //设置图标的大小
    this->mainButton02->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "color: white;"
                "text-align: left;"
                "padding-left: 15px;"
                "}"
                "QPushButton:hover {"
                "background-color: #686868;"
                "}"
                "QPushButton:pressed {"
                "background-color: #383838;"
                "}"
                "QPushButton[selected=true] {"
                "background-color: #383838;" // 选中状态更深的颜色
                "}"
                "QPushButton::icon {"
                "margin-right: 25px;"
                "}"
                );
    this->mainButton03 = new QPushButton();
    this->mainButton03->setFixedSize(150,80);
    this->mainButton03->setText("检索"); //设置按钮的文本
    this->mainButton03->setFont(font_mainTop01); //设置字体
    this->mainButton03->setIcon(QIcon(":/image/UI/Retrieval.png")); //设置按钮的图标
    this->mainButton03->setIconSize(QSize(35,35)); //设置图标的大小
    this->mainButton03->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "color: white;"
                "text-align: left;"
                "padding-left: 15px;"
                "}"
                "QPushButton:hover {"
                "background-color: #686868;"
                "}"
                "QPushButton:pressed {"
                "background-color: #383838;"
                "}"
                "QPushButton[selected=true] {"
                "background-color: #383838;" // 选中状态更深的颜色
                "}"
                "QPushButton::icon {"
                "margin-right: 25px;"
                "}"
                );
    this->mainButton04 = new QPushButton();
    this->mainButton04->setFixedSize(150,80);
    this->mainButton04->setText("配置"); //设置按钮的文本
    this->mainButton04->setFont(font_mainTop01); //设置字体
    this->mainButton04->setIcon(QIcon(":/image/UI/Cenfiguration.png")); //设置按钮的图标
    this->mainButton04->setIconSize(QSize(35,35)); //设置图标的大小
    this->mainButton04->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "color: white;"
                "text-align: left;"
                "padding-left: 15px;"
                "}"
                "QPushButton:hover {"
                "background-color: #686868;"
                "}"
                "QPushButton:pressed {"
                "background-color: #383838;"
                "}"
                "QPushButton[selected=true] {"
                "background-color: #383838;" // 选中状态更深的颜色
                "}"
                "QPushButton::icon {"
                "margin-right: 30px;"
                "}"
                );
    this->systemOperation_Widget = new QWidget(); //自定义系统三大键div
    this->systemOperation_Widget->setFixedSize(150,35);
    this->systemOperationLayout = new QHBoxLayout(); //自定义系统三大键div布局
    this->systemOperationLayout->setMargin(0);
    this->systemOperationLayout->setSpacing(0);
    this->miniminze_btn = new QPushButton(); //最小化
    this->miniminze_btn->setFixedSize(35,35);
    this->miniminze_btn->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "background-image:url(:/image/UI/Minimize_Button.png);"
                "}"
                "QPushButton:hover {"
                "border-radius: 8px;"
                "background-color: #787878"
                "}"
                );
    this->fullScreen_btn = new QPushButton(); //全屏
    this->fullScreen_btn->setFixedSize(35,35);
    this->fullScreen_btn->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "border-radius: 8px;"
                "background-image:url(:/image/UI/FullScreen_Button.png);"
                "}"
                "QPushButton:hover {"
                "border-radius: 8px;"
                "background-color: #787878"
                "}"
                );
    this->close_btn = new QPushButton(); //关闭
    this->close_btn->setFixedSize(35,35);
    this->close_btn->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "background-image:url(:/image/UI/Close_Button.png);"
                "}"
                "QPushButton:hover {"
                "border-radius: 8px;"
                "background-color: #787878"
                "}"
                );
    this->downMain_Widget = new QWidget(); //下方主界面
    //QSS
    this->downMain_Widget->setStyleSheet(
                "background-color: #686868;"
                );
    this->downMainLayout = new QHBoxLayout(); //下方主界面的布局
    this->downMainLayout->setMargin(0);
    this->downMainLayout->setSpacing(0);
    this->listWin_Widget = new QWidget(); //左边列表界面
    this->listWin_Widget->setFixedWidth(300);
    //QSS
    this->listWin_Widget->setStyleSheet(
                "background-color: #383838;"
                );
    this->listWinLayout = new QVBoxLayout(); //左边列表布局
    this->listWinLayout->setMargin(0);
    this->listWinLayout->setSpacing(0);
    this->vedioWin_Widget = new QWidget(); //右边视频界面
    //QSS
    this->vedioWin_Widget->setStyleSheet(
                "background-color: black;"
                );
    this->chanleViewLayout = new QGridLayout(); //右边视频界面布局
    this->chanleViewLayout->setMargin(0);
    this->chanleViewLayout->setSpacing(0);

    this->chanleHLayout= new QHBoxLayout();
    //    this->chanleHLayout->setMargin(0);
    //    this->chanleHLayout->setSpacing(0);
    this->chanl_Widget = new QWidget();
    this->chanl_Widget->setLayout(chanleHLayout);
    this->chanl_Widget->setStyleSheet(
                "background-color: #383838;"
                );
    //通道数
    this->chanel_Btn1 = new QPushButton("1X1");
    this->chanel_Btn2 = new QPushButton("1X2");
    this->chanel_Btn4 = new QPushButton("2X2");
    this->chanel_Btn8 = new QPushButton("2X4");
    this->chanel_Btn9 = new QPushButton("3X3");
    this->chanel_Btn16 = new QPushButton("4X4");
    chanleHLayout->addWidget(chanel_Btn1);
    chanleHLayout->addWidget(chanel_Btn2);
    chanleHLayout->addWidget(chanel_Btn4);
    chanleHLayout->addWidget(chanel_Btn8);
    chanleHLayout->addWidget(chanel_Btn9);
    chanleHLayout->addWidget(chanel_Btn16);

    //回放界面
    this->replayWin = new ReplayWin();
    this->replayWin->hide();
    //检索界面
    this->retrievalWin = new RetrievalWin();
    this->retrievalWin->hide();
    //配置界面
    this->configurationWin = new ConfigurationWin();
    this->configurationWin->hide();
}

void MainWindow::addControl_window()
{
    this->setLayout(this->mainLayout);
    this->mainLayout->addWidget(this->custom_title_Widget,1);
    this->custom_title_Widget->setLayout(this->custom_titleLayout);
    this->custom_titleLayout->addWidget(this->logo);
    this->custom_titleLayout->addWidget(this->mainButton01);
    this->custom_titleLayout->addWidget(this->mainButton02);
    this->custom_titleLayout->addWidget(this->mainButton03);
    this->custom_titleLayout->addWidget(this->mainButton04);
    this->custom_titleLayout->addStretch(0);
    this->custom_titleLayout->addWidget(this->systemOperation_Widget);
    this->systemOperation_Widget->setLayout(this->systemOperationLayout);
    this->systemOperationLayout->addWidget(this->miniminze_btn);
    this->systemOperationLayout->addWidget(this->fullScreen_btn);
    this->systemOperationLayout->addWidget(this->close_btn);
    this->mainLayout->addWidget(this->downMain_Widget);

    this->downMain_Widget->setLayout(this->downMainLayout);
    this->downMainLayout->addWidget(this->listWin_Widget);
    this->downMainLayout->addWidget(this->vedioWin_Widget);
    this->listWin_Widget->setLayout(this->listWinLayout);
    //下排按钮
    this->mainLayout->addWidget(chanl_Widget,0);
    //回放界面
    this->mainLayout->addWidget(this->replayWin);
    //检索界面
    this->mainLayout->addWidget(this->retrievalWin);
    //配置界面
    this->mainLayout->addWidget(this->configurationWin);
}

void MainWindow::initialize_control()
{
    //设置字体font_playbackBtn02
    QFont font_playbackBtn02;
    font_playbackBtn02.setPointSize(12);
    this->PlaybackButton_Widget = new QWidget(); //两个回放按钮的窗口
    this->PlaybackButton_Widget->setFixedHeight(35);
    this->PlaybackButtonLayout = new QHBoxLayout(); //两个回放按钮的布局
    this->PlaybackButtonLayout->setMargin(0);
    this->PlaybackButtonLayout->setSpacing(0);

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
    this->btn01 = new QPushButton("摄像头0");
    this->btn01->setFixedHeight(30);
    this->btn01->setStyleSheet("color: white;"); // 设置字体颜色为白色，背景颜色为黑色
    // 设置objectName（重要）
    btn01->setObjectName(QString("%1").arg(1));

    this->btn02 = new QPushButton("摄像头1");
    this->btn02->setFixedHeight(30);
    this->btn02->setStyleSheet("color: white;");
    btn02->setObjectName(QString("%1").arg(2));

    this->btn03 = new QPushButton("摄像头2");
    this->btn03->setFixedHeight(30);
    this->btn03->setStyleSheet("color: white; ");
    btn03->setObjectName(QString("%1").arg(3));

    this->btn04 = new QPushButton("摄像头3");
    this->btn04->setFixedHeight(30);
    this->btn04->setStyleSheet("color: white; ");
    btn04->setObjectName(QString("%1").arg(4));

    this->btn05 = new QPushButton("摄像头4");
    this->btn05->setFixedHeight(30);
    this->btn05->setStyleSheet("color: white;");
    btn05->setObjectName(QString("%1").arg(5));

    this->btn06 = new QPushButton("摄像头5");
    this->btn06->setFixedHeight(30);
    this->btn06->setStyleSheet("color: white;");
    btn06->setObjectName(QString("%1").arg(6));

    this->btn07 = new QPushButton("摄像头6");
    this->btn07->setFixedHeight(30);
    this->btn07->setStyleSheet("color: white; ");
    btn07->setObjectName(QString("%1").arg(7));

    this->btn08 = new QPushButton("摄像头7");
    this->btn08->setFixedHeight(30);
    this->btn08->setStyleSheet("color: white;");
    btn08->setObjectName(QString("%1").arg(8));

    this->btn09 = new QPushButton("摄像头8");
    this->btn09->setFixedHeight(30);
    this->btn09->setStyleSheet("color: white;");
    btn09->setObjectName(QString("%1").arg(9));

    this->btn10 = new QPushButton("摄像头9");
    this->btn10->setFixedHeight(30);
    this->btn10->setStyleSheet("color: white; ");
    btn10->setObjectName(QString("%1").arg(10));

    this->btn11 = new QPushButton("摄像头10");
    this->btn11->setFixedHeight(30);
    this->btn11->setStyleSheet("color: white;");
    btn11->setObjectName(QString("%1").arg(11));


}

void MainWindow::addControl_control()
{

    this->listWinLayout->addWidget(this->scrollArea_Widget);
    this->scrollArea_Widget->setLayout(this->scrollAreaWidgetLayout);
    this->scrollAreaWidgetLayout->addWidget(this->cameraArea_scrollArea);
    this->cameraArea_scrollArea->setWidget(this->listItem_Widget);
    this->listItem_Widget->setLayout(this->cameraListLayout);
    //-------------先测试，后面使用函数添加摄像头(配置)01-------------
    this->cameraListLayout->addWidget(this->btn01);
    this->cameraListLayout->addWidget(this->btn02);
    this->cameraListLayout->addWidget(this->btn03);
    this->cameraListLayout->addWidget(this->btn04);
    this->cameraListLayout->addWidget(this->btn05);
    this->cameraListLayout->addWidget(this->btn06);
    this->cameraListLayout->addWidget(this->btn07);
    this->cameraListLayout->addWidget(this->btn08);
    this->cameraListLayout->addWidget(this->btn09);
    this->cameraListLayout->addWidget(this->btn10);
    this->cameraListLayout->addWidget(this->btn11);
    this->cameraListLayout->addStretch(0);
    //----------------------------------------------------------
    //this->listWinLayout->addWidget(this->qCalendarWidgetTime);
    this->vedioWin_Widget->setLayout(this->chanleViewLayout);
}

void MainWindow::setWindowLocationAndSize(int x, int y, int length, int width)
{
    this->setGeometry(x,y,length,width);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //鼠标输入
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            if(obj == this->custom_title_Widget)
            {
                m_dragPosition = mouseEvent->globalPos() - this->pos();
                return true;
            }
        }
    }
    else if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->buttons() == Qt::LeftButton)
        {
            if(obj == this->custom_title_Widget)
            {
                if(fullScreen_bool)
                {
                    this->showNormal();
                    fullScreen_bool=false;
                }
                QPoint newPos = mouseEvent->globalPos() - m_dragPosition;
                this->move(newPos);
                return true;
            }
        }
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->buttons() == Qt::LeftButton)
        {
            if(obj == this->custom_title_Widget)
            {
                if(fullScreen_bool)
                {
                    this->showNormal();
                    fullScreen_bool=false;
                }
                else
                {
                    this->showFullScreen();
                    fullScreen_bool=true;
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 调用父类的resizeEvent
    QWidget::resizeEvent(event);

    // 只有在预览界面时才调整通道大小
    if(this->trackingMainWidget == this->PreviewWin)
    {
        // 重新计算通道大小，适应新的窗口尺寸
        this->changeCountTo(currentRow, currentCol);
        emit this->sentChangeChanleSize();
    }
}

void MainWindow::initAddWidgets()
{
    // 创建 16 个 QWidget 对象并添加到 widgets 容器中
       for (int i = 0; i < 16; ++i)
       {
           QString devName = QString("摄像头%1").arg(i);
           plyaViewWidget *widget = new plyaViewWidget(this,i,devName);
           widgets.push_back(widget);
       }
}

void MainWindow::hide_video_all()
{
    for(int i=0;i<16;++i)
    {
        widgets.at(i)->setVisible(false);
    }
}

void MainWindow::miniminze()
{
    this->showMinimized();
}

void MainWindow::fullScreen()
{
    if(fullScreen_bool)
    {
        this->showNormal();
        fullScreen_bool=false;
    }
    else
    {
        this->showFullScreen();
        fullScreen_bool=true;
    }

    // 延迟一点再重新计算大小，确保窗口已经调整完成
    QTimer::singleShot(50, this, [this]() {
        // 重新计算通道大小，适应新的窗口尺寸
        if(this->trackingMainWidget == this->PreviewWin)
        {
            this->changeCountTo(currentRow, currentCol);
            emit this->sentChangeChanleSize();
        }
    });
}

void MainWindow::changeCountTo(int row, int col)
{
    //隐藏所有通道
    this->hide_video_all();

    // 更新当前通道布局
    currentRow = row;
    currentCol = col;

    // 动态计算通道大小，根据当前视频窗口大小
    int windowWidth = this->vedioWin_Widget->width();
    int windowHeight = this->vedioWin_Widget->height();

    // 确保窗口有效大小（避免除零错误）
    if (windowWidth <= 0) windowWidth = 800;
    if (windowHeight <= 0) windowHeight = 600;

    int w = windowWidth / col;
    int h = windowHeight / row;

    // 按照行列数排列通道
    for (int r = 0; r < row; ++r)
    {
        for (int c = 0; c < col; ++c)
        {
            int index = r * col + c;
            if (index < widgets.size())
            {
                // 设置动态计算的大小
                widgets.at(index)->setFixedSize(w, h);
                chanleViewLayout->addWidget(widgets.at(index), r, c);
                widgets.at(index)->setVisible(true);
            }
        }
    }
}
//切换设备显示
void MainWindow::switchDev(QString devName,QString targetDevName)
{
    qDebug()<<"devName"<<devName;
    qDebug()<<"targetDevName"<<devName;
    //获取第几个
    QStringList current = devName.split("摄像头");
    QStringList target = targetDevName.split("摄像头");
    int curId = current[1].toInt()-1;
    int tarId = target[1].toInt()-1;
    qDebug()<<"curId:"<<curId<<"  tarId:"<<tarId;
    //切换
    replaceWidgetInGrid(chanleViewLayout,widgets.at(curId),widgets.at(tarId));
}

void MainWindow::singleShow()
{
    // 获取触发信号的按钮
    QPushButton *clickedBtn = qobject_cast<QPushButton*>(sender());
    if (!clickedBtn) return;

    // 获取按钮名称（如 "btn01"）
    QString btnName = clickedBtn->objectName();

    bool selectedOk = false;
    int selectedIndex = btnName.toInt(&selectedOk) - 1;
    if (!selectedOk || selectedIndex < 0 || selectedIndex >= widgets.size())
    {
        QMessageBox::information(this, "提示", "摄像头编号无效。");
        return;
    }
    if (!widgets.at(selectedIndex)->isCameraAvailable())
    {
        QMessageBox::information(this, "提示", "该摄像头不存在或未连接。");
        return;
    }

    //隐藏所有通道
    this->hide_video_all();

    // 更新当前通道布局为1x1
    currentRow = 1;
    currentCol = 1;

    // 动态计算通道大小，根据当前视频窗口大小
    int windowWidth = this->vedioWin_Widget->width();
    int windowHeight = this->vedioWin_Widget->height();

    // 确保窗口有效大小（避免除零错误）
    if (windowWidth <= 0) windowWidth = 800;
    if (windowHeight <= 0) windowHeight = 600;

    int w = windowWidth;
    int h = windowHeight;

    bool ok = false;
    int i = btnName.toInt(&ok) - 1;
    if (!ok || i < 0 || i >= widgets.size())
    {
        QMessageBox::information(this, "提示", "摄像头编号无效。");
        return;
    }
    if (!widgets.at(i)->isCameraAvailable())
    {
        QMessageBox::information(this, "提示", "该摄像头不存在或未连接。");
        return;
    }
    //按照按列数排列通道
    for (int r = 0; r < 1; ++r)
    {
        for (int c = 0; c < 1; ++c)
        {
            int index = r * 1 + c;
            if (index < widgets.size())
            {
                //设置大小
                widgets.at(i)->setFixedSize (w,h);
                chanleViewLayout->addWidget(widgets.at(i), r, c);
                widgets.at(i)->setVisible(true);

            }
        }
    }

    // 发送信号，通知视频显示区域调整大小
    emit this->sentChangeChanleSize();

}
//切换设备
void MainWindow::replaceWidgetInGrid(QGridLayout* grid, QWidget* oldWidget, QWidget* newWidget)
{
    // 检查两个控件是否都在 grid 中
    int index1 = grid->indexOf(oldWidget);
    int index2 = grid->indexOf(newWidget);
    //都在
    if (index1 != -1 && index2 != -1)
    {
        //交换位置
        // 获取 widget1 的位置信息
        int row1, col1, rowSpan1, colSpan1;
        grid->getItemPosition(index1, &row1, &col1, &rowSpan1, &colSpan1);

        // 获取 widget2 的位置信息
        int row2, col2, rowSpan2, colSpan2;
        grid->getItemPosition(index2, &row2, &col2, &rowSpan2, &colSpan2);

        // 移除两个控件
        grid->removeWidget(oldWidget);
        grid->removeWidget(newWidget);

        // 交换位置重新添加
        grid->addWidget(oldWidget, row2, col2, rowSpan2, colSpan2);
        grid->addWidget(newWidget, row1, col1, rowSpan1, colSpan1);

        // 确保控件可见
        oldWidget->show();
        newWidget->show();
        return;
    }
    //一个在一个不在
    // 获取旧控件的位置
    int row, col, rowSpan, colSpan;
    grid->getItemPosition(grid->indexOf(oldWidget), &row, &col, &rowSpan, &colSpan);

    // 移除旧控件
    grid->removeWidget(oldWidget);
    oldWidget->hide();

    // 添加新控件到相同位置
    grid->addWidget(newWidget, row, col, rowSpan, colSpan);
    newWidget->show();
}

void MainWindow::btnConnect()
{
    connect(btn01, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn02, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn03, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn04, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn05, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn06, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn07, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn08, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn09, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn10, &QPushButton::clicked, this, &MainWindow::singleShow);
    connect(btn11, &QPushButton::clicked, this, &MainWindow::singleShow);
}

void MainWindow::setMainButtonSelected(QPushButton* button, bool selected)
{
    if (button)
    {
        button->setProperty("selected", selected);
        button->style()->unpolish(button);
        button->style()->polish(button);
        if (selected)
        {
            // 取消之前选中按钮的状态
            if (currentSelectedMainButton && currentSelectedMainButton != button)
            {
                setMainButtonSelected(currentSelectedMainButton, false);
            }
            currentSelectedMainButton = button;
        }
        else if (currentSelectedMainButton == button)
        {
            currentSelectedMainButton = nullptr;
        }
    }
}

void MainWindow::pageTtansition(QPushButton* Button)
{
    //因为主界面底下按钮窗口直接在主布局中，所以主界面被分为了两个窗口（1.监控视图窗口 2.监控排列选择按钮窗口）
    //因此需要隐藏两次，需要特别判断
    if(this->trackingMainWidget==this->PreviewWin) //主界面
    {
        this->downMain_Widget->hide();
        this->chanl_Widget->hide();
    }
    else if(this->trackingMainWidget==this->replayWin) //回放界面
    {
        this->replayWin->hide();
    }
    else if (this->trackingMainWidget==this->retrievalWin) //检索界面
    {
        this->retrievalWin->hide();
    }
    else if (this->trackingMainWidget==this->configurationWin) //配置界面
    {
        this->configurationWin->hide();
    }
    //返回按钮对应的界面
    this->trackingMainWidget=buttonCorrespondsToInterface(Button);
    //如果是预览按钮则显示两个窗口
    if(Button==this->mainButton01)
    {
        this->downMain_Widget->show();
        this->chanl_Widget->show();
        return;
    }
    //返回按钮对应的界面
    this->trackingMainWidget->show();
}

QWidget* MainWindow::buttonCorrespondsToInterface(QPushButton *Button)
{
    if(Button==this->mainButton01)
    {
        return PreviewWin; //预览界面（主界面）
    }
    else if(Button==this->mainButton02)
    {
        return replayWin; //回放界面
    }
    else if(Button==this->mainButton03)
    {
        return retrievalWin; //检索界面
    }
    else if(Button==this->mainButton04)
    {
        return configurationWin; //配置界面
    }
}

void MainWindow::initChanelSize()
{
    channel_size.append(qMakePair(900, 574));
    channel_size.append(qMakePair(450, 574));
    channel_size.append(qMakePair(450, 287));
    channel_size.append(qMakePair(233, 287));
    channel_size.append(qMakePair(311, 191));
    channel_size.append(qMakePair(241, 144));

}

int MainWindow::addjudetchannel(int row, int col)
{

    int num = row * col;
    switch (num)
    {
        case 1:  return 0;  // 第一个元素
        case 2:  return 1;  // 第二个元素
        case 4:  return 2;
        case 8:  return 3;
        case 9:  return 4;
        case 16: return 5;
        default: return 0;  // 默认返回第一个
    }

}

