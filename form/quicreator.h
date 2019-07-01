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

    QScopedPointer<QCamera> camera;
    QScopedPointer<QCameraImageCapture> imageCapture;

    FaceDete* facedete;
    CaptureThread *captureThread;

private:
    void insertLog(const QString& log);
    void displayLogOnCamera(const QString& log);
    void debugFunc();           // TODO
    cv::Mat QImage2Mat(QImage const& src);
    int faceRect[4];

private slots:
    void initForm();
    void initCamera();
    void initHistoryWidget();
    void initNav();
    void initAction();
    void initFace();
    void initOther();

    void startAndStopCamera();
    void setStyle(const QString &str);
    void takeImage();
    void processCapturedImage(int requestId, const QImage& img);

    void setCamera(const QCameraInfo &camera_info);
    void updateCamera(QAction *action);
    void displayCameraError();
    void resetConsumption();
    void btnPayClicked();
    void btnReadyPayClicked();
    void navBtnClicked();

    void initStyle();
    void about();
};

#endif // QUICREATOR_H
