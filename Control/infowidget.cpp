#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
    mainLayout->setContentsMargins(0, 10, 0, 10);
    connect(jdtBtn, SIGNAL(stateChanged(bool)), this, SIGNAL(faceDetectChanged(bool)));
}

void InfoWidget::initControl()
{
    lab = new QLabel(QString::fromUtf8(u8"人脸检测"), this);
    jdtBtn = new JdtMyPushButton(this);
    jdtBtn->setFlagState(false);
    jdtBtn->setCheckedState(false);
}

void InfoWidget::initLayout()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(lab);
    mainLayout->addWidget(jdtBtn);
    setStyleSheet(
        "InfoWidget { border: 1px solid #334155; border-radius: 8px; background-color: #182235; color: white; }"
        "InfoWidget QLabel { color: #e2e8f0; padding: 4px 12px; background-color: #0f172a; border: 1px solid #263244; border-radius: 6px; }");
}

bool InfoWidget::faceDetectEnabled() const
{
    return jdtBtn->isCheckedState();
}
