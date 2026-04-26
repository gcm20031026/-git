#include "versoninfowidget.h"

VersonInfoWidget::VersonInfoWidget(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
}

void VersonInfoWidget::initControl()
{
    titleLab = new QLabel("关于Security",this);
    titleLab->setStyleSheet(
        "QLabel {"
        "   font-weight: bold;"
        "   color: white;"
        "   font-size: 20px;"
        "   background-color: transparent;"
        "}"
    );
    titleLab->setFixedHeight(40);
    titleLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐
    versonLab = new QLabel("当前版本：0.0.01",this);
    versonLab->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 12px;"
        "   background-color: transparent;"
        "}"
    );
    versonLab->setFixedHeight(30);
    versonLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐

    checkUpdateBtn = new QPushButton("检查更新");
    checkUpdateBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   color: white;"
        "   border: 1px solid white;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   background-color: #B8B8B8;"
        "}"
    );
    checkUpdateBtn->setFixedSize(150,40);
    feedbackBtn = new QPushButton("反馈意见");
    feedbackBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   color: white;"
        "   border: 1px solid white;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   background-color: #B8B8B8;"
        "}"
    );
    feedbackBtn->setFixedSize(150,40);

    serviceBtn = new QPushButton("客服中心");
    serviceBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"
        "   color: white;"
        "   border: 1px solid white;"
        "   border-radius: 10px;"
        "   padding: 5px 10px;"
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   background-color: #B8B8B8;"
        "}"
    );
    serviceBtn->setFixedSize(150,40);
    offwebBtn_agr = new QPushButton("<<Security官网>>");
    offwebBtn_agr->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"  // 背景色
        "   color: white;"               // 默认字体颜色
        "   border: none;"               // 无边框
        "   text-decoration: underline;" // 字体带下划线
        "   padding: 5px;"               // 适当内边距
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   color: pink;"               // 鼠标悬停时字体变粉色
        "}"
    );
    offwebBtn_agr->setCursor(Qt::PointingHandCursor);  // 鼠标悬停时变为手型
    downBtn_agr = new QPushButton("<<APP下载>>");
    downBtn_agr->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"  // 背景色
        "   color: white;"               // 默认字体颜色
        "   border: none;"               // 无边框
        "   text-decoration: underline;" // 字体带下划线
        "   padding: 5px;"               // 适当内边距
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   color: pink;"               // 鼠标悬停时字体变粉色
        "}"
    );
    downBtn_agr->setCursor(Qt::PointingHandCursor);  // 鼠标悬停时变为手型
    userAgrBtn = new QPushButton("<<用户协议>>");
    userAgrBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"  // 背景色
        "   color: white;"               // 默认字体颜色
        "   border: none;"               // 无边框
        "   text-decoration: underline;" // 字体带下划线
        "   padding: 5px;"               // 适当内边距
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   color: pink;"               // 鼠标悬停时字体变粉色
        "}"
    );
    userAgrBtn->setCursor(Qt::PointingHandCursor);  // 鼠标悬停时变为手型
    privacyAgrBtn = new QPushButton("<<隐私政策>>");
    privacyAgrBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #333333;"  // 背景色
        "   color: white;"               // 默认字体颜色
        "   border: none;"               // 无边框
        "   text-decoration: underline;" // 字体带下划线
        "   padding: 5px;"               // 适当内边距
        "   text-align: center;"  // 文字左对齐
        "}"
        "QPushButton:hover {"
        "   color: pink;"               // 鼠标悬停时字体变粉色
        "}"
    );
    privacyAgrBtn->setCursor(Qt::PointingHandCursor);  // 鼠标悬停时变为手型
}

void VersonInfoWidget::initLayout()
{
    mainLayout = new QVBoxLayout(this);

    btnWidget = new QWidget(this);
    btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐
    btnLayout->addWidget(checkUpdateBtn);
    btnLayout->addWidget(feedbackBtn);
    btnLayout->addWidget(serviceBtn);

    agreementWidget = new QWidget(this);
    agreementLayout = new QHBoxLayout(agreementWidget);
    agreementLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 左对齐
    agreementLayout->addWidget(offwebBtn_agr);
    agreementLayout->addWidget(downBtn_agr);
    agreementLayout->addWidget(userAgrBtn);
    agreementLayout->addWidget(privacyAgrBtn);

    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(versonLab);
    mainLayout->addWidget(btnWidget);
    mainLayout->addWidget(agreementWidget);

}
