#ifndef FACEDETETHREAD_H
#define FACEDETETHREAD_H

#include <QMutex>
#include <QImage>
#include <QList>
//#include <QRunnable>
//#include <QThreadPool>

#include "FaceDete.h"


typedef struct t_student {
    QString id;
    QString name;
    QString major;
    int faceRect[4];
} Student;



//class FaceDeteThread :  public QObject, public QRunnable
class FaceDeteThread :  public QThread
{
    Q_OBJECT
public:
    FaceDeteThread();

private:
    FaceDete* facedete;
    QMutex lock;
    QImage img;
    cv::Mat mat;
    int faceRect[4];
    bool canRun;
    Json::Value detectedResult;
    QList<Student> result;

protected:
    void run();

signals:
    void DetectFinished(Student res);

public slots:
    void ReceiveImg(const QImage& _img);
    void StopImmediately();
    void CanRun();
};

#endif // FACEDETETHREAD_H
