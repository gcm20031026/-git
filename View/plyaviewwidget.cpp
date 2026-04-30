#include "plyaviewwidget.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMouseEvent>
#include <QMessageBox>

static DecodeThread *sharedSimulationVideos[4] = {nullptr, nullptr, nullptr, nullptr};

plyaViewWidget::plyaViewWidget(QWidget *parent, int id, QString devName) : QWidget(parent)
{
    this->id = id;
    this->devName = devName;

    // 主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 无外边距
    mainLayout->setSpacing(0);                  // 无间距

    this->setTimeLable(); // 显示时间

    videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px; background-color: #020617;");

    // 初始化右键菜单
    createMenu();
    myCamera = new MyCamera(this);
    videoPlay->setText("等待打开");
    videoPlay->setAlignment(Qt::AlignCenter);
    videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px; color: #94a3b8; background-color: #020617;");
}

void plyaViewWidget::setTimeLable()
{
    // 时间标签 - 现在放在顶部
    timeLabel = new QLabel(this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: #dbeafe; background-color: #111827; border-bottom: 1px solid #263244; padding: 2px; font-weight: 700;");
    timeLabel->setFixedHeight(24); // 稍微增加高度

    // 视频内容区域
    //    videoArea = new QWidget();
    //    videoArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    videoPlay = new QLabel(this);
    videoPlay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //    videoLayout = new QVBoxLayout(videoArea);
    //    videoLayout->addWidget(videoPlay);

    // 调整添加顺序：先时间标签，后视频区域
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(videoPlay, 1); // 添加伸缩因子确保视频区域填满剩余空间

    // 定时器更新时间
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &plyaViewWidget::updateTime);

    timer->start(1000);
    updateTime();

    // videoPlay->resize(x,y);
}

void plyaViewWidget::createMenu()
{
    menuRD = new QMenu(this);
    changeMenu = new QMenu(tr("切换设备"), this); // 创建切换设备菜单并直接设置标题
    // 设置菜单样式：白色背景 + 黑色文字
    menuRD->setStyleSheet(
        "QMenu {"
        "    background-color: white;"   // 背景色
        "    color: black;"              // 文字颜色
        "    border: 1px solid #DDDDDD;" // 边框（可选）
        "}"
        "QMenu::item {"
        "    padding: 5px 20px 5px 10px;" // 项内边距
        "}"
        "QMenu::item:selected {"
        "    background-color: #E0E0E0;" // 选中项背景色
        "}");
    // 设置子菜单样式（与主菜单相同）
    changeMenu->setStyleSheet(menuRD->styleSheet());

    // 将子菜单添加到主菜单
    this->m_pActionChange = menuRD->addMenu(changeMenu);
    this->m_pActionClose = menuRD->addAction(tr("暂停视频"));
    m_pActionScreenshot = menuRD->addAction(tr("截图"));
    this->detectFace = menuRD->addAction("开启人脸检测");
    this->saveVideos = menuRD->addAction("保存视频");
    menuRD->addSeparator();
    connect(menuRD, SIGNAL(triggered(QAction *)), this, SLOT(SlotMenuClicked(QAction *)));
    connect(changeMenu, SIGNAL(triggered(QAction *)), this, SLOT(SlotMenuDevClicked(QAction *)));
    addMenuSwitchItem();
}

// 鼠标进入区域显示青色框
void plyaViewWidget::enterEvent(QEvent *event)
{
    if (cameraAvailable)
    {
        videoPlay->setStyleSheet("border: 1px solid #22d3ee; border-radius: 2px;");
    }
    else
    {
        videoPlay->setStyleSheet("border: 1px solid #22d3ee; border-radius: 2px; color: #f8fafc; background-color: #020617;");
    }
}
// 鼠标离开显示黑色边框
void plyaViewWidget::leaveEvent(QEvent *event)
{
    if (cameraAvailable)
    {
        videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px;");
    }
    else
    {
        videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px; color: #94a3b8; background-color: #020617;");
    }
}
// 鼠标右击出现上下文菜单
void plyaViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        menuRD->exec(QCursor::pos());
    }
}

void plyaViewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit doubleClicked(id);
        event->accept();
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}

void plyaViewWidget::showCamera()
{
    if (previewStarted)
    {
        return;
    }
    previewStarted = true;

    QString inputName;
    if (id < 2)
    {
        QString cameraInfo = myCamera->getCameraInfo(id);
        if (cameraInfo.isEmpty())
        {
            cameraAvailable = false;
            videoPlay->setText("摄像头不存在");
            videoPlay->setAlignment(Qt::AlignCenter);
            videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px; color: #94a3b8; background-color: #020617;");
            emit alarmRaised(QString::fromUtf8(u8"高"), devName, QString::fromUtf8(u8"设备离线"));
            return;
        }
        inputName = QString("video=%1").arg(cameraInfo);
    }
    else
    {
        int simulationIndex = 0;
        if (id >= 12)
        {
            simulationIndex = 3;
        }
        else if (id >= 8)
        {
            simulationIndex = 2;
        }
        else if (id >= 4)
        {
            simulationIndex = 1;
        }

        const QString videoFileName = QString("VurVideo%1.mp4").arg(simulationIndex);

        inputName = QDir::current().filePath(QString("savaVideos/%1").arg(videoFileName));
        if (!QFileInfo::exists(inputName))
        {
            inputName = QString("D:/biyesheji/Security_surveillance_system/Security_surveillance_system2/bin/savaVideos/%1").arg(videoFileName);
        }
        if (!QFileInfo::exists(inputName))
        {
            cameraAvailable = false;
            videoPlay->setText(QString::fromUtf8("模拟视频不存在"));
            videoPlay->setAlignment(Qt::AlignCenter);
            videoPlay->setStyleSheet("border: 1px solid #263244; border-radius: 2px; color: #94a3b8; background-color: #020617;");
            emit alarmRaised(QString::fromUtf8(u8"中"), devName, QString::fromUtf8(u8"视频源文件缺失"));
            return;
        }

        if (sharedSimulationVideos[simulationIndex] == nullptr)
        {
            sharedSimulationVideos[simulationIndex] = new DecodeThread(
                inputName,
                QString::fromUtf8("模拟视频源%1").arg(simulationIndex));
            sharedSimulationVideos[simulationIndex]->start();
        }

        video = sharedSimulationVideos[simulationIndex];
        cameraAvailable = true;
        connect(this->video, &DecodeThread::sentImg, this, &plyaViewWidget::getImg, Qt::UniqueConnection);
        connect(this->video, &DecodeThread::alarmRaised, this, &plyaViewWidget::alarmRaised, Qt::UniqueConnection);
        connect(this, &plyaViewWidget::savePicture, this->video, &DecodeThread::savaPictureScr, Qt::UniqueConnection);
        connect(this, &plyaViewWidget::startStopDetectfaceSignal, this->video, &DecodeThread::startStopDetectfaceSlot, Qt::UniqueConnection);
        connect(this, &plyaViewWidget::saveVideosSignal, this->video, &DecodeThread::saveVideos, Qt::UniqueConnection);
        connect(this, &plyaViewWidget::setTimeSignal, this->video, &DecodeThread::getTime, Qt::UniqueConnection);
        return;
    }

    video = new DecodeThread(inputName, this->devName);
    video->start();
    cameraAvailable = true;
    connect(this->video, &DecodeThread::sentImg, this, &plyaViewWidget::getImg, Qt::UniqueConnection);
    connect(this->video, &DecodeThread::alarmRaised, this, &plyaViewWidget::alarmRaised, Qt::UniqueConnection);
    connect(this, &plyaViewWidget::savePicture, this->video, &DecodeThread::savaPictureScr, Qt::UniqueConnection);
    connect(this, &plyaViewWidget::startStopDetectfaceSignal, this->video, &DecodeThread::startStopDetectfaceSlot, Qt::UniqueConnection);
    connect(this, &plyaViewWidget::saveVideosSignal, this->video, &DecodeThread::saveVideos, Qt::UniqueConnection);
    connect(this, &plyaViewWidget::setTimeSignal, this->video, &DecodeThread::getTime, Qt::UniqueConnection);
}

void plyaViewWidget::startPreviewIfNeeded()
{
    if (!previewStarted)
    {
        showCamera();
    }
}

bool plyaViewWidget::isCameraAvailable() const
{
    return cameraAvailable;
}

// 更新时间
void plyaViewWidget::updateTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("%1   yyyy-MM-dd HH:mm:ss").arg(devName);
    timeLabel->setText(currentTime);
}
// 获取视频流
void plyaViewWidget::getImg(QImage img)
{
    x = videoPlay->width();
    y = videoPlay->height();
    //    // 录制视频（添加到编码队列）
    //    writer->addFrame(img);

    QPixmap pix = QPixmap::fromImage(img);

    // 按显示区域大小填充，避免边框内出现黑边
    pix = pix.scaled(x, y, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    videoPlay->setPixmap(pix);
    videoPlay->setAlignment(Qt::AlignCenter);
    update();
}
// 改变大小
void plyaViewWidget::changeSize()
{
    x = videoPlay->width();
    y = videoPlay->height();

    emit this->sent_X_Y(x, y);

    // 确保视频显示区域适应新的大小
    if (videoPlay->pixmap())
    {
        QPixmap pix = videoPlay->pixmap()->scaled(x, y, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        videoPlay->setPixmap(pix);
        videoPlay->setAlignment(Qt::AlignCenter);
    }
}

void plyaViewWidget::SlotMenuClicked(QAction *action)
{
    static int i = 0;
    if (action == this->m_pActionChange)
    {
        qDebug() << "切换";
    }
    else if (action == this->m_pActionClose)
    {
        if (video == nullptr)
        {
            QMessageBox::information(this, "提示", "当前摄像头不存在，无法控制视频。");
            return;
        }

        if (playFlag)
        {
            video->pause(); // 暂停
            action->setText("开始视频");
            playFlag = false;
        }
        else
        {
            video->resume(); // 开始
            action->setText("暂停视频");
            playFlag = true;
        }
    }
    else if (action == this->m_pActionScreenshot)
    {
        if (video == nullptr)
        {
            QMessageBox::information(this, "提示", "当前摄像头不存在，无法截图。");
            return;
        }

        // 发送保存图片信号
        //  获取当前日期时间
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // 转换为自定义格式的字符串
        QString picName = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");
        emit this->savePicture(picName);
    }
    else if (action == this->detectFace)
    {
        if (video == nullptr)
        {
            QMessageBox::information(this, "提示", "当前摄像头不存在，无法开启人脸检测。");
            return;
        }

        if (detectFaceFlag)
        {
            setFaceDetectEnabled(false);
        }
        else
        {
            setFaceDetectEnabled(true);
        }
    }
    else if (action == this->saveVideos)
    {
        if (video == nullptr)
        {
            QMessageBox::information(this, "提示", "当前摄像头不存在，无法保存视频。");
            emit alarmRaised(QString::fromUtf8(u8"中"), devName, QString::fromUtf8(u8"录像操作失败"));
            return;
        }

        emit this->saveVideosSignal();
        // 获取当前日期时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        // 转换为自定义格式的字符串
        QString m_times = currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");
        emit this->setTimeSignal(m_times);
    }
}

void plyaViewWidget::setFaceDetectEnabled(bool enabled)
{
    detectFaceFlag = enabled;
    if (detectFace != nullptr)
    {
        detectFace->setText(enabled ? QString::fromUtf8("关闭人脸检测") : QString::fromUtf8("开启人脸检测"));
    }
    if (video != nullptr)
    {
        emit this->startStopDetectfaceSignal(detectFaceFlag);
    }
}

// 添加切换菜单项
void plyaViewWidget::addMenuSwitchItem()
{
    for (int i = 1; i <= 16; ++i)
    {
        // 判断当前设备名
        QString devName = QString("摄像头%1").arg(i);
        // 不添加当前设备的选项
        if (this->devName == devName)
            continue;
        // 添加菜单项
        QAction *devAction = new QAction(devName, this);
        changeMenu->addAction(devAction);
    }
}
// 点击捕捉要切换设备
void plyaViewWidget::SlotMenuDevClicked(QAction *action)
{
    qDebug() << action->text();
    // 发送要切换设备信号
    emit this->sentSwichDev(this->devName, action->text());
}

plyaViewWidget::~plyaViewWidget()
{
    //    if(video) {
    //        writer->stop();
    //        video->wait();
    //        delete video;
    //    }
    //    writer->stop();
}
