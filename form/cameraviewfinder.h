#ifndef CAMERAVIEWFINDER_H
#define CAMERAVIEWFINDER_H

#include <QCameraViewfinder>

class CameraViewfinder : public QCameraViewfinder
{
public:
    CameraViewfinder(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *e);
};

#endif // CAMERAVIEWFINDER_H
