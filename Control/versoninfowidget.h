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
    explicit VersonInfoWidget(QWidget *parent = nullptr);
    void initControl();
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
