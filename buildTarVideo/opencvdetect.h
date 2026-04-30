#ifndef OPENCVDETECT_H
#define OPENCVDETECT_H

#include <QObject>
#include <QImage>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

class openCVDetect : public QObject
{
    Q_OBJECT
public:
    // 构造 OpenCV 检测对象并加载人脸模型。
    explicit openCVDetect(QObject *parent = nullptr);
    bool initFaceDetection(); // 加载 Haar 人脸检测模型。
    QImage ImageTocvMat(QImage img); // 兼容旧接口：输入 QImage 并执行一次人脸检测绘制。
    QImage detectAndDraw(QImage img, bool faceEnabled, bool runFaceDetection); // 按开关状态检测人脸并绘制绿色检测框。
    QImage cvMatToQImage(const cv::Mat &mat); // 将 OpenCV Mat 转换为 Qt QImage。
    void addImage(QImage &img); // 缓存一帧图像。
    QImage deepCopyImage(const QImage &source); // 深拷贝 QImage，避免外部缓冲区释放导致图像失效。

private:
    void updateFaceDetection(const cv::Mat &frame); // 更新当前帧的人脸检测结果。

    cv::CascadeClassifier faceDetector;
    QImage image;
    std::vector<cv::Rect> lastFaces;

signals:
};

#endif // OPENCVDETECT_H
