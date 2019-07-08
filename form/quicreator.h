#ifndef QUICREATOR_H
#define QUICREATOR_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QScopedPointer>
#include <QCameraInfo>

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
    explicit QUICreator(QWidget *parent = nullptr);
    ~QUICreator();

private:
    Ui::QUICreator *ui;
    QImage img_tmp;

    QScopedPointer<QCamera> camera;
    QScopedPointer<QCameraImageCapture> imageCapture;

    CaptureThread *captureThread;
    FaceDeteThread *faceThread;

private:
    void insertLog(const QString& log);
    void displayLogOnCamera(const QString& log);
    void debugFunc();           // TODO

    // 打印摄像头支持的分辨率
    void debug_show_supported_viewfinder_resolutions();


private slots:
    void initForm();
    void initCamera();
    void initHistoryWidget();
    void initNav();
    void initAction();
    void initFace();
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

    void debug_show_detect_result(Student res);
    void faceDetectFinished(QList<Student> res);
    void faceTrackFinished(QVector<QRect> res);
    void faceDetectFinishedWithoutResult();
    void faceTrackFinishedWithoutResult();
};

#endif // QUICREATOR_H
