#ifndef ALARMWIN_H
#define ALARMWIN_H

#include <QWidget>
#include <QVector>
#include "data/qmysqlite.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;

class AlarmWin : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmWin(QWidget *parent = nullptr);
    void addRealtimeAlarm(const QString &level, const QString &source, const QString &type);

public slots:
    void refreshTable();

private:
    void buildUi();
    void applyStyle();
    void refreshRealSources();
    void updateStats();

    QVector<AlarmDbRecord> alarms;
    QLabel *totalValueLabel = nullptr;
    QLabel *pendingValueLabel = nullptr;
    QLabel *handledValueLabel = nullptr;
    QLabel *hintLabel = nullptr;
    QComboBox *filterCombo = nullptr;
    QPushButton *realRefreshButton = nullptr;
    QPushButton *handleButton = nullptr;
    QPushButton *refreshButton = nullptr;
    QTableWidget *table = nullptr;
};

#endif // ALARMWIN_H
