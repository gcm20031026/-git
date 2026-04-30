#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include "Control/jdtmypushbutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    // 构造人脸检测开关设置控件。
    explicit InfoWidget(QWidget *parent = nullptr);
    // 初始化标题和开关按钮。
    void initControl();
    // 设置人脸检测设置项的布局。
    void initLayout();
    // 返回当前是否开启人脸检测。
    bool faceDetectEnabled() const;

signals:
    // 人脸检测开关状态变化时发出。
    void faceDetectChanged(bool enabled);

private:
    QHBoxLayout *mainLayout;
    QLabel *lab;
    JdtMyPushButton *jdtBtn;
};

#endif // INFOWIDGET_H
