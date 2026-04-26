#include "opencvdetect.h"
#include<QDebug>
openCVDetect::openCVDetect(QObject *parent) : QObject(parent)
{
    initFaceDetection();


}
//初始化人脸检测器
bool openCVDetect::initFaceDetection()
{
    QString modelPath = "./etc/haarcascades/haarcascade_frontalface_default.xml";
    if (!faceDetector.load(modelPath.toStdString()))
    {
        qDebug() << "Error: Could not load face detection model!";
        return false;
    }
    return true;
}

QImage openCVDetect::ImageTocvMat(QImage img)
{

    // 1. 确保输入是 RGB 格式（如果不是，转换）
    if (img.format() != QImage::Format_RGB888)
    {
        img = img.convertToFormat(QImage::Format_RGB888);
    }
    qDebug() << "Image size:" << img.width() << "x" << img.height();
    // 2. QImage → cv::Mat (RGB)
    cv::Mat frame = cv::Mat(img.height(), img.width(), CV_8UC3, (void*)img.bits(), img.bytesPerLine());

    // 3. RGB → BGR（因为 OpenCV 默认是 BGR）
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);

    // 4. 人脸检测（仅在灰度图上运行）
    std::vector<cv::Rect> faces;
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY); // 转为灰度图
    faceDetector.detectMultiScale(grayFrame, faces, 1.1, 3, 0, cv::Size(30, 30));

    // 5. 在原始彩色图（frame）上绘制人脸框
    for (const auto& face : faces) {
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2); // 在 BGR 图上画绿色框
    }

    // 6. 转换回 QImage（BGR → RGB）
    return cvMatToQImage(frame); // 确保 cvMatToQImage 正确处理 BGR→RGB
}

QImage openCVDetect::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC3) { // BGR 格式
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB); // BGR → RGB
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    return QImage(); // 其他情况返回空图
}

QImage openCVDetect::deepCopyImage(const QImage &source)
{
    if (source.isNull()) {
          return QImage();
      }

      // 创建新图像并复制内容
      QImage image(source.size(), source.format());

      // 复制像素数据
      if (source.format() == QImage::Format_Indexed8) {
          // 8位图像处理
          std::memcpy(image.bits(), source.constBits(), source.byteCount());
          image.setColorTable(source.colorTable());
      } else {
          // 其他格式处理
          std::memcpy(image.bits(), source.constBits(), source.bytesPerLine() * source.height());
      }

      return image;
}


