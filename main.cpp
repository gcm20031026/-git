#include "view/mainwindow.h"
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
    a.QApplication::setStyle("Fusion"); //令控件不受系统影响
//    CTranceToDest *de = new CTranceToDest("./savaVideos/0.mp4","./savaVideos/0.h264") ;
//    de->generateDestFile();

    MainWindow w;
    w.show();
//    JdtMyPushButton w;
//    w.show();
//    ConfigurationWin w;
//    w.show();

//    FormatOption w;
//    w.show();


    return a.exec();
}
