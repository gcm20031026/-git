#include "versoninfowidget.h"
#include <QList>

VersonInfoWidget::VersonInfoWidget(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
}

void VersonInfoWidget::initControl()
{
    titleLab = new QLabel(QString::fromUtf8(u8"关于系统"), this);
    titleLab->setFixedHeight(38);
    titleLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titleLab->setStyleSheet("QLabel { color: #f8fafc; font-size: 18px; font-weight: 800; background: transparent; }");

    versonLab = new QLabel(QString::fromUtf8(u8"当前版本：1.0.01"), this);
    versonLab->setFixedHeight(28);
    versonLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    versonLab->setStyleSheet("QLabel { color: #94a3b8; font-size: 13px; background: transparent; }");

    QString primaryButtonStyle =
        "QPushButton { background-color: #182235; color: #dbeafe; border: 1px solid #334155; border-radius: 8px; padding: 5px 12px; font-weight: 700; }"
        "QPushButton:hover { background-color: #233148; border-color: #22d3ee; color: #ffffff; }"
        "QPushButton:pressed { background-color: #0f766e; border-color: #5eead4; }";
    QString linkButtonStyle =
        "QPushButton { background: transparent; border: none; color: #67e8f9; padding: 5px 8px; text-align: center; }"
        "QPushButton:hover { color: #ffffff; text-decoration: underline; }";

    checkUpdateBtn = new QPushButton(QString::fromUtf8(u8"检查更新"));
    feedbackBtn = new QPushButton(QString::fromUtf8(u8"反馈意见"));
    serviceBtn = new QPushButton(QString::fromUtf8(u8"客服中心"));
    QList<QPushButton*> actionButtons;
    actionButtons << checkUpdateBtn << feedbackBtn << serviceBtn;
    for (int i = 0; i < actionButtons.size(); ++i)
    {
        actionButtons.at(i)->setFixedSize(150, 40);
        actionButtons.at(i)->setCursor(Qt::PointingHandCursor);
        actionButtons.at(i)->setStyleSheet(primaryButtonStyle);
    }

    offwebBtn_agr = new QPushButton(QString::fromUtf8(u8"Security 官网"));
    downBtn_agr = new QPushButton(QString::fromUtf8(u8"APP 下载"));
    userAgrBtn = new QPushButton(QString::fromUtf8(u8"用户协议"));
    privacyAgrBtn = new QPushButton(QString::fromUtf8(u8"隐私政策"));
    QList<QPushButton*> linkButtons;
    linkButtons << offwebBtn_agr << downBtn_agr << userAgrBtn << privacyAgrBtn;
    for (int i = 0; i < linkButtons.size(); ++i)
    {
        linkButtons.at(i)->setCursor(Qt::PointingHandCursor);
        linkButtons.at(i)->setStyleSheet(linkButtonStyle);
    }
}

void VersonInfoWidget::initLayout()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 8, 0, 0);
    mainLayout->setSpacing(8);

    btnWidget = new QWidget(this);
    btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setSpacing(12);
    btnLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    btnLayout->addWidget(checkUpdateBtn);
    btnLayout->addWidget(feedbackBtn);
    btnLayout->addWidget(serviceBtn);

    agreementWidget = new QWidget(this);
    agreementLayout = new QHBoxLayout(agreementWidget);
    agreementLayout->setContentsMargins(0, 0, 0, 0);
    agreementLayout->setSpacing(10);
    agreementLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    agreementLayout->addWidget(offwebBtn_agr);
    agreementLayout->addWidget(downBtn_agr);
    agreementLayout->addWidget(userAgrBtn);
    agreementLayout->addWidget(privacyAgrBtn);

    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(versonLab);
    mainLayout->addWidget(btnWidget);
    mainLayout->addWidget(agreementWidget);
}
