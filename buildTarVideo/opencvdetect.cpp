#include "opencvdetect.h"
#include <QDebug>
#include <cstring>

openCVDetect::openCVDetect(QObject *parent) : QObject(parent)
{
    initFaceDetection();
}

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
    return detectAndDraw(img, true, true);
}

QImage openCVDetect::detectAndDraw(QImage img, bool faceEnabled, bool runFaceDetection)
{
    if (img.format() != QImage::Format_RGB888)
    {
        img = img.convertToFormat(QImage::Format_RGB888);
    }

    cv::Mat frame(img.height(), img.width(), CV_8UC3, (void*)img.bits(), img.bytesPerLine());
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);

    if (faceEnabled && runFaceDetection)
    {
        updateFaceDetection(frame);
    }
    else if (!faceEnabled)
    {
        lastFaces.clear();
    }

    if (faceEnabled)
    {
        for (const auto &face : lastFaces)
        {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }
    }

    return cvMatToQImage(frame);
}

void openCVDetect::updateFaceDetection(const cv::Mat &frame)
{
    if (frame.empty())
    {
        lastFaces.clear();
        return;
    }

    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayFrame, grayFrame);

    const int targetWidth = 640;
    double scale = grayFrame.cols > targetWidth ? static_cast<double>(targetWidth) / grayFrame.cols : 1.0;
    cv::Mat detectFrame;
    if (scale < 1.0)
    {
        cv::resize(grayFrame, detectFrame, cv::Size(), scale, scale, cv::INTER_LINEAR);
    }
    else
    {
        detectFrame = grayFrame;
    }

    std::vector<cv::Rect> detectedFaces;
    faceDetector.detectMultiScale(detectFrame, detectedFaces, 1.08, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(22, 22));

    lastFaces.clear();
    for (const auto &face : detectedFaces)
    {
        if (scale < 1.0)
        {
            lastFaces.push_back(cv::Rect(
                static_cast<int>(face.x / scale),
                static_cast<int>(face.y / scale),
                static_cast<int>(face.width / scale),
                static_cast<int>(face.height / scale)));
        }
        else
        {
            lastFaces.push_back(face);
        }
    }
}

QImage openCVDetect::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    return QImage();
}

void openCVDetect::addImage(QImage &img)
{
    image = img;
}

QImage openCVDetect::deepCopyImage(const QImage &source)
{
    if (source.isNull())
    {
        return QImage();
    }

    QImage image(source.size(), source.format());
    if (source.format() == QImage::Format_Indexed8)
    {
        std::memcpy(image.bits(), source.constBits(), source.byteCount());
        image.setColorTable(source.colorTable());
    }
    else
    {
        std::memcpy(image.bits(), source.constBits(), source.bytesPerLine() * source.height());
    }

    return image;
}
