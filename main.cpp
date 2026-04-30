#include "view/mainwindow.h"
#include "view/loginwindow.h"
#include "data/userstorage.h"
#include <QApplication>
#include<QDebug>
#include"Control/jdtmypushbutton.h"
#include"Control/formatoption.h"
#include<View/configurationwin.h>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include"buildTarVideo/opencvdetect.h"
#include"buildTarVideo/ctrancetodest.h"
#include<QImage>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}
using namespace cv;
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(
        "QToolTip { color: #f8fafc; background-color: #111827; border: 1px solid #334155; border-radius: 4px; padding: 6px; }"
        "QMessageBox { background-color: #111827; color: #e2e8f0; }"
        "QMessageBox QLabel { color: #e2e8f0; }"
        "QMessageBox QPushButton { min-width: 72px; min-height: 30px; border-radius: 8px; border: 1px solid #334155; background-color: #182235; color: #f8fafc; }"
        "QMessageBox QPushButton:hover { background-color: #0f766e; border-color: #5eead4; }"
    );
    a.QApplication::setStyle("Fusion"); //令控件不受系统影响
//    CTranceToDest *de = new CTranceToDest("./savaVideos/0.mp4","./savaVideos/0.h264") ;
//    de->generateDestFile();

    QMySqlite *database = QMySqlite::getInstance(QApplication::applicationDirPath() + "/securityDB.db");
    database->initUserTable();

    LoginWindow loginWindow(database);
    if (loginWindow.exec() != QDialog::Accepted)
    {
        return 0;
    }
    UserStorage::setCurrentUser(loginWindow.currentUser());

    MainWindow w;
    w.setWindowTitle(QString::fromUtf8(u8"智能安防监控系统 - %1").arg(loginWindow.currentUser().displayName));
    w.show();
//    JdtMyPushButton w;
//    w.show();
//    ConfigurationWin w;
//    w.show();

//    FormatOption w;
//    w.show();


    return a.exec();
}
