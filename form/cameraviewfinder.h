#ifndef CAMERAVIEWFINDER_H
#define CAMERAVIEWFINDER_H

#include <QCameraViewfinder>
#include <QQueue>
#include <QString>



class CameraViewfinder : public QCameraViewfinder
{
private:
    QVector<QRect> rects;

protected:
    void paintEvent(QPaintEvent *e);

public:
    CameraViewfinder(QWidget *parent = nullptr);
    ~CameraViewfinder();

    void ReceiveRects(QVector<QRect> rects);
};

#endif // CAMERAVIEWFINDER_H
