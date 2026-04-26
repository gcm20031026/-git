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
    explicit JdtMyPushButton(QWidget *parent = nullptr);
     void setFlagState(bool flag);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    void updateAppearance();
    void startAnimation(int targetX);

    int m_circleX;
    bool m_isOnLeft = false;
    QPropertyAnimation *m_animation;

    bool flag=true;

    // 属性访问器
    int circleX() const { return m_circleX; }
    void setCircleX(int x) { m_circleX = x; update(); }
};
