#include "retrievalwin.h"
#include <QFileInfo>

RetrievalWin::RetrievalWin(QWidget *parent) : QWidget(parent)
{
    initialize_control();
    addControl_control();

    connect(this->refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(this->additionBtn, SIGNAL(clicked()), this, SLOT(addition_fileFormat()));
    connect(this->subtractionBtn, SIGNAL(clicked()), this, SLOT(subtraction_fileFormat()));
    connect(this->yesButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(this->searchEdit, SIGNAL(textChanged(QString)), this, SLOT(refresh()));
    connect(this->fileFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
    connect(this->fileFormat, SIGNAL(editTextChanged(QString)), this, SLOT(refresh()));

    refresh();
}

void RetrievalWin::initialize_control()
{
    this->font01.setFamily(QString::fromUtf8("黑体"));
    this->font01.setPointSize(12);
    this->font02.setFamily(QString::fromUtf8("黑体"));
    this->font02.setPointSize(12);
    this->font03.setFamily(QString::fromUtf8("宋体"));
    this->font03.setPointSize(12);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->mainLayout = new QVBoxLayout();
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);

    this->mainWin = new QWidget();
    this->mainWin->setStyleSheet("background-color: #20242b;");
    this->mianUpAndDownLayout = new QVBoxLayout();
    this->mianUpAndDownLayout->setContentsMargins(18, 16, 18, 18);
    this->mianUpAndDownLayout->setSpacing(14);

    this->UpWin = new QWidget();
    this->UpWin->setFixedHeight(92);
    this->UpWin->setStyleSheet(
        "QWidget { background-color: #2b313a; border: 1px solid #3d4652; border-radius: 8px; }"
        "QLabel { border: none; background: transparent; color: #dfe7f3; }");
    this->UpWin_Layout = new QHBoxLayout();
    this->UpWin_Layout->setContentsMargins(18, 12, 18, 12);
    this->UpWin_Layout->setSpacing(12);

    this->searchEdit = new QLineEdit();
    this->searchEdit->setPlaceholderText(QString::fromUtf8("输入摄像头、文件名或保存时间，自动查询"));
    this->searchEdit->setFont(this->font02);
    this->searchEdit->setMinimumSize(300, 38);
    this->searchEdit->setStyleSheet(
        "QLineEdit { background-color: #20242b; border: 1px solid #4b5563; border-radius: 6px; color: #f8fafc; padding: 0 12px; }"
        "QLineEdit:focus { border: 1px solid #5aa9ff; }");

    this->refreshButton = new QPushButton(QString::fromUtf8("刷新"));
    this->refreshButton->setFont(this->font02);
    this->refreshButton->setFixedSize(74, 38);
    this->refreshButton->setStyleSheet(
        "QPushButton { background-color: #384250; border: 1px solid #536071; border-radius: 6px; color: #f8fafc; }"
        "QPushButton:hover { background-color: #465466; }"
        "QPushButton:pressed { background-color: #2f3845; }");

    this->fileFormat = new QComboBox();
    this->fileFormat->setEditable(true);
    this->fileFormat->setFixedSize(126, 38);
    this->fileFormat->setFont(font02);
    this->fileFormat->setStyleSheet(
        "QComboBox { background-color: #20242b; border: 1px solid #4b5563; border-radius: 6px; color: #f8fafc; padding-left: 10px; }"
        "QComboBox::drop-down { width: 24px; border: none; }"
        "QComboBox QAbstractItemView { background-color: #2b313a; color: #f8fafc; selection-background-color: #3f8cff; }");
    this->fileFormat->addItem(QString::fromUtf8("全部"));
    this->fileFormat->addItem(".mp4");
    this->fileFormat->addItem(".flv");
    this->fileFormat->addItem(".avi");
    this->fileFormat->addItem(".wmv");

    this->additionBtn = new QPushButton("+");
    this->additionBtn->setFixedSize(38, 38);
    this->subtractionBtn = new QPushButton("-");
    this->subtractionBtn->setFixedSize(38, 38);
    QString smallButtonStyle =
        "QPushButton { background-color: #384250; border: 1px solid #536071; border-radius: 6px; color: #f8fafc; font-size: 18px; }"
        "QPushButton:hover { background-color: #465466; }"
        "QPushButton:pressed { background-color: #2f3845; }";
    this->additionBtn->setStyleSheet(smallButtonStyle);
    this->subtractionBtn->setStyleSheet(smallButtonStyle);

    this->yesButton = new QPushButton(QString::fromUtf8("回放"));
    this->yesButton->setFont(this->font02);
    this->yesButton->setFixedSize(82, 38);
    this->yesButton->setStyleSheet(
        "QPushButton { background-color: #2f7dd3; border: 1px solid #4093f0; border-radius: 6px; color: white; }"
        "QPushButton:hover { background-color: #3b8ce3; }"
        "QPushButton:pressed { background-color: #286db9; }");

    this->DownWin = new QWidget();
    this->DownWin->setStyleSheet("QWidget { background-color: transparent; }");
    this->listOne_outSide_Layout = new QVBoxLayout();
    this->listOne_outSide_Layout->setMargin(0);
    this->listOne_outSide_Layout->setSpacing(0);
    this->listOne = new QScrollArea();
    this->listOne->setWidgetResizable(true);
    this->listOne->setFrameShape(QFrame::NoFrame);
    this->listOne->setStyleSheet(
        "QScrollArea { background-color: transparent; border: none; }"
        "QScrollBar:vertical { background: #252b33; width: 10px; margin: 0; border-radius: 5px; }"
        "QScrollBar::handle:vertical { background: #566273; border-radius: 5px; min-height: 28px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }");
    this->listOne_Win = new QWidget();
    this->listOne_Win->setStyleSheet("QWidget { background-color: transparent; }");
    this->listOne_InSide_Layout = new QVBoxLayout();
    this->listOne_InSide_Layout->setContentsMargins(0, 0, 0, 0);
    this->listOne_InSide_Layout->setSpacing(10);

    this->HintWin = new QWidget();
    HintWin->setStyleSheet("QWidget { background-color: #2b313a; border: 1px solid #3d4652; border-radius: 8px; }");
    HintWin->setFixedHeight(78);
    QHBoxLayout *hintLayout = new QHBoxLayout();
    hintLayout->setContentsMargins(12, 0, 12, 0);
    hintLayout->setSpacing(0);
    HintWin->setLayout(hintLayout);
    QLabel *hintLabel = new QLabel(QString::fromUtf8("暂无匹配录像记录"));
    hintLabel->setFont(this->font03);
    hintLabel->setStyleSheet("QLabel { color: #aab4c2; border: none; background: transparent; }");
    hintLayout->addStretch(0);
    hintLayout->addWidget(hintLabel);
    hintLayout->addStretch(0);

    QMySqlite::getInstance(QDir::current().filePath("monitor_records.db"))->initVideoRecordTable();
}

void RetrievalWin::addControl_control()
{
    this->setLayout(this->mainLayout);
    this->mainLayout->addWidget(this->mainWin);
    this->mainWin->setLayout(this->mianUpAndDownLayout);
    this->mianUpAndDownLayout->addWidget(this->UpWin);
    this->mianUpAndDownLayout->addWidget(this->DownWin);

    QLabel *titleLabel = new QLabel(QString::fromUtf8("录像检索"));
    titleLabel->setFont(this->font01);
    titleLabel->setFixedWidth(92);

    QLabel *formatLabel = new QLabel(QString::fromUtf8("格式"));
    formatLabel->setFont(this->font02);
    formatLabel->setFixedWidth(36);

    this->UpWin->setLayout(this->UpWin_Layout);
    this->UpWin_Layout->addWidget(titleLabel);
    this->UpWin_Layout->addWidget(this->searchEdit, 1);
    this->UpWin_Layout->addWidget(this->refreshButton);
    this->UpWin_Layout->addSpacing(8);
    this->UpWin_Layout->addWidget(formatLabel);
    this->UpWin_Layout->addWidget(this->fileFormat);
    this->UpWin_Layout->addWidget(this->additionBtn);
    this->UpWin_Layout->addWidget(this->subtractionBtn);
    this->UpWin_Layout->addSpacing(8);
    this->UpWin_Layout->addWidget(this->yesButton);

    this->DownWin->setLayout(this->listOne_outSide_Layout);
    this->listOne_outSide_Layout->addWidget(this->listOne);
    this->listOne->setWidget(this->listOne_Win);
    this->listOne_Win->setLayout(this->listOne_InSide_Layout);
}

void RetrievalWin::addRecordList(const QVector<VideoRecord> &records)
{
    clear_FileList();
    videoRecords = records;

    for (int i = 0; i < records.size(); ++i)
    {
        const VideoRecord &record = records.at(i);
        QWidget *fileWin = new QWidget();
        fileWin->setMinimumHeight(72);
        fileWin->setStyleSheet(
            "QWidget { background-color: #2b313a; border: 1px solid #3d4652; border-radius: 8px; }"
            "QWidget:hover { background-color: #333b46; border-color: #536071; }");

        QHBoxLayout *fileWinLayout = new QHBoxLayout();
        fileWinLayout->setContentsMargins(14, 8, 14, 8);
        fileWinLayout->setSpacing(12);
        fileWin->setLayout(fileWinLayout);

        QCheckBox *checkBox = new QCheckBox(fileWin);
        checkBox->setStyleSheet(
            "QCheckBox { color: white; border: none; background: transparent; }"
            "QCheckBox::indicator { width: 18px; height: 18px; }");
        checkBoxList.push_back(checkBox);

        QLabel *infoLabel = new QLabel(
            QString::fromUtf8("%1    %2    %3\n%4")
                .arg(record.cameraName)
                .arg(record.saveTime)
                .arg(record.format)
                .arg(record.filePath));
        infoLabel->setFont(this->font01);
        infoLabel->setWordWrap(true);
        infoLabel->setStyleSheet("QLabel { color: #eef2f7; border: none; background: transparent; }");

        fileWinLayout->addWidget(checkBox);
        fileWinLayout->addWidget(infoLabel, 1);
        this->listOne_InSide_Layout->addWidget(fileWin);
        FileListWin.push_back(fileWin);
    }

    if (records.isEmpty())
    {
        this->listOne_InSide_Layout->addWidget(HintWin);
    }
    this->listOne_InSide_Layout->addStretch(1);
}

void RetrievalWin::addFileList(QString &directoryPath, QStringList &keywords)
{
    Q_UNUSED(directoryPath);
    Q_UNUSED(keywords);
    refresh();
}

QStringList RetrievalWin::findFilesByKeywords(QString &directoryPath, QStringList &keywords, bool recursive)
{
    Q_UNUSED(directoryPath);
    Q_UNUSED(keywords);
    Q_UNUSED(recursive);
    return QStringList();
}

bool RetrievalWin::repeatedJudgment(QList<QComboBox *> comboBoxList)
{
    Q_UNUSED(comboBoxList);
    return false;
}

void RetrievalWin::clear_FileList()
{
    QLayoutItem *item;
    while ((item = this->listOne_InSide_Layout->takeAt(0)) != nullptr)
    {
        delete item;
    }
    for (int i = 0; i < FileListWin.size(); i++)
    {
        delete FileListWin.at(i);
    }
    FileListWin.clear();
    comboBoxList.clear();
    yesfileName.clear();
    checkBoxList.clear();
}

void RetrievalWin::camerasNum_change_retrievalwin(QList<QPushButton *> WatchList_QWidget_Buttons)
{
    Q_UNUSED(WatchList_QWidget_Buttons);
}

void RetrievalWin::refresh()
{
    QString format = this->fileFormat->currentText();
    QVector<VideoRecord> records = QMySqlite::getInstance(QDir::current().filePath("monitor_records.db"))
                                       ->queryVideoRecords(this->searchEdit->text(), format);
    addRecordList(records);
}

void RetrievalWin::addition_fileFormat()
{
    QString text = this->fileFormat->currentText().trimmed();
    if (text.isEmpty() || this->fileFormat->findText(text) >= 0)
    {
        return;
    }
    this->fileFormat->addItem(text);
    this->fileFormat->setCurrentText(text);
    refresh();
}

void RetrievalWin::subtraction_fileFormat()
{
    if (this->fileFormat->currentText() == QString::fromUtf8("全部") || this->fileFormat->count() <= 1)
    {
        return;
    }
    this->fileFormat->removeItem(this->fileFormat->currentIndex());
    refresh();
}

void RetrievalWin::save()
{
    cameraList_fileName.clear();
    cameraList_cameraName.clear();

    for (int i = 0; i < checkBoxList.size() && i < videoRecords.size(); ++i)
    {
        if (checkBoxList.at(i)->isChecked())
        {
            if (!QFileInfo::exists(videoRecords.at(i).filePath))
            {
                QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("选中的录像文件不存在，请重新检索。"));
                continue;
            }
            cameraList_fileName << videoRecords.at(i).filePath;
            cameraList_cameraName << videoRecords.at(i).cameraName;
        }
    }

    if (cameraList_fileName.isEmpty())
    {
        QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("请先勾选需要回放的录像。"));
        return;
    }

    emit to_save(cameraList_fileName, cameraList_cameraName);
}

bool RetrievalWin::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}
