#ifndef RETRIEVALWIN_H
#define RETRIEVALWIN_H

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>
#include "data/qmysqlite.h"

class RetrievalWin : public QWidget
{
    Q_OBJECT
private:
    QString Path01;
    QStringList Path02, cameraList_fileName, cameraList_cameraName;
    QVBoxLayout *mainLayout, *mianUpAndDownLayout, *listOne_outSide_Layout, *listOne_InSide_Layout;
    QHBoxLayout *UpWin_Layout;
    QWidget *mainWin, *UpWin, *DownWin, *listOne_Win;
    QScrollArea *listOne;
    QList<QWidget *> FileListWin;
    QList<QCheckBox *> checkBoxList;
    QVector<VideoRecord> videoRecords;
    QList<QComboBox *> comboBoxList;
    QFont font01, font02, font03;
    QPushButton *yesButton, *refreshButton, *additionBtn, *subtractionBtn;
    QList<QLabel *> yesfileName;
    QLineEdit *searchEdit;
    QComboBox *fileFormat;
    QWidget *HintWin;

public:
    explicit RetrievalWin(QWidget *parent = nullptr);
    void initialize_control();
    void addControl_control();
    void addFileList(QString &directoryPath, QStringList &keywords);
    void addRecordList(const QVector<VideoRecord> &records);
    QStringList findFilesByKeywords(QString &directoryPath, QStringList &keywords, bool recursive = true);
    bool repeatedJudgment(QList<QComboBox *> comboBoxList);
    void clear_FileList();
    void playRecordAt(int index);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void to_save(QStringList fileName, QStringList cameraName);
    void requestReplayPage();

public slots:
    void camerasNum_change_retrievalwin(QList<QPushButton *>);
    void refresh();
    void addition_fileFormat();
    void subtraction_fileFormat();
    void save();
};

#endif // RETRIEVALWIN_H
