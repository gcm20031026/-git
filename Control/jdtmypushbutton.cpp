#include "jdtmypushbutton.h"
#include<QDebug>
#include<QMessageBox>
JdtMyPushButton::JdtMyPushButton(QWidget *parent)
    : QPushButton(parent)
{
    setFixedSize(120, 50);
    m_animation = new QPropertyAnimation(this, "circleX");
    m_animation->setDuration(200);
    updateAppearance();
}

void JdtMyPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 1. 绘制背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_isOnLeft ? Qt::black : Qt::white);
    painter.drawRoundedRect(rect(), height()/2, height()/2);

    // 2. 绘制圆形滑块
    painter.setBrush(Qt::gray);
    painter.drawEllipse(QPoint(m_circleX, height()/2), height()/2-5, height()/2-5);

    // 3. 绘制文字
    painter.setPen(m_isOnLeft ? Qt::white : Qt::black);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(16);
    painter.setFont(font);

    QString text = m_isOnLeft ? "否" : "是";
    QRect textRect = m_isOnLeft
        ? QRect(height()/2, 0, width()-height(), height())
        : QRect(0, 0, width()-height()/2, height());

    painter.drawText(textRect, Qt::AlignCenter, text);
}

void JdtMyPushButton::mousePressEvent(QMouseEvent *event)
{
    // 检查是否点击了圆形区域
    QRect circleRect(m_circleX - height()/2+5, 0, height()-10, height());
    if (circleRect.contains(event->pos()))
    {
        if(flag)
        {
            if(!m_isOnLeft)
            {
                //弹出警告框
                QMessageBox:: StandardButton result=QMessageBox::warning(NULL, "提示", "确定不保存录像",
                                         QMessageBox::No | QMessageBox::No, QMessageBox::Yes);
                //不选择
                if(result == QMessageBox::No) return;
                //选择
                m_isOnLeft = !m_isOnLeft;
                int targetX = m_isOnLeft ? height()/2 : width() - height()/2;
                startAnimation(targetX);
                emit stateChanged(isCheckedState());

                this->setCursor(Qt::PointingHandCursor);

            }
        }
        else
        {
            m_isOnLeft = !m_isOnLeft;
            int targetX = m_isOnLeft ? height()/2 : width() - height()/2;
            startAnimation(targetX);
            emit stateChanged(isCheckedState());
            this->setCursor(Qt::PointingHandCursor);

        }

    }

    QPushButton::mousePressEvent(event);
}

void JdtMyPushButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    updateAppearance();
}

void JdtMyPushButton::setFlagState(bool flag)
{
    this->flag = flag;
}

void JdtMyPushButton::setCheckedState(bool checked)
{
    m_isOnLeft = !checked;
    updateAppearance();
    emit stateChanged(isCheckedState());
}

bool JdtMyPushButton::isCheckedState() const
{
    return !m_isOnLeft;
}

void JdtMyPushButton::updateAppearance()
{
    m_circleX = m_isOnLeft ? height()/2 : width() - height()/2;
    update();
}

void JdtMyPushButton::startAnimation(int targetX)
{
    m_animation->stop();
    m_animation->setStartValue(m_circleX);
    m_animation->setEndValue(targetX);

    m_animation->start();
}
