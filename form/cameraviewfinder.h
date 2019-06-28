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
    bool is_camera_running = true;

protected:
    void paintEvent(QPaintEvent *e);

public:
    CameraViewfinder(QWidget *parent = nullptr);
    ~CameraViewfinder();

    void insertLog(const QString& log);
    void startCamera();
    void stopCamera();

};

#endif // CAMERAVIEWFINDER_H
