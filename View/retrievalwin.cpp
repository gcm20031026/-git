#include "retrievalwin.h"

RetrievalWin::RetrievalWin(QWidget *parent) : QWidget(parent)
{
    initialize_control();
    addControl_control();
    connect(this->refreshButton,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(this->additionBtn,SIGNAL(clicked()),this,SLOT(addition_fileFormat()));
    connect(this->subtractionBtn,SIGNAL(clicked()),this,SLOT(subtraction_fileFormat()));
    connect(this->yesButton,SIGNAL(clicked()),this,SLOT(save()));
}

void RetrievalWin::initialize_control()
{
    //设置字体font01
    this->font01.setFamily("黑体");
    this->font01.setPointSize(20);
    //设置字体font02
    this->font02.setFamily("黑体");
    this->font02.setPointSize(15);
    //设置字体font03
    this->font03.setFamily("宋体");
    this->font03.setPointSize(15);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //隐藏系统边框
    this->mainLayout = new QVBoxLayout();
    this->mainLayout->setMargin(0);
    this->mainLayout->setSpacing(0);
    this->mainWin = new QWidget();
    this->mainWin->setStyleSheet(
                "background-color: #383838;"
                );
    this->mianUpAndDownLayout = new QVBoxLayout();
    this->mianUpAndDownLayout->setMargin(0);
    this->mianUpAndDownLayout->setSpacing(0);
    this->UpWin = new QWidget();
    this->UpWin->setFixedHeight(70);
    this->UpWin->setStyleSheet(
                "border-bottom: 2px solid #989898;"
                );
    this->UpWin_Layout = new QHBoxLayout();
    this->UpWin_Layout->setMargin(0);
    this->UpWin_Layout->setSpacing(0);
    this->searchEdit = new QLineEdit();
    this->searchEdit->setPlaceholderText("文件路径/bin目录下的相对路径...");
    this->searchEdit->setText("savaVideos");
    this->searchEdit->setFont(this->font02);
    this->searchEdit->setFixedSize(300,35);
    this->searchEdit->setStyleSheet(
                "QLineEdit {"
                "border: 1px solid gray;"
                "border-radius: 8px;"
                "color: white;"
                "}"
                "QLineEdit:hover {"
                "border-radius: 8px;"
                "background-color: #787878"
                "}"
                );
    this->refreshButton = new QPushButton("刷新");
    this->refreshButton->setFont(this->font02);
    this->refreshButton->setFixedHeight(35);
    this->refreshButton->setFixedWidth(70);
    this->refreshButton->setStyleSheet(
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
    this->fileFormat = new QComboBox(); //文件格式
    this->fileFormat->setEditable(true); // 允许用户编辑
    this->fileFormat->setFixedSize(120,35);
    this->fileFormat->setFont(font02);
    this->fileFormat->setStyleSheet(
                "QComboBox {"
                "color: white;"
                "}"
                );
    this->fileFormat->addItem(".mp4");
    this->additionBtn = new QPushButton("+");
    this->additionBtn->setFixedSize(35,35);
    this->additionBtn->setStyleSheet(
                "QPushButton {"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "color: white;"
                "}"
                "QPushButton:hover {"
                "background-color: #787878;"
                "border-radius: 5px;"
                "}"
                );
    this->subtractionBtn = new QPushButton("-");
    this->subtractionBtn->setFixedSize(35,35);
    this->subtractionBtn->setStyleSheet(
                "QPushButton {"
                "border: 1px solid gray;"
                "border-radius: 5px;"
                "color: white;"
                "}"
                "QPushButton:hover {"
                "background-color: #787878;"
                "border-radius: 5px;"
                "}"
                );
    this->yesButton = new QPushButton("保存");
    this->yesButton->setFont(this->font02);
    this->yesButton->setFixedHeight(35);
    this->yesButton->setFixedWidth(70);
    this->yesButton->setStyleSheet(
                "QPushButton {"
                "border: 1px solid gray;"
                "border-radius: 8px;"
                "color: white;"
                "}"
                "QPushButton:hover {"
                "border-radius: 8px;"
                "background-color: #787878;"
                "}"
                );
    this->DownWin = new QWidget();
    this->listOne_outSide_Layout = new QVBoxLayout();
    this->listOne_outSide_Layout->setMargin(0);
    this->listOne_outSide_Layout->setSpacing(0);
    this->listOne = new QScrollArea();
    this->listOne->setWidgetResizable(true);
    //this->listOne->setStyleSheet();
    this->listOne_Win = new QWidget();
    this->listOne_InSide_Layout = new QVBoxLayout();
    //下面是提示模块
    this->HintWin = new QWidget();
    HintWin->setStyleSheet(
                "QWidget {"
                "background-color: #282828;"
                "}"
                );
    HintWin->setFixedHeight(40);
    QHBoxLayout *fileWinLayout = new QHBoxLayout();
    fileWinLayout->setMargin(0);
    fileWinLayout->setSpacing(0);
    HintWin->setLayout(fileWinLayout);
    QLabel *fileLabel = new QLabel("请先在回放窗口添加摄像头");
    fileLabel->setFont(this->font03);
    fileLabel->setStyleSheet(
                "QLabel {"
                "color: white;"
                "}"
                );
    fileWinLayout->addStretch(0);
    fileWinLayout->addWidget(fileLabel);
    fileWinLayout->addStretch(0);
    //预设路径！！！
    this->Path01 = "savaVideos";
    this->Path02<<".mp4";
    addFileList(Path01,Path02);
    //qDebug()<<"1111"<<findFilesByKeywords(Path01,Path02)<<"1111";
    qDebug()<<FileListWin.size();
}

void RetrievalWin::addControl_control()
{
    this->setLayout(this->mainLayout);
    this->mainLayout->addWidget(this->mainWin);
    this->mainWin->setLayout(this->mianUpAndDownLayout);
    this->mianUpAndDownLayout->addWidget(this->UpWin);
    this->mianUpAndDownLayout->addWidget(this->DownWin);
    this->UpWin->setLayout(this->UpWin_Layout);
    this->UpWin_Layout->addStretch(0);
    this->UpWin_Layout->addWidget(this->searchEdit);
    this->UpWin_Layout->addWidget(this->refreshButton);
    this->UpWin_Layout->addStretch(0);
    this->UpWin_Layout->addWidget(this->fileFormat);
    this->UpWin_Layout->addWidget(this->additionBtn);
    this->UpWin_Layout->addWidget(this->subtractionBtn);
    this->UpWin_Layout->addStretch(0);
    this->UpWin_Layout->addWidget(this->yesButton);
    this->UpWin_Layout->addStretch(0);
    this->DownWin->setLayout(this->listOne_outSide_Layout);
    this->listOne_outSide_Layout->addWidget(this->listOne);
    this->listOne->setWidget(this->listOne_Win);
    this->listOne_Win->setLayout(this->listOne_InSide_Layout);
}

void RetrievalWin::addFileList(QString &directoryPath, QStringList &keywords)
{
    clear_FileList();
    QStringList result = findFilesByKeywords(directoryPath,keywords);
    for(int i=0;i<result.size();i++)
    {
        QWidget *fileWin = new QWidget();
        fileWin->setStyleSheet(
                    "QWidget {"
                    "background-color: #585858;"
                    "}"
                    "QWidget:hover {"
                    "background-color: #484848;"
                    "}"
                    "QWidget:pressed {"
                    "}"
                    "}"
                    );
        fileWin->setFixedHeight(40);
        QHBoxLayout *fileWinLayout = new QHBoxLayout();
        fileWinLayout->setMargin(0);
        fileWinLayout->setSpacing(0);
        fileWin->setLayout(fileWinLayout);
        QLabel *fileLabel = new QLabel(result.at(i));
        fileLabel->setFont(this->font01);
        fileLabel->setStyleSheet(
                    "QLabel {"
                    "color: white;"
                    "}"
                    );
        QComboBox *comboBox = new QComboBox();
        comboBox->setFixedSize(120,35);
        comboBox->setFont(font02);
        comboBox->setStyleSheet(
                    "QComboBox {"
                    "color: white;"
                    "}"
                    );
        comboBox->addItem("无");
        //comboBox->setMaxVisibleItems(3); // 下拉框最多显示3个选项（超出则滚动）
        fileWinLayout->addWidget(fileLabel);
        fileWinLayout->addStretch(0);
        fileWinLayout->addWidget(comboBox);
        this->listOne_InSide_Layout->addWidget(fileWin);
        FileListWin.push_back(fileWin);
        comboBoxList.push_back(comboBox);
        yesfileName.push_back(fileLabel);
    }
    this->listOne_InSide_Layout->addWidget(HintWin);
    this->listOne_InSide_Layout->addStretch(0);
}

QStringList RetrievalWin::findFilesByKeywords(QString &directoryPath, QStringList &keywords, bool recursive)
{
    QStringList result;
    // 设置迭代器标志
    QDirIterator::IteratorFlags flags = recursive
            ? QDirIterator::Subdirectories
            : QDirIterator::NoIteratorFlags;
    QDirIterator it(directoryPath, QDir::Files | QDir::NoSymLinks, flags);
    while (it.hasNext())
    {
        QString filePath = it.next();
        QString fileName = QFileInfo(filePath).fileName(); // 获取纯文件名
        // 检查是否包含任意一个关键词
        for (const QString &keyword : keywords)
        {
            if (fileName.contains(keyword, Qt::CaseInsensitive))
            {
                result.append(filePath);
                break; // 只要匹配一个关键词就添加，然后检查下一个文件
            }
        }
    }
    return result;
}

bool RetrievalWin::repeatedJudgment(QList<QComboBox *> comboBoxList)
{
    for (int i = 0; i < comboBoxList.size(); i++)
    {
        for (int j = i + 1; j < comboBoxList.size(); ++j)
        {
            if (comboBoxList.at(i)->currentText() == comboBoxList.at(j)->currentText())
            {
                if(comboBoxList.at(i)->currentText()=="无")
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void RetrievalWin::clear_FileList()
{
    if(FileListWin.size()==0)
    {
        return;
    }
    QLayoutItem* item;
    while ((item = this->listOne_InSide_Layout->takeAt(0)) != nullptr)
    {
        // 不删除控件，仅移除布局项
        delete item; // 必须删除 QLayoutItem，否则内存泄漏，只释放了控件管理item的内存，不释放控件内存
    }
    for(int i=0;i<FileListWin.size();i++) //释放控件内存
    {
        delete FileListWin.at(i);
    }
    qDebug()<<"FileListWin.size()="<<FileListWin.size();
    FileListWin.clear();
    comboBoxList.clear();
    yesfileName.clear();
    qDebug()<<"FileListWin.size()="<<FileListWin.size();
}

void RetrievalWin::camerasNum_change_retrievalwin(QList<QPushButton *> WatchList_QWidget_Buttons)
{
    qDebug()<<"In retrievalwin of WatchList_QWidget_Buttons.size():"<<WatchList_QWidget_Buttons.size();
    for(int index=0;index<comboBoxList.size();index++) //这是遍历每个下拉框
    {
        //先清除下拉框中的选项
        comboBoxList.at(index)->clear();
        comboBoxList.at(index)->addItem("无");
        for(int j=0;j<WatchList_QWidget_Buttons.size();j++) //这是遍历下拉框中的每个选项
        {
            comboBoxList.at(index)->addItem(WatchList_QWidget_Buttons.at(j)->text());
        }
    }
}

void RetrievalWin::refresh()
{
    this->Path01=this->searchEdit->text();
    this->Path02.clear();
    for (int i = 0; i < this->fileFormat->count(); i++)
    {
        this->Path02<<this->fileFormat->itemText(i);
    }
    addFileList(Path01,Path02);
}

void RetrievalWin::addition_fileFormat()
{
    //this->fileFormat->addItem("."); //逻辑修改
    QMessageBox::information(this,"添加格式","为了避免冗余直接在左侧编辑框修改，并回车即可添加文件格式！","好的");
}

void RetrievalWin::subtraction_fileFormat()
{
    if(this->fileFormat->count()<2)
    {
        QMessageBox::information(this,"删除失败","至少需要查询一种文件格式！","好的");
        return;
    }
    this->fileFormat->removeItem(this->fileFormat->currentIndex()); //删除当前选中
}

void RetrievalWin::save()
{
    //检查有没有重复摄像头
    if(repeatedJudgment(comboBoxList))
    {
        QMessageBox::information(this,"摄像头重复","同一摄像头只能播放一个回放，请重新选择！","好的");
        return;
    }
    cameraList_fileName.clear(); //清空播放文件路径缓冲区
    cameraList_cameraName.clear(); //清空播放缓摄像头名称冲区
    for(int i=0;i<comboBoxList.size();i++)
    {
        if(comboBoxList.at(i)->currentText()!="无")
        {
            cameraList_fileName<<yesfileName.at(i)->text();
            cameraList_cameraName<<comboBoxList.at(i)->currentText();
        }
    }
    qDebug()<<cameraList_fileName;
    qDebug()<<cameraList_cameraName;
    //QMessageBox::information(this,"保存成功","保存成功，可前往回放窗口查看回放。","好的"); //有bug？？？
    to_save(cameraList_fileName,cameraList_cameraName); //发送回回放界面
}
