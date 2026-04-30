#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
#include <QCameraInfo>
#include<QString>
class MyCamera : public QObject
{
    Q_OBJECT
public:
    // 构造摄像头信息读取对象。
    explicit MyCamera(QObject *parent = nullptr);
    QList<QCameraInfo> infos;//摄像头信息
    // 按序号返回摄像头设备描述，序号无效时返回空字符串。
    QString getCameraInfo(int i);
signals:

};

#endif // MYCAMERA_H
