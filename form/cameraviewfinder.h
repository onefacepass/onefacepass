#ifndef CAMERAVIEWFINDER_H
#define CAMERAVIEWFINDER_H

#include <QCameraViewfinder>
#include <QQueue>
#include <QString>


// TODO: q_log 会有竞争问题么？？？？

class CameraViewfinder : public QCameraViewfinder
{
private:
    QQueue<QString> q_log;



public:
    CameraViewfinder(QWidget *parent = nullptr);
    void insertLog(const QString& log);

    void paintEvent(QPaintEvent *e);
};

#endif // CAMERAVIEWFINDER_H
