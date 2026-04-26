#include "formatoption.h"
#include"QDebug"
QString savaFormat;
FormatOption::FormatOption(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
    savaFormat = comboBox->currentText();
}

void FormatOption::initControl()
{
    lab = new QLabel("下载视频的格式",this);
    comboBox = new QComboBox(this);
    comboBox->addItem(".mp4");
    comboBox->addItem(".flv");
    comboBox->addItem(".avi");
    comboBox->addItem(".wmv");
}

void FormatOption::initLayout()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(lab);
    mainLayout->addWidget(comboBox);
    this->setStyleSheet(
        "FormatOption {"
        "   border: none;"  // 移除外层边框（如果需要可以保留）
        "   border-radius: 4px;"
        "   background-color: #444444;"  // 背景色
        "   color: white;"  // 默认文字颜色
        "}"
        "FormatOption QLabel {"
        "   color: white;"
        "   padding: 3px 8px;"  // 增加内边距让边框更明显
        "   background-color: transparent;"  // 透明背景
        "   border: 1px solid white;"  // 添加白色边框
        "   border-radius: 10px;"  // 圆角
        "}"
        "FormatOption QComboBox {"
        "   color: black;"  // 文字颜色
        "   background-color: white;"  // 背景色
        "   border: 1px solid white;"  // 边框
        "   padding: 3px 8px;"  // 内边距
        "   border-radius: 1px;"  // 圆角
        "}"
        "FormatOption QComboBox QAbstractItemView {"
        "   color: black;"  // 下拉列表中文字颜色
        "   background-color: white;"  // 下拉列表背景色
        "   selection-color: white;"  // 选中项文字颜色
        "   selection-background-color: #444444;"  // 选中项背景色
        "}"
        );
    mainLayout->setContentsMargins(0, 10, 0, 10);  // 移除内边距
}

QString FormatOption::getSelectedFormat() const
{
    qDebug()<<"1111111111111111"<<comboBox->currentText();
    return comboBox->currentText();
}
