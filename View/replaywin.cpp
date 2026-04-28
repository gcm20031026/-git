#include "replaywin.h"
#include "math.h"
#include <QFileInfo>
ReplayWin::ReplayWin(QWidget *parent) : QWidget(parent)
{
    // Test file path must be relative to the application working directory.
    initialize_control();
    addControl_control();
    // connect(this->pt,SIGNAL(toImg(QImage,QLabel*)),this,SLOT(showImage(QImage,QLabel*)));
    // pt->setbeginAndStopBool(false);
    // pt->start();
}

void ReplayWin::initialize_control()
{
    // 设置字体
    this->font01.setFamily("宋体");
    this->font01.setPointSize(12);
    this->font02.setFamily("宋体");
    this->font02.setPointSize(25);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->mainLayout = new QHBoxLayout();
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);
    this->listWin_Widget = new QWidget();
    this->listWin_Widget->setFixedWidth(300);
    this->listWin_Widget->setStyleSheet(
        "background-color: #383838;");
    this->listWinLayout = new QVBoxLayout();
    this->listWinLayout->setMargin(0);
    this->listWinLayout->setSpacing(0);
    this->scrollArea_Widget = new QWidget();
    this->scrollAreaWidgetLayout = new QVBoxLayout();
    this->scrollAreaWidgetLayout->setMargin(0);
    this->scrollAreaWidgetLayout->setSpacing(0);
    this->cameraArea_scrollArea = new QScrollArea();
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
        "}");
    this->listItem_Widget = new QWidget();
    this->cameraListLayout = new QVBoxLayout();
    this->recordListHintLabel = new QLabel("选择日期查看录像");
    this->recordListHintLabel->setFixedHeight(34);
    this->recordListHintLabel->setAlignment(Qt::AlignCenter);
    this->recordListHintLabel->setFont(this->font01);
    this->recordListHintLabel->setStyleSheet("QLabel { color: white; background-color: #303030; }");
    // 日历
    this->qCalendarWidgetTime = new QCalendarWidget();
    this->vedioWin_Widget = new QWidget();
    this->vedioWin_Widget->setStyleSheet(
        "background-color: black;");
    this->chanleViewLayout = new QVBoxLayout();
    this->chanleViewLayout->setMargin(0);
    this->chanleViewLayout->setSpacing(0);
    this->vedio_Widget_NO = new QWidget();
    this->vedio_Widget_NOVLayout = new QVBoxLayout();
    this->vedio_Widget_NOVLayout->setMargin(0);
    this->vedio_Widget_NOVLayout->setSpacing(0);
    this->vedio_Widget_NOWin = new QWidget();
    this->noCameraLabel = new QLabel("当前未选择录像\n请在左侧日期列表中选择录像，或从检索模块勾选录像回放");
    this->noCameraLabel->setFont(this->font02);
    this->noCameraLabel->setStyleSheet(
        "QLabel {"
        "color: white;"
        "}");
    this->vedio_Widget_NOHLayout = new QHBoxLayout();
    this->vedio_Widget_NOHLayout->setMargin(0);
    this->vedio_Widget_NOHLayout->setSpacing(0);
    this->vedio_Widget = new QWidget();
    this->vedio_Widget->hide();
    this->vedioLayout = new QGridLayout();
    this->vedioLayout->setMargin(0);
    this->vedioLayout->setSpacing(0);
    this->progressBar_Widget = new QWidget();
    this->progressBar_Widget->setFixedHeight(78);
    this->progressBar_Widget->setStyleSheet(
        "background-color: #2f2f2f;");
    this->progressBarLayout = new QVBoxLayout();
    this->progressBarLayout->setContentsMargins(12, 8, 12, 8);
    this->progressBarLayout->setSpacing(6);
    this->progressBar = new QSlider(Qt::Horizontal);
    this->progressBar->setValue(0);
    this->progressBar->setRange(0, 100);
    this->progressBar->setStyleSheet(
        "QSlider::groove:horizontal { height: 8px; border-radius: 4px; background: #151515; }"
        "QSlider::sub-page:horizontal { border-radius: 4px; background: #14b8c7; }"
        "QSlider::add-page:horizontal { border-radius: 4px; background: #555555; }"
        "QSlider::handle:horizontal { width: 16px; height: 16px; margin: -5px 0; border-radius: 8px; background: #f2f2f2; border: 1px solid #9a9a9a; }"
        "QSlider::handle:horizontal:hover { background: white; border: 1px solid #14b8c7; }");
    this->progressBarButton_Widget = new QWidget();
    this->progressBarButton_Widget->setFixedHeight(34);
    this->progressBarButton_Widget->setStyleSheet(
        "background-color: #2f2f2f;");
    this->progressBarButtonLayout = new QHBoxLayout();
    this->progressBarButtonLayout->setContentsMargins(14, 0, 14, 0);
    this->progressBarButtonLayout->setSpacing(10);
    this->beginAndStopBtn = new QPushButton();
    this->beginAndStopBtn->setFixedSize(35, 35);
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
        "}");
    this->speedComboBox = new QComboBox();
    this->speedComboBox->setFixedSize(78, 28);
    this->speedComboBox->addItem("0.5x", 0.5);
    this->speedComboBox->addItem("1.0x", 1.0);
    this->speedComboBox->addItem("1.25x", 1.25);
    this->speedComboBox->addItem("1.5x", 1.5);
    this->speedComboBox->addItem("2.0x", 2.0);
    this->speedComboBox->setCurrentIndex(1);
    this->speedComboBox->setStyleSheet(
        "QComboBox { color: white; background-color: #454545; border: 1px solid #707070; border-radius: 5px; padding-left: 8px; }"
        "QComboBox:hover { border: 1px solid #14b8c7; }"
        "QComboBox::drop-down { width: 18px; border: none; }"
        "QComboBox QAbstractItemView { color: white; background-color: #3f3f3f; selection-background-color: #14b8c7; outline: none; }");
    connect(this->beginAndStopBtn, &QPushButton::clicked, this, [this]()
            { beginAndStop(); });
    connect(this->progressBar, &QSlider::sliderPressed, this, [this]()
            { progressSliderPressed = true; });
    connect(this->progressBar, &QSlider::sliderReleased, this, [this]()
            {
                progressSliderPressed = false;
                seekProgress(); });
    connect(this->speedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePlaybackSpeed(int)));
    connect(this->qCalendarWidgetTime, SIGNAL(selectionChanged()), this, SLOT(refreshReplayRecordListSlot()));
}

void ReplayWin::addControl_control()
{
    this->setLayout(this->mainLayout);
    this->mainLayout->addWidget(this->listWin_Widget);
    this->listWin_Widget->setLayout(this->listWinLayout);
    this->listWinLayout->addWidget(this->qCalendarWidgetTime);
    this->listWinLayout->addWidget(this->recordListHintLabel);
    this->listWinLayout->addWidget(this->scrollArea_Widget);
    this->scrollArea_Widget->setLayout(this->scrollAreaWidgetLayout);
    this->scrollAreaWidgetLayout->addWidget(this->cameraArea_scrollArea);
    this->cameraArea_scrollArea->setWidget(this->listItem_Widget);
    this->listItem_Widget->setLayout(this->cameraListLayout);
    this->mainLayout->addWidget(this->vedioWin_Widget);
    this->vedioWin_Widget->setLayout(this->chanleViewLayout);
    this->chanleViewLayout->addWidget(this->vedio_Widget_NO);
    this->vedio_Widget_NO->setLayout(this->vedio_Widget_NOVLayout);
    this->vedio_Widget_NOVLayout->addStretch(0);
    this->vedio_Widget_NOVLayout->addWidget(this->vedio_Widget_NOWin);
    this->vedio_Widget_NOVLayout->addStretch(0);
    this->vedio_Widget_NOWin->setLayout(this->vedio_Widget_NOHLayout);
    this->vedio_Widget_NOHLayout->addStretch(0);
    this->vedio_Widget_NOHLayout->addWidget(this->noCameraLabel);
    this->vedio_Widget_NOHLayout->addStretch(0);
    this->chanleViewLayout->addWidget(this->vedio_Widget);
    this->vedio_Widget->setLayout(this->vedioLayout);
    this->chanleViewLayout->addWidget(this->progressBar_Widget);
    this->progressBar_Widget->setLayout(this->progressBarLayout);
    this->progressBarLayout->addWidget(this->progressBar);
    this->progressBarLayout->addWidget(this->progressBarButton_Widget);
    this->progressBarButton_Widget->setLayout(this->progressBarButtonLayout);
    this->progressBarButtonLayout->addStretch(0);
    this->progressBarButtonLayout->addWidget(this->beginAndStopBtn);
    this->progressBarButtonLayout->addWidget(this->speedComboBox);
    this->progressBarButtonLayout->addStretch(0);
    refreshReplayRecordList();
}

void ReplayWin::initialize_camera(int CameraNum)
{
    for (int i = 0; i < CameraNum; i++)
    {
        QPushButton *Button = new QPushButton(QString("通道%1").arg(i + 1));
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
            "}");
        Buttons.push_back(Button);
        connect(Button, &QPushButton::clicked, this, [this, Button]()
                {
            judgmentClick(Button); });
    }
}

void ReplayWin::addList_camera()
{
    for (int i = 0; i < Buttons.size(); i++)
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
    // qDebug()<<WatchList_QWidget_Buttons.size()<<WatchList_QWidget.size();
    if (WatchList_QWidget.size() == 0)
    {
        this->vedio_Widget->hide();
        this->vedio_Widget_NO->show();
    }
}

void ReplayWin::add_camera(QPushButton *Button)
{
    QWidget *cameraWin = new QWidget();
    QWidget *cameraWinUP = new QWidget(cameraWin);
    QVBoxLayout *cameraWinLayout = new QVBoxLayout(cameraWin);
    cameraWinLayout->setMargin(0);
    cameraWinLayout->setSpacing(0);
    cameraWin->setLayout(cameraWinLayout);
    cameraWinLayout->addWidget(cameraWinUP);
    QHBoxLayout *cameraWinUPLayout = new QHBoxLayout(cameraWinUP);
    cameraWinUPLayout->setMargin(0);
    cameraWinUPLayout->setSpacing(0);
    cameraWinUP->setLayout(cameraWinUPLayout);
    QLabel *cameraView = new QLabel();
    cameraView->setStyleSheet("border: 2px solid #989898;");
    cameraWinUPLayout->addWidget(cameraView);
    WatchList_QWidget.push_back(cameraWin);
    WatchList_QWidget_Buttons.push_back(Button);
    Videos.push_back(cameraView);
    this->vedio_Widget_NO->hide();
    this->vedio_Widget->show();
}

void ReplayWin::judgmentClick(QPushButton *Button)
{
    if (beginAndStopBool)
    {
        QMessageBox::information(this, "请先暂停回放", "当前正在播放，请先暂停后再切换通道。", "好的");
        return;
    }
    if (Button->property("buttonId").toInt())
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
            "}");
        // 移除通道
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
            "}");
        add_camera(Button);
    }
    addList_WatchList_Btn();
}

void ReplayWin::addList_WatchList_Btn()
{
    qDebug() << "In ReplayWin of Videos.size():" << Videos.size();
    for (int i = 0; i < Videos.size(); i++)
    {
        Videos.at(i)->setPixmap(QPixmap());
        Videos.at(i)->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    }
    int WatchList_QWidget_W = static_cast<int>(ceil(sqrt(WatchList_QWidget.count())));
    int WatchList_QWidget_H = static_cast<int>(ceil(WatchList_QWidget.count() / static_cast<double>(WatchList_QWidget_W)));
    // 按通道数量自动布局
    int index = 0;
    for (int i = 0; i < WatchList_QWidget_H; i++)
    {
        for (int j = 0; j < WatchList_QWidget_W; j++)
        {
            this->vedioLayout->addWidget(WatchList_QWidget.at(index), i, j);
            index++;
            if (index == WatchList_QWidget.count())
            {
                break;
            }
        }
    }
    camerasNum_change(WatchList_QWidget_Buttons);
}

void ReplayWin::clearReplayViews()
{
    QLayoutItem *item = nullptr;
    while ((item = vedioLayout->takeAt(0)) != nullptr)
    {
        delete item;
    }

    for (int i = 0; i < WatchList_QWidget.size(); ++i)
    {
        delete WatchList_QWidget.at(i);
    }
    WatchList_QWidget.clear();
    WatchList_QWidget_Buttons.clear();
    Videos.clear();
    beginVideos.clear();
}

void ReplayWin::layoutReplayViews()
{
    if (WatchList_QWidget.isEmpty())
    {
        this->vedio_Widget->hide();
        this->vedio_Widget_NO->show();
        return;
    }

    this->vedio_Widget_NO->hide();
    this->vedio_Widget->show();

    int columns = static_cast<int>(ceil(sqrt(WatchList_QWidget.count())));
    int rows = static_cast<int>(ceil(WatchList_QWidget.count() / static_cast<double>(columns)));
    int index = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            if (index >= WatchList_QWidget.count())
            {
                return;
            }
            this->vedioLayout->addWidget(WatchList_QWidget.at(index), row, col);
            ++index;
        }
    }
}

void ReplayWin::refreshReplayRecordList()
{
    QLayoutItem *item = nullptr;
    while ((item = cameraListLayout->takeAt(0)) != nullptr)
    {
        QWidget *widget = item->widget();
        if (widget != nullptr)
        {
            delete widget;
        }
        delete item;
    }
    Buttons.clear();
    replayRecordList.clear();

    QString dateText = qCalendarWidgetTime->selectedDate().toString("yyyy-MM-dd");
    QVector<VideoRecord> records = QMySqlite::getInstance(QDir::current().filePath("monitor_records.db"))
                                       ->queryVideoRecords(dateText, QString());

    for (int i = 0; i < records.size(); ++i)
    {
        if (!records.at(i).saveTime.startsWith(dateText))
        {
            continue;
        }
        replayRecordList.push_back(records.at(i));
    }

    recordListHintLabel->setText(QString("%1  共%2条录像").arg(dateText).arg(replayRecordList.size()));

    if (replayRecordList.isEmpty())
    {
        QLabel *emptyLabel = new QLabel("当天暂无录像");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setFixedHeight(48);
        emptyLabel->setFont(this->font01);
        emptyLabel->setStyleSheet("QLabel { color: #d0d0d0; background-color: #343434; }");
        cameraListLayout->addWidget(emptyLabel);
        cameraListLayout->addStretch(0);
        return;
    }

    for (int i = 0; i < replayRecordList.size(); ++i)
    {
        const VideoRecord &record = replayRecordList.at(i);
        QString timeText = record.saveTime;
        if (timeText.length() >= 19)
        {
            timeText = timeText.mid(11, 8);
        }
        QString fileName = QFileInfo(record.filePath).fileName();
        QPushButton *button = new QPushButton(QString("%1  %2  %3\n%4")
                                                  .arg(timeText)
                                                  .arg(record.cameraName)
                                                  .arg(record.format)
                                                  .arg(fileName));
        button->setProperty("recordIndex", i);
        button->setFixedHeight(58);
        button->setFont(this->font01);
        button->setStyleSheet(
            "QPushButton {"
            "text-align: left;"
            "padding-left: 10px;"
            "border: 1px solid #666666;"
            "border-radius: 6px;"
            "color: white;"
            "background-color: #3b3b3b;"
            "}"
            "QPushButton:hover {"
            "border: 1px solid #14b8c7;"
            "background-color: #474747;"
            "}");
        connect(button, &QPushButton::clicked, this, [this, button]()
                { playRecordAt(button->property("recordIndex").toInt()); });
        Buttons.push_back(button);
        cameraListLayout->addWidget(button);
    }
    cameraListLayout->addStretch(0);
}

void ReplayWin::playRecordAt(int index)
{
    if (index < 0 || index >= replayRecordList.size())
    {
        return;
    }

    const VideoRecord &record = replayRecordList.at(index);
    if (!QFileInfo::exists(record.filePath))
    {
        QMessageBox::warning(this, "提示", "录像文件不存在，请重新检索或检查保存路径。");
        return;
    }

    if (currentReplayFileName.contains(record.filePath))
    {
        return;
    }
    if (currentReplayFileName.size() >= 16)
    {
        QMessageBox::information(this, "提示", "最多支持16通道回放。");
        return;
    }

    QStringList fileName = currentReplayFileName;
    QStringList cameraName = currentReplayCameraName;
    fileName << record.filePath;
    cameraName << record.cameraName;
    receiveRetrievalwin_saveFileNameAndcameraName(fileName, cameraName);
}

void ReplayWin::refreshReplayRecordListSlot()
{
    refreshReplayRecordList();
}

void ReplayWin::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void ReplayWin::beginAndStop()
{
    if (beginAndStopBool)
    {
        for (int i = 0; i < pts.size(); i++)
        {
            pts.at(i)->setbeginAndStopBool(false);
        }
        beginAndStopBool = false;
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
            "}");
        return;
    }

    bool needReplayFromStart = replayFinished || pts.isEmpty();
    for (int i = 0; i < pts.size(); ++i)
    {
        if (!pts.at(i)->isRunning())
        {
            needReplayFromStart = true;
            break;
        }
    }
    if (needReplayFromStart && !currentReplayFileName.isEmpty())
    {
        QStringList fileName = currentReplayFileName;
        QStringList cameraName = currentReplayCameraName;
        receiveRetrievalwin_saveFileNameAndcameraName(fileName, cameraName);
    }
    if (Videos.isEmpty())
    {
        QMessageBox::information(this, "请选择录像", "当前还未选择回放录像。", "好的");
        return;
    }
    for (int i = 0; i < Videos.size(); i++)
    {
        Videos.at(i)->setFixedSize(Videos.at(i)->size());
    }
    for (int i = 0; i < pts.size(); i++)
    {
        pts.at(i)->setbeginAndStopBool(true);
    }
    beginAndStopBool = true;
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
        "}");
}

void ReplayWin::Redraw(QImage Redraw_img, QLabel *Redraw_nowview)
{
    if (Redraw_img.isNull() || Redraw_img.width() <= 0 || Redraw_img.height() <= 0 || Redraw_nowview == nullptr)
    {
        return;
    }

    qreal scaleRatio = static_cast<qreal>(Redraw_nowview->width()) / Redraw_img.width();
    int scaledHeight = static_cast<int>(Redraw_img.height() * scaleRatio);
    QPixmap scaledPixmap = QPixmap::fromImage(Redraw_img).scaled(
        Redraw_nowview->width(), scaledHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap finalPixmap(Redraw_nowview->size());
    finalPixmap.fill(Qt::transparent);
    QPainter painter(&finalPixmap);
    painter.drawPixmap(
        (Redraw_nowview->width() - scaledPixmap.width()) / 2,
        (Redraw_nowview->height() - scaledPixmap.height()) / 2,
        scaledPixmap);
    painter.end();
    Redraw_nowview->setPixmap(finalPixmap);
}

void ReplayWin::showImage(QImage img, QLabel *nowview)
{
    if (!img.isNull() && nowview != nullptr && Videos.contains(nowview))
    {
        this->update();
        Redraw(img, nowview);
    }
}

void ReplayWin::updateProgress(int value)
{
    if (!pts.isEmpty() && sender() != pts.first())
    {
        return;
    }
    if (value < 0)
    {
        value = 0;
    }
    if (value > 100)
    {
        value = 100;
    }
    replayFinished = value >= 100;
    if (!progressSliderPressed)
    {
        progressBar->setValue(value);
    }
}

void ReplayWin::seekProgress()
{
    int value = progressBar->value();
    replayFinished = value >= 100;
    bool hasRunningThread = false;
    for (int i = 0; i < pts.size(); ++i)
    {
        if (pts.at(i)->isRunning())
        {
            hasRunningThread = true;
            break;
        }
    }
    if (!hasRunningThread && !currentReplayFileName.isEmpty() && value < 100)
    {
        QStringList fileName = currentReplayFileName;
        QStringList cameraName = currentReplayCameraName;
        receiveRetrievalwin_saveFileNameAndcameraName(fileName, cameraName);
    }
    for (int i = 0; i < pts.size(); ++i)
    {
        if (pts.at(i)->isRunning())
        {
            pts.at(i)->requestSeekPercent(value);
        }
    }
}

void ReplayWin::updatePlaybackSpeed(int index)
{
    replaySpeed = speedComboBox->itemData(index).toDouble();
    if (replaySpeed <= 0.0)
    {
        replaySpeed = 1.0;
    }
    for (int i = 0; i < pts.size(); ++i)
    {
        pts.at(i)->setPlaybackSpeed(replaySpeed);
    }
}

void ReplayWin::receiveRetrievalwin_saveFileNameAndcameraName(QStringList fileName, QStringList cameraName)
{
    currentReplayFileName = fileName;
    currentReplayCameraName = cameraName;
    replayFinished = false;

    for (int i = 0; i < pts.size(); i++)
    {
        pts.at(i)->setbeginAndStopBool(true);
        pts.at(i)->setdeleteStopBool(true);
        pts.at(i)->wait();
        delete pts.at(i);
    }
    pts.clear();
    clearReplayViews();
    progressBar->setValue(0);

    int count = qMin(fileName.size(), cameraName.size());
    count = qMin(count, 16);
    for (int i = 0; i < count; ++i)
    {
        QPushButton *button = new QPushButton(QString("通道%1").arg(i + 1), this);
        add_camera(button);
    }
    layoutReplayViews();
    beginVideos = Videos;

    for (int i = 0; i < count && i < beginVideos.size(); i++)
    {
        ffmpegToRGBKun *apt = new ffmpegToRGBKun(fileName.at(i).toUtf8().constData(), beginVideos.at(i));
        connect(apt, SIGNAL(toImg(QImage, QLabel *)), this, SLOT(showImage(QImage, QLabel *)));
        connect(apt, SIGNAL(progressChanged(int)), this, SLOT(updateProgress(int)));
        apt->setPlaybackSpeed(replaySpeed);
        apt->start();
        apt->setbeginAndStopBool(false);
        pts.push_back(apt);
    }

    beginAndStopBool = false;
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
        "}");
}
