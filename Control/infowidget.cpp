#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
    mainLayout->setContentsMargins(0, 10, 0, 10);  // 移除内边距
}

void InfoWidget::initControl()
{
    lab = new QLabel("开启人脸检测",this);
    jdtBtn = new JdtMyPushButton(this);
    jdtBtn->setFlagState(false);

}

void InfoWidget::initLayout()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(lab);
    mainLayout->addWidget(jdtBtn);
    this->setStyleSheet(
        "InfoWidget {"
        "   border: none;"  // 移除外层边框（如果需要可以保留）
        "   border-radius: 4px;"
        "   background-color: #444444;"  // 背景色
        "   color: white;"  // 默认文字颜色
        "}"
        "InfoWidget QLabel {"
        "   color: white;"
        "   padding: 3px 8px;"  // 增加内边距让边框更明显
        "   background-color: transparent;"  // 透明背景
        "   border: 1px solid white;"  // 添加白色边框
        "   border-radius: 10px;"  // 圆角
        "}");
}
