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


    if (!is_camera_running) {   // fixme
        qDebug() << "camera stopped";
        QPixmap pix(this->width(), this->height());
        pix.fill(Qt::black);
        painter.drawPixmap(10, 10, pix);

        return;
    }

    painter.setPen(Qt::red);
    painter.drawRect(rect[0], rect[1], rect[2],  rect[3]);

    if (!q_log.empty()) {
        QString log = this->q_log.dequeue();

        painter.drawText(10, 10, log);
    }

}

void CameraViewfinder::insertLog(const QString &log)
{
    this->q_log.enqueue(log);
}

void CameraViewfinder::startCamera()
{
    this->is_camera_running = true;
}

void CameraViewfinder::stopCamera()
{
    this->is_camera_running = false;
}

void CameraViewfinder::ReceiveRect(int *r)
{
    memcpy_s(rect, 4*sizeof (int), r, 4*sizeof (int));
}

