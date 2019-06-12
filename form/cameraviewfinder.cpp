#include <QPainter>

#include "cameraviewfinder.h"

CameraViewfinder::CameraViewfinder(QWidget *parent)
    : QCameraViewfinder(parent)
{

}

void CameraViewfinder::paintEvent(QPaintEvent *e)
{
    QCameraViewfinder::paintEvent(e);

    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(50, 50, 100, 100);
}
