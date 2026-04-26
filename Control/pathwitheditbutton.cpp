#include "pathwitheditbutton.h"
#include <QDebug>
#include<QFileDialog>
#include<QString>
QString SavePath = "./savaVideos/";  // 声明全局变量
PathWithEditButton::PathWithEditButton(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
    // 获取当前工作目录
    QString currentDir = QDir::currentPath();
    currentDir.replace(R"(\)", "/");
    currentDir = QString("%1/savaVideos").arg(currentDir);
    SavePath = currentDir;
    qDebug() << "当前工作目录：" << SavePath;
}

void PathWithEditButton::setPath(const QString &path)
{
    m_pathLabel->setText(path);
}

QString PathWithEditButton::path() const
{
    return m_pathLabel->text();
}

void PathWithEditButton::setButtonText(const QString &text)
{
    m_editButton->setText(text);
}

QString PathWithEditButton::buttonText() const
{
    return m_editButton->text();
}
//跳出文件对话框
void PathWithEditButton::onButtonClicked()
{
    //选择保存的路径
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    dir.replace(R"(\)", "/");  // 使用原始字符串避免转义

    if(dir.isEmpty()) return ;
    SavePath = dir;
    //更改路径
    this->setPath(SavePath);

}

void PathWithEditButton::setupUI()
{
    // 创建布局和控件
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 8, 8, 8); // 增加内边距让边框更明显
    m_layout->setSpacing(10);

    m_pathLabel = new QLabel(this);
    m_pathLabel->setText("C:\\Users\\Default\\Videos");

    m_editButton = new QPushButton(this);
    m_editButton->setText("更改目录");
    m_editButton->setCursor(Qt::PointingHandCursor);

    // 添加到布局
    m_layout->addWidget(m_pathLabel);
    m_layout->addWidget(m_editButton);
    m_layout->addStretch();

    // 连接信号槽
    connect(m_editButton, &QPushButton::clicked, this, &PathWithEditButton::onButtonClicked);
}

void PathWithEditButton::setupStyles()
{
    this->setStyleSheet(
        "PathWithEditButton {"
        "   border: none;"  // 移除外层边框（如果需要可以保留）
        "   border-radius: 4px;"
        "   background-color: #444444;"  // 背景色
        "   color: white;"  // 默认文字颜色
        "}"
        "PathWithEditButton QLabel {"
        "   color: white;"
        "   padding: 3px 8px;"  // 增加内边距让边框更明显
        "   background-color: transparent;"  // 透明背景
        "   border: 1px solid white;"  // 添加白色边框
        "   border-radius: 3px;"  // 圆角
        "}"
        "PathWithEditButton QPushButton {"
        "   border: 1px solid white;"  // 添加白色边框
        "   border-radius: 3px;"  // 圆角
        "   color: #4da6ff;"  // 亮蓝色
        "   background: transparent;"  // 透明背景
        "   padding: 2px 8px;"  // 调整内边距
        "   text-align: center;"
        "   text-decoration: none;"
        "}"
        "PathWithEditButton QPushButton:hover {"
        "   text-decoration: underline;"
        "   color: #66b3ff;"  // 悬停时更亮
        "   border: 1px solid #66b3ff;"  // 悬停时边框变亮蓝色
        "}"
    );
}
