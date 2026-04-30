#pragma once
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

class JdtMyPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int circleX READ circleX WRITE setCircleX)
public:
    // 构造自定义滑动开关按钮。
    explicit JdtMyPushButton(QWidget *parent = nullptr);
    // 设置开关的内部状态标志。
    void setFlagState(bool flag);
    // 设置开关是否处于开启状态。
    void setCheckedState(bool checked);
    // 返回开关当前是否开启。
    bool isCheckedState() const;

protected:
    // 绘制开关背景和圆形滑块。
    void paintEvent(QPaintEvent *) override;
    // 响应鼠标点击并切换开关状态。
    void mousePressEvent(QMouseEvent *event) override;
    // 控件尺寸变化时重新计算滑块位置。
    void resizeEvent(QResizeEvent *event) override;


private:
    // 根据当前状态刷新显示效果。
    void updateAppearance();
    // 启动滑块移动动画。
    void startAnimation(int targetX);

    int m_circleX;
    bool m_isOnLeft = false;
    QPropertyAnimation *m_animation;

    bool flag=true;

    // 属性访问器
    // 滑块横坐标属性，供动画系统读取。
    int circleX() const { return m_circleX; }
    // 更新滑块横坐标并触发重绘。
    void setCircleX(int x) { m_circleX = x; update(); }

signals:
    // 开关状态改变时发出。
    void stateChanged(bool checked);
};
