#ifndef RETRIEVALWIN_H
#define RETRIEVALWIN_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>

class RetrievalWin : public QWidget //检索窗口
{
    Q_OBJECT
private:
    QString Path01;
    QStringList Path02,cameraList_fileName,cameraList_cameraName;
    QVBoxLayout *mainLayout,*mianUpAndDownLayout,*listOne_outSide_Layout,*listOne_InSide_Layout;
    QHBoxLayout *UpWin_Layout;
    QWidget *mainWin,*UpWin,*DownWin,*listOne_Win;
    QScrollArea *listOne;
    QList<QWidget*>FileListWin;
    QList<QComboBox*>comboBoxList;
    QFont font01,font02,font03;
    QPushButton *yesButton,*refreshButton,*additionBtn,*subtractionBtn;
    QList<QLabel*>yesfileName;
    QLineEdit *searchEdit;
    QComboBox *fileFormat; //文件格式
    QWidget *HintWin; //提示模块
public:
    explicit RetrievalWin(QWidget *parent = nullptr);
    void initialize_control();
    void addControl_control();
    void addFileList(QString &directoryPath, QStringList &keywords);
    QStringList findFilesByKeywords(QString &directoryPath,QStringList &keywords,bool recursive = true); //关键词查找文件，返回的路径是绝对还是相对取决于传入的目录（都是在bin目录下）
    bool repeatedJudgment(QList<QComboBox*>comboBoxList);
    void clear_FileList(); //移除文件列表的控件
signals:
    void to_save(QStringList fileName,QStringList cameraName);
public slots:
    void camerasNum_change_retrievalwin(QList<QPushButton*>);
    void refresh();
    void addition_fileFormat();
    void subtraction_fileFormat();
    void save();
};

#endif // RETRIEVALWIN_H
