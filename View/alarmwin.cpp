#include "alarmwin.h"

#include "data/userstorage.h"

#include <QColor>
#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStorageInfo>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace
{
QString pendingText()
{
    return QString::fromUtf8(u8"待处理");
}

QString handledText()
{
    return QString::fromUtf8(u8"已处理");
}

QMySqlite *alarmDb()
{
    return QMySqlite::getInstance(UserStorage::recordDbPath());
}
}

AlarmWin::AlarmWin(QWidget *parent) : QWidget(parent)
{
    setObjectName("alarmWin");
    setAutoFillBackground(true);
    buildUi();
    applyStyle();
    refreshRealSources();
    refreshTable();
}

void AlarmWin::buildUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(14);

    QWidget *header = new QWidget(this);
    header->setObjectName("header");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(18, 14, 18, 14);
    headerLayout->setSpacing(12);

    QLabel *titleLabel = new QLabel(QString::fromUtf8(u8"告警中心"), header);
    titleLabel->setObjectName("titleLabel");
    hintLabel = new QLabel(QString::fromUtf8(u8"联动预览、录像与用户存储，展示真实运行异常和处理状态"), header);
    hintLabel->setObjectName("hintLabel");

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(4);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(hintLabel);

    filterCombo = new QComboBox(header);
    filterCombo->addItem(QString::fromUtf8(u8"全部告警"));
    filterCombo->addItem(pendingText());
    filterCombo->addItem(handledText());

    refreshButton = new QPushButton(QString::fromUtf8(u8"刷新"), header);
    handleButton = new QPushButton(QString::fromUtf8(u8"标记处理"), header);
    realRefreshButton = new QPushButton(QString::fromUtf8(u8"检测当前状态"), header);

    headerLayout->addLayout(titleLayout, 1);
    headerLayout->addWidget(filterCombo);
    headerLayout->addWidget(refreshButton);
    headerLayout->addWidget(handleButton);
    headerLayout->addWidget(realRefreshButton);

    QWidget *stats = new QWidget(this);
    stats->setObjectName("stats");
    stats->setAttribute(Qt::WA_StyledBackground, true);
    QHBoxLayout *statsLayout = new QHBoxLayout(stats);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(12);

    auto createStatCard = [stats](const QString &title, const QString &objectName, QLabel **valueLabel) {
        QFrame *card = new QFrame(stats);
        card->setObjectName("statCard");
        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(18, 14, 18, 14);
        layout->setSpacing(6);
        QLabel *titleLabel = new QLabel(title, card);
        titleLabel->setObjectName("statTitle");
        *valueLabel = new QLabel("0", card);
        (*valueLabel)->setObjectName(objectName);
        layout->addWidget(titleLabel);
        layout->addWidget(*valueLabel);
        return card;
    };

    statsLayout->addWidget(createStatCard(QString::fromUtf8(u8"告警总数"), "totalValue", &totalValueLabel));
    statsLayout->addWidget(createStatCard(QString::fromUtf8(u8"待处理"), "pendingValue", &pendingValueLabel));
    statsLayout->addWidget(createStatCard(QString::fromUtf8(u8"已处理"), "handledValue", &handledValueLabel));

    table = new QTableWidget(this);
    table->setObjectName("alarmTable");
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList()
                                     << QString::fromUtf8(u8"时间")
                                     << QString::fromUtf8(u8"级别")
                                     << QString::fromUtf8(u8"来源")
                                     << QString::fromUtf8(u8"类型")
                                     << QString::fromUtf8(u8"状态"));
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    mainLayout->addWidget(header);
    mainLayout->addWidget(stats);
    mainLayout->addWidget(table, 1);

    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AlarmWin::refreshTable);
    connect(refreshButton, &QPushButton::clicked, this, &AlarmWin::refreshTable);
    connect(realRefreshButton, &QPushButton::clicked, this, [this]() {
        refreshRealSources();
        refreshTable();
    });
    connect(handleButton, &QPushButton::clicked, this, [this]() {
        QList<QTableWidgetItem*> selected = table->selectedItems();
        if (selected.isEmpty())
        {
            return;
        }
        int alarmId = table->item(selected.first()->row(), 0)->data(Qt::UserRole).toInt();
        if (alarmId > 0)
        {
            alarmDb()->updateAlarmStatus(alarmId, handledText());
            refreshTable();
        }
    });
}

void AlarmWin::applyStyle()
{
    setStyleSheet(
        "#alarmWin { background-color: #0f172a; }"
        "#alarmWin QWidget { background-color: #0f172a; }"
        "#header { background-color: #111827; border: 1px solid #263244; border-radius: 8px; }"
        "#stats { background-color: #0f172a; border: none; }"
        "#titleLabel { color: #f8fafc; font-size: 24px; font-weight: 800; background: transparent; }"
        "#hintLabel { color: #94a3b8; font-size: 13px; background: transparent; }"
        "#statCard { background-color: #111827; border: 1px solid #263244; border-radius: 8px; }"
        "#statTitle { color: #94a3b8; font-size: 13px; background: transparent; }"
        "#totalValue, #pendingValue, #handledValue { color: #f8fafc; font-size: 28px; font-weight: 800; background: transparent; }"
        "#pendingValue { color: #fbbf24; }"
        "#handledValue { color: #5eead4; }"
        "QPushButton { min-height: 36px; border: 1px solid #334155; border-radius: 8px; padding: 0 14px; color: #e2e8f0; background-color: #182235; font-weight: 700; }"
        "QPushButton:hover { color: #ffffff; background-color: #233148; border-color: #22d3ee; }"
        "QPushButton:pressed { background-color: #0f766e; border-color: #5eead4; }"
        "QComboBox { min-height: 36px; border: 1px solid #334155; border-radius: 8px; padding-left: 10px; color: #f8fafc; background-color: #0f172a; }"
        "QComboBox:hover { border-color: #22d3ee; }"
        "QComboBox::drop-down { width: 24px; border: none; }"
        "QComboBox QAbstractItemView { background-color: #111827; color: #f8fafc; selection-background-color: #0f766e; }"
        "#alarmTable { background-color: #111827; alternate-background-color: #151f31; border: 1px solid #263244; border-radius: 8px; gridline-color: #263244; color: #e2e8f0; }"
        "#alarmTable::viewport { background-color: #111827; }"
        "QTableWidget::item { padding: 8px; border: none; }"
        "QTableWidget::item:selected { background-color: #0f766e; color: white; }"
        "QHeaderView::section { background-color: #182235; color: #dbeafe; border: none; border-bottom: 1px solid #334155; padding: 10px; font-weight: 800; }"
        "QScrollBar:vertical { background: #111827; width: 8px; margin: 0; border-radius: 4px; }"
        "QScrollBar::handle:vertical { background: #475569; border-radius: 4px; min-height: 28px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }");
}

void AlarmWin::addRealtimeAlarm(const QString &level, const QString &source, const QString &type)
{
    alarmDb()->insertAlarmRecord(level, source, type, pendingText());
    refreshTable();
}

void AlarmWin::refreshRealSources()
{
    UserStorage::ensureCurrentUserDirs();

    QStorageInfo storage(UserStorage::videoDir());
    if (storage.isValid() && storage.isReady())
    {
        const qint64 freeMb = storage.bytesAvailable() / 1024 / 1024;
        if (freeMb < 512)
        {
            alarmDb()->insertAlarmRecord(QString::fromUtf8(u8"高"),
                                         QString::fromUtf8(u8"用户存储目录"),
                                         QString::fromUtf8(u8"磁盘空间不足"),
                                         pendingText());
        }
    }

    QDir videoDir(UserStorage::videoDir());
    if (!videoDir.exists())
    {
        alarmDb()->insertAlarmRecord(QString::fromUtf8(u8"中"),
                                     QString::fromUtf8(u8"录像存储"),
                                     QString::fromUtf8(u8"录像目录不可用"),
                                     pendingText());
    }
}

void AlarmWin::refreshTable()
{
    const QString filter = filterCombo->currentText();
    alarms = alarmDb()->queryAlarmRecords(filter);

    table->setRowCount(0);
    for (int i = 0; i < alarms.size(); ++i)
    {
        const AlarmDbRecord &record = alarms.at(i);
        int row = table->rowCount();
        table->insertRow(row);
        QStringList values;
        values << record.time << record.level << record.source << record.type << record.status;
        for (int col = 0; col < values.size(); ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(values.at(col));
            item->setData(Qt::UserRole, record.id);
            if (col == 1)
            {
                if (record.level == QString::fromUtf8(u8"高")) item->setForeground(QColor("#f87171"));
                if (record.level == QString::fromUtf8(u8"中")) item->setForeground(QColor("#fbbf24"));
                if (record.level == QString::fromUtf8(u8"低")) item->setForeground(QColor("#93c5fd"));
            }
            if (col == 4 && record.status == handledText())
            {
                item->setForeground(QColor("#5eead4"));
            }
            table->setItem(row, col, item);
        }
        table->setRowHeight(row, 44);
    }
    updateStats();
}

void AlarmWin::updateStats()
{
    QVector<AlarmDbRecord> allAlarms = alarmDb()->queryAlarmRecords();
    int pending = 0;
    int handled = 0;
    for (int i = 0; i < allAlarms.size(); ++i)
    {
        if (allAlarms.at(i).status == handledText())
        {
            ++handled;
        }
        else
        {
            ++pending;
        }
    }
    totalValueLabel->setText(QString::number(allAlarms.size()));
    pendingValueLabel->setText(QString::number(pending));
    handledValueLabel->setText(QString::number(handled));
}
