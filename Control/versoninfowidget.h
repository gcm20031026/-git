#ifndef VERSONINFOWIDGET_H
#define VERSONINFOWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>

class VersonInfoWidget : public QWidget
{
    Q_OBJECT
public:
    // 构造版本信息展示控件。
    explicit VersonInfoWidget(QWidget *parent = nullptr);
    // 初始化版本文字、按钮等控件。
    void initControl();
    // 设置版本信息区域的布局。
    void initLayout();
private:
    QVBoxLayout* mainLayout;
    QHBoxLayout*btnLayout,*agreementLayout;
    QLabel* titleLab,*versonLab;
    QPushButton*checkUpdateBtn,*feedbackBtn,*serviceBtn,*offwebBtn_agr,*downBtn_agr,*userAgrBtn,*privacyAgrBtn;
    QWidget *btnWidget,*agreementWidget;
signals:

};

#endif // VERSONINFOWIDGET_H
