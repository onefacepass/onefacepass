#ifndef CAMERA_H
#define CAMERA_H

#include <QCamera>
#include <QCameraViewfinder>
#include <QAction>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QList>
#include <QCameraInfo>

class Camera : QCamera
{
//    Q_OBJECT

public:
    Camera();
    QList<QCameraInfo> get_available_camera_infos();
    QCameraInfo get_default_camera();
    void set_current_camera(const QCameraInfo &camera);
    QCamera* get_current_camera();
    void start();
    void stop();
    void setViewFinder(QCameraViewfinder* viewfinder);

public slots:

private:
    QScopedPointer<QCamera>             camera;
    QList<QCameraInfo>                  available_camera_infos;
    QScopedPointer<QCameraImageCapture> image_capture;
    QScopedPointer<QMediaRecorder>      media_recoder;

    QImageEncoderSettings               image_settings;
    QAudioEncoderSettings               audio_settings;
    QVideoEncoderSettings               video_settings;
    QString                             video_container_format;

    bool                                is_capturing_image;
    bool                                application_exting;
};

#endif // CAMERA_H
