#include "mycamera.h"
#include<QList>
#include<QDebug>

MyCamera::MyCamera(QObject *parent) : QObject(parent)
{
    // 获取系统可用摄像头信息
    infos = QCameraInfo::availableCameras();
}

QString MyCamera::getCameraInfo(int i)
{
    // 先检查索引是否越界
    if (i < 0 || i >= infos.size())
    {
        qWarning() << "Camera index out of range:" << i;
        return "";  // 返回空字符串
    }
    return infos.at(i).description();
}
