#ifndef OPENCVDETECT_H
#define OPENCVDETECT_H

#include <QObject>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include<QImage>
class openCVDetect : public QObject
{
    Q_OBJECT
public:
    explicit openCVDetect(QObject *parent = nullptr);
    bool initFaceDetection();            // 初始化人脸检测
    QImage ImageTocvMat(QImage img);
    QImage cvMatToQImage(const cv::Mat& mat); // 转换 cv::Mat 到 QImage
    void addImage(QImage &img);
    QImage deepCopyImage(const QImage &source);
private:
        cv::CascadeClassifier faceDetector;  // 人脸检测器
        QImage image;

signals:

};

#endif // OPENCVDETECT_H
