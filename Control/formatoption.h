#ifndef FORMATOPTION_H
#define FORMATOPTION_H

#include <QWidget>
#include<QHBoxLayout>
#include<QLabel>
#include<QComboBox>
extern QString savaFormat;
class FormatOption : public QWidget
{
    Q_OBJECT
public:
    explicit FormatOption(QWidget *parent = nullptr);
    void initControl();
    void initLayout();
    // 新增方法：获取当前选中的格式
    QString getSelectedFormat() const;
private:
    QHBoxLayout* mainLayout;
    QLabel* lab;
    QComboBox *comboBox;
signals:

};

#endif // FORMATOPTION_H
