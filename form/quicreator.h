﻿#ifndef QUICREATOR_H
#define QUICREATOR_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QScopedPointer>
#include <QCameraInfo>
#include <QSettings>

#include "stuwidget.h"
#include "FaceDete.h"
#include "capturethread.h"
#include "facedetethread.h"

class QPushButton;
class QLineEdit;

namespace Ui
{
class QUICreator;
}

class QUICreator : public QMainWindow
{
    Q_OBJECT

public:
    explicit QUICreator(QWidget *parent = nullptr, const QString& config_file = ":/config.ini");
    ~QUICreator();


private:
    int MAX_RESULT = 5;
    Ui::QUICreator *ui;
    QImage img_tmp;
    QSettings *config;

    QScopedPointer<QCamera> camera;
    QScopedPointer<QCameraImageCapture> imageCapture;

    CaptureThread *captureThread;
    FaceDeteThread *faceThread;

    QVector<std::shared_ptr<StuWidget>> stuWidgets;

private:
    void insertLog(const QString& log);

    // 打印摄像头支持的分辨率
    void debug_show_supported_viewfinder_resolutions();
    void debug_show_student_info(Student s);


private slots:
    void initForm();
    void initCamera();
    void initNav();
    void initAction();
    void initFace();
    void initStudentWidget();
    void initOther();

    void setStyle(const QString &str);
    void takeImage();
    void processCapturedImage(int requestId, const QImage& img);
    void doFaceDetect();
    void doFaceTrack();

    void setCamera(const QCameraInfo &camera_info);
    void updateCamera(QAction *action);
    void displayCameraError();
    void resetConsumption();
    void btnPayClicked();
    void btnReadyPayClicked();
    void navBtnClicked();

    void initStyle();
    void about();

    void faceDetectFinished(QVector<Student> res);
    void faceTrackFinished(QVector<QRect> res);
    void faceDetectFinishedWithoutResult();
    void faceTrackFinishedWithoutResult();
};

#endif // QUICREATOR_H
