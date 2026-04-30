#ifndef FORMATOPTION_H
#define FORMATOPTION_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

extern QString savaFormat;

class FormatOption : public QWidget
{
    Q_OBJECT
public:
    // 构造视频保存格式选择控件。
    explicit FormatOption(QWidget *parent = nullptr);
    // 初始化标签、下拉框等界面控件。
    void initControl();
    // 组织格式选择控件的布局。
    void initLayout();
    // 返回当前选中的视频保存格式。
    QString getSelectedFormat() const;

signals:
    // 当保存格式发生变化时通知配置页和录制模块。
    void formatChanged(const QString &format);

private:
    QHBoxLayout *mainLayout;
    QLabel *lab;
    QComboBox *comboBox;
};

#endif // FORMATOPTION_H
