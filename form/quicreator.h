#ifndef QUICREATOR_H
#define QUICREATOR_H

#include <QMainWindow>
#include "camera.h"

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

    Camera* camera;

private:
    void insertLog(const QString& log);
    void displayLogOnCamera(const QString& log);
    void debugFunc();           // TODO


private slots:
    void initForm();
    void initCamera();
    void initHistoryWidget();
    void initNav();
    void initOther();

    void startAndStopCamera();
    void setStyle(const QString &str);

    void setCamera(const QCameraInfo &camera_info);
    void updateCamera(QAction *action);
    void displayCameraError();
    void resetConsumption();
    void btnPayClicked();
    void btnReadyPayClicked();
    void navBtnClicked();

    void initStyle();
};

#endif // QUICREATOR_H
