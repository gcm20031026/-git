#include "configurationwin.h"
#include<QDebug>
#include<QDir>
ConfigurationWin::ConfigurationWin(QWidget *parent) : QWidget(parent)
{
    // 设置窗口基本样式
    this->setStyleSheet(
        "ConfigurationWin {"
        "   background-color: #A9A9A9;"  // 主窗口背景
        "}"
    );

    initControl();
    initLayout();
}

void ConfigurationWin::initControl()
{
    // 创建配置容器Widget
    configWidget = new QWidget(this);
    configWidget->setObjectName("configWidget");

    // 设置标题 - 左对齐
    settingLab = new QLabel("下载设置", configWidget);
    settingLab->setStyleSheet(
        "QLabel {"
        "   font-weight: bold;"
        "   color: white;"
        "   font-size: 20px;"
        "   background-color: transparent;"
        "}"
    );
    settingLab->setFixedHeight(40);
    settingLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐

    // 设置目录说明 - 左对齐
    settingCatalog = new QLabel("下载目录：（下载文件存放路径）", configWidget);
    settingCatalog->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 12px;"
        "   background-color: transparent;"
        "}"
    );
    settingCatalog->setFixedHeight(30);
    settingCatalog->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐

    // 创建路径编辑控件
    pathWidget = new PathWithEditButton(configWidget);
    // 获取当前工作目录
    QString currentDir = QDir::currentPath();
    qDebug() << "当前工作目录：" << currentDir;
    pathWidget->setPath(SavePath);
    pathWidget->setFixedHeight(50);

    // 创建恢复默认路径按钮 - 左对齐
    recover_btn = new QPushButton("恢复到默认地址", configWidget);
    recover_btn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   color: white;"
        "   border: 1px solid white;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   text-align: left;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   background-color: #B8B8B8;"
        "}"
    );
    recover_btn->setCursor(Qt::PointingHandCursor);
    recover_btn->setFixedSize(150,40);

    // 保存视频录像标签 - 左对齐
    autoSave = new QLabel("保存视频录像");
    autoSave->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   background-color: transparent;"  // 透明背景
        "   border: 1px solid white;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "}"
    );
    autoSave->setFixedSize(150,40);
    autoSave->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐

    // 滑动选择条
    jdtBtn = new JdtMyPushButton();

    //选择视频格式
    formatOp = new FormatOption(configWidget);
    formatOp->setFixedSize(250,60);

    //人脸识别
    infoWidget = new InfoWidget(configWidget);
    infoWidget->setFixedSize(260,70);

    //信息协议
    versonWidget = new VersonInfoWidget(configWidget);
}

void ConfigurationWin::initLayout()
{
    // 主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 配置容器布局 - 左对齐
    configLayout = new QVBoxLayout(configWidget);
    configLayout->setContentsMargins(15, 15, 15, 15);  // 统一边距
    configLayout->setSpacing(20);  // 增加控件之间的间距
    configLayout->setAlignment(Qt::AlignLeft);  // 整体左对齐

    // 开启保存的布局
    jdtWidget = new QWidget();
    QHBoxLayout* jdtLayout = new QHBoxLayout(jdtWidget);
    jdtLayout->setContentsMargins(0, 0, 0, 0);  // 移除内边距
    jdtLayout->setAlignment(Qt::AlignLeft);  // 左对齐
    jdtLayout->addWidget(autoSave);
    jdtLayout->addWidget(jdtBtn);
    jdtLayout->setSpacing(10);  // 控件间距

    // 添加控件到配置容器
    configLayout->addWidget(settingLab);
    configLayout->addWidget(settingCatalog);
    configLayout->addWidget(pathWidget);
    configLayout->addWidget(recover_btn);
    configLayout->addWidget(jdtWidget);
    configLayout->addWidget(formatOp);
    configLayout->addWidget(infoWidget);
    configLayout->addWidget(versonWidget);
    configLayout->addStretch();

    // 添加配置容器到主布局
    mainLayout->addWidget(configWidget);

    // 设置配置容器样式
    configWidget->setStyleSheet(
        "QWidget#configWidget {"
        "   background-color: #333333;"
        "   border-radius: 5px;"
        "   color: white;"
        "}"
        "QWidget#configWidget QWidget {"
        "   background-color: transparent;"
        "}"
    );
}
