#ifndef PATHWITHEDITBUTTON_H
#define PATHWITHEDITBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
extern QString SavePath;
class PathWithEditButton : public QWidget
{
    Q_OBJECT
public:
    // 构造“路径显示 + 编辑按钮”组合控件。
    explicit PathWithEditButton(QWidget *parent = nullptr);

    // 设置路径文本
    void setPath(const QString &path); // 设置路径显示文本。
    // 获取当前路径
    QString path() const; // 获取当前路径文本。

    // 设置按钮文本
    void setButtonText(const QString &text); // 设置按钮显示文本。
    // 获取按钮文本
    QString buttonText() const; // 获取按钮显示文本。

signals:
    // 按钮点击信号
    void buttonClicked(); // 编辑按钮被点击时发出。

private slots:
    // 内部按钮点击处理
    void onButtonClicked(); // 内部按钮点击处理函数，转发 buttonClicked 信号。

private:
    QLabel *m_pathLabel;          // 路径显示标签
    QPushButton *m_editButton;    // 编辑按钮
    QHBoxLayout *m_layout;        // 布局管理器

    void setupUI();               // 初始化UI
    void setupStyles();           // 设置样式
};

#endif // PATHWITHEDITBUTTON_H
