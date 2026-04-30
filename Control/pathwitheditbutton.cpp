#include "pathwitheditbutton.h"
#include "data/userstorage.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>

QString SavePath = "./savaVideos/";

PathWithEditButton::PathWithEditButton(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();

    UserStorage::ensureCurrentUserDirs();
    SavePath = UserStorage::videoDir();
    SavePath.replace(R"(\)", "/");
    setPath(SavePath);
    qDebug() << "current save path:" << SavePath;
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

void PathWithEditButton::onButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        QString::fromUtf8(u8"选择保存目录"),
        SavePath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    dir.replace(R"(\)", "/");

    if (dir.isEmpty())
    {
        return;
    }

    SavePath = dir;
    setPath(SavePath);
}

void PathWithEditButton::setupUI()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(10);

    m_pathLabel = new QLabel(this);
    m_pathLabel->setText(QString::fromUtf8(u8"未选择目录"));

    m_editButton = new QPushButton(this);
    m_editButton->setText(QString::fromUtf8(u8"更改目录"));
    m_editButton->setCursor(Qt::PointingHandCursor);

    m_layout->addWidget(m_pathLabel, 1);
    m_layout->addWidget(m_editButton);

    connect(m_editButton, &QPushButton::clicked, this, &PathWithEditButton::onButtonClicked);
}

void PathWithEditButton::setupStyles()
{
    setStyleSheet(
        "PathWithEditButton { border: 1px solid #334155; border-radius: 8px; background-color: #182235; color: white; }"
        "PathWithEditButton QLabel { color: #e2e8f0; padding: 4px 10px; background-color: #0f172a; border: 1px solid #263244; border-radius: 6px; }"
        "PathWithEditButton QPushButton { border: 1px solid #0e7490; border-radius: 6px; color: #cffafe; background: #155e75; padding: 4px 12px; font-weight: 700; }"
        "PathWithEditButton QPushButton:hover { color: #ffffff; background: #0e7490; border-color: #67e8f9; }");
}
