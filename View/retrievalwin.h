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
    // 构造录像检索页面。
    explicit RetrievalWin(QWidget *parent = nullptr);
    // 初始化检索页控件。
    void initialize_control();
    // 组装检索页控件布局。
    void addControl_control();
    // 按关键字扫描目录并添加文件结果。
    void addFileList(QString &directoryPath, QStringList &keywords);
    // 根据数据库录像记录生成检索结果列表。
    void addRecordList(const QVector<VideoRecord> &records);
    void applyModernUi();
    // 在目录中按关键字查找文件。
    QStringList findFilesByKeywords(QString &directoryPath, QStringList &keywords, bool recursive = true);
    // 判断多个格式下拉框是否存在重复选项。
    bool repeatedJudgment(QList<QComboBox *> comboBoxList);
    // 清空当前检索结果列表。
    void clear_FileList();
    // 请求回放指定索引的录像。
    void playRecordAt(int index);
    // 处理结果项双击等事件。
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    // 将选中的录像文件和摄像头名称发送给回放页。
    void to_save(QStringList fileName, QStringList cameraName);
    // 请求主窗口切换到回放页面。
    void requestReplayPage();

public slots:
    // 同步回放页传来的摄像头列表。
    void camerasNum_change_retrievalwin(QList<QPushButton *>);
    // 根据搜索条件刷新检索结果。
    void refresh();
    // 增加一个格式筛选条件。
    void addition_fileFormat();
    // 删除一个格式筛选条件。
    void subtraction_fileFormat();
    // 保存当前勾选结果并进入回放。
    void save();
};

#endif // RETRIEVALWIN_H
