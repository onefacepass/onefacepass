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
    QList<QWidget *> frms;      // TODO: 已注册学生信息的展示面板

    QString currentFile;        //当前QSS文件
    QString currentQSS;         //当前QSS文本


private slots:
    void initForm();
    void initCamera();
    void initHistoryWidget();
    void initNav();
    void initOther();

    void startAndStopCamera();
    void setStyle(const QString &str);

private:
    void insertLog(const QString& log);
    void displayLogOnCamera(const QString& log);
    void debugFunc();           // TODO


private slots:
    void setCamera(const QCameraInfo &cameraInfo);
    void updateCamera(QAction *action);
    void displayCameraError();
    void resetConsumption();
    void btnPayClicked();
    void btnReadyPayClicked();
    void navBtnClicked();

private slots:
    void initStyle();
};

#endif // QUICREATOR_H
