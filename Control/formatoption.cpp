#include "formatoption.h"
#include <QDebug>

QString savaFormat;

FormatOption::FormatOption(QWidget *parent) : QWidget(parent)
{
    initControl();
    initLayout();
    savaFormat = comboBox->currentText();

    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(formatChanged(QString)));
    connect(comboBox, &QComboBox::currentTextChanged, this, [](const QString &format) {
        savaFormat = format;
        qDebug() << "save video format changed:" << savaFormat;
    });
}

void FormatOption::initControl()
{
    lab = new QLabel(QString::fromUtf8(u8"录像格式"), this);
    comboBox = new QComboBox(this);
    comboBox->addItem(".mp4");
    comboBox->addItem(".flv");
    comboBox->addItem(".avi");
    comboBox->addItem(".wmv");
}

void FormatOption::initLayout()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 10, 0, 10);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(lab);
    mainLayout->addWidget(comboBox);

    setStyleSheet(
        "FormatOption { border: 1px solid #334155; border-radius: 8px; background-color: #182235; color: white; }"
        "FormatOption QLabel { color: #e2e8f0; padding: 4px 12px; background-color: #0f172a; border: 1px solid #263244; border-radius: 6px; }"
        "FormatOption QComboBox { min-height: 30px; color: #e2e8f0; background-color: #0f172a; border: 1px solid #334155; padding: 0 10px; border-radius: 6px; }"
        "FormatOption QComboBox:hover { border-color: #22d3ee; }"
        "FormatOption QComboBox QAbstractItemView { color: #e2e8f0; background-color: #111827; selection-background-color: #0f766e; }");
}

QString FormatOption::getSelectedFormat() const
{
    return comboBox->currentText();
}
