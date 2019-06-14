#include <QPainter>

#include "cameraviewfinder.h"

CameraViewfinder::CameraViewfinder(QWidget *parent)
    : QCameraViewfinder(parent)
{

}

CameraViewfinder::~CameraViewfinder()
{
    while (!q_log.isEmpty()) {
        q_log.dequeue();
    }
}

void CameraViewfinder::paintEvent(QPaintEvent *e)
{
    QCameraViewfinder::paintEvent(e);

    QPainter painter(this);
    painter.setPen(Qt::red);

    if (!q_log.empty()) {
        QString log = this->q_log.dequeue();

        painter.drawText(10, 10, log);
    }

}

void CameraViewfinder::insertLog(const QString &log)
{
    this->q_log.enqueue(log);
}


