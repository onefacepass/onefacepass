#include <QPainter>

#include "cameraviewfinder.h"

CameraViewfinder::CameraViewfinder(QWidget *parent)
    : QCameraViewfinder(parent)
{

}

CameraViewfinder::~CameraViewfinder()
{

}

void CameraViewfinder::paintEvent(QPaintEvent *e)
{
    QCameraViewfinder::paintEvent(e);

    QPainter painter(this);

    painter.setPen(Qt::red);
    painter.drawRects(rects);
}


void CameraViewfinder::ReceiveRects(QVector<QRect> _rects)
{
    rects.clear();
    rects.append(_rects);
}

