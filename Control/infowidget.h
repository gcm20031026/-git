#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include"Control/jdtmypushbutton.h"
#include<QLabel>
#include<QHBoxLayout>
class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);
    void initControl();
    void initLayout();
private:
    QHBoxLayout* mainLayout;
    QLabel* lab;
    JdtMyPushButton *jdtBtn;

signals:

};

#endif // INFOWIDGET_H
