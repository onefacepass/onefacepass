

#include "camera.h"



Camera::Camera()
{

   available_camera_infos = QCameraInfo::availableCameras();


}

QList<QCameraInfo> Camera::get_available_camera_infos()
{
    return available_camera_infos;
}

QCameraInfo Camera::get_default_camera()
{
    return QCameraInfo::defaultCamera();
}

void Camera::set_current_camera(const QCameraInfo &cameraInfo)
{
    this->camera.reset(new QCamera(cameraInfo));
}


QCamera* Camera::get_current_camera()
{
    return this->camera.data();
}

void Camera::start()
{
    this->camera->start();
}

void Camera::setViewFinder(QCameraViewfinder* viewfinder)
{
    this->camera->setViewfinder(viewfinder);
}
