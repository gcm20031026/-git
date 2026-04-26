#ifndef CONFIGURATIONWIN_H
#define CONFIGURATIONWIN_H

#include <QWidget>
#include<QLabel>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QPushButton>
#include"Control/pathwitheditbutton.h"
#include"Control/jdtmypushbutton.h"
#include"Control/formatoption.h"
#include"Control/infowidget.h"
#include"Control/versoninfowidget.h"
#include"Control/pathwitheditbutton.h"
extern QString SavePath;
class ConfigurationWin : public QWidget //配置窗口
{
    Q_OBJECT
public:
    explicit ConfigurationWin(QWidget *parent = nullptr);
    void initControl();//初始化控件
    void initLayout();//初始化布局
private:
    QLabel *settingLab,*settingCatalog,*autoSave;;
    QVBoxLayout*mainLayout,*configLayout;
    QHBoxLayout*jdtLayout;
    PathWithEditButton*pathWidget;
    QWidget *configWidget,*jdtWidget;;
    QPushButton*recover_btn;
    JdtMyPushButton *jdtBtn;
    FormatOption *formatOp;
    InfoWidget *infoWidget;
    VersonInfoWidget*versonWidget;

signals:

public slots:
};

#endif // CONFIGURATIONWIN_H
