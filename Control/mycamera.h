#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
#include <QCameraInfo>
#include<QString>
class MyCamera : public QObject
{
    Q_OBJECT
public:
    explicit MyCamera(QObject *parent = nullptr);
    QList<QCameraInfo> infos;//摄像头信息
    QString getCameraInfo(int i);
signals:

};

#endif // MYCAMERA_H
