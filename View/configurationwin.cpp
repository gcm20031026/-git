#include "configurationwin.h"
#include <QDebug>
#include <QDir>

ConfigurationWin::ConfigurationWin(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("ConfigurationWin { background-color: #0f172a; }");
    initControl();
    initLayout();
}

void ConfigurationWin::initControl()
{
    configWidget = new QWidget(this);
    configWidget->setObjectName("configWidget");

    settingLab = new QLabel(QString::fromUtf8(u8"系统配置"), configWidget);
    settingLab->setStyleSheet("QLabel { color: #f8fafc; font-size: 24px; font-weight: 800; background: transparent; }");
    settingLab->setFixedHeight(42);
    settingLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    settingCatalog = new QLabel(QString::fromUtf8(u8"录像保存目录"), configWidget);
    settingCatalog->setStyleSheet("QLabel { color: #94a3b8; font-size: 14px; background: transparent; }");
    settingCatalog->setFixedHeight(26);
    settingCatalog->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    pathWidget = new PathWithEditButton(configWidget);
    qDebug() << "current work dir:" << QDir::currentPath();
    pathWidget->setPath(SavePath);
    pathWidget->setFixedHeight(54);

    recover_btn = new QPushButton(QString::fromUtf8(u8"恢复默认目录"), configWidget);
    recover_btn->setCursor(Qt::PointingHandCursor);
    recover_btn->setFixedSize(150, 40);
    recover_btn->setStyleSheet(
        "QPushButton { background-color: #182235; color: #dbeafe; border: 1px solid #334155; border-radius: 8px; padding: 5px 12px; font-weight: 700; }"
        "QPushButton:hover { background-color: #233148; border-color: #22d3ee; color: #ffffff; }"
        "QPushButton:pressed { background-color: #0f766e; border-color: #5eead4; }");

    autoSave = new QLabel(QString::fromUtf8(u8"自动保存录像"));
    autoSave->setFixedSize(150, 40);
    autoSave->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    autoSave->setStyleSheet(
        "QLabel { color: #e2e8f0; background-color: #182235; border: 1px solid #334155; border-radius: 8px; padding: 5px 12px; }");

    jdtBtn = new JdtMyPushButton();

    formatOp = new FormatOption(configWidget);
    formatOp->setFixedSize(300, 60);

    infoWidget = new InfoWidget(configWidget);
    infoWidget->setFixedSize(300, 70);
    connect(formatOp, SIGNAL(formatChanged(QString)), this, SIGNAL(saveFormatChanged(QString)));
    connect(infoWidget, SIGNAL(faceDetectChanged(bool)), this, SIGNAL(faceDetectChanged(bool)));

    versonWidget = new VersonInfoWidget(configWidget);
}

void ConfigurationWin::initLayout()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(0);

    configLayout = new QVBoxLayout(configWidget);
    configLayout->setContentsMargins(28, 26, 28, 26);
    configLayout->setSpacing(18);
    configLayout->setAlignment(Qt::AlignLeft);

    jdtWidget = new QWidget();
    QHBoxLayout *jdtLayout = new QHBoxLayout(jdtWidget);
    jdtLayout->setContentsMargins(0, 0, 0, 0);
    jdtLayout->setAlignment(Qt::AlignLeft);
    jdtLayout->setSpacing(12);
    jdtLayout->addWidget(autoSave);
    jdtLayout->addWidget(jdtBtn);

    configLayout->addWidget(settingLab);
    configLayout->addWidget(settingCatalog);
    configLayout->addWidget(pathWidget);
    configLayout->addWidget(recover_btn);
    configLayout->addWidget(jdtWidget);
    configLayout->addWidget(formatOp);
    configLayout->addWidget(infoWidget);
    configLayout->addWidget(versonWidget);
    configLayout->addStretch();

    mainLayout->addWidget(configWidget);

    configWidget->setStyleSheet(
        "QWidget#configWidget { background-color: #111827; border: 1px solid #263244; border-radius: 8px; color: white; }"
        "QWidget#configWidget QWidget { background-color: transparent; }");
}
