#ifndef FACEDETETHREAD_H
#define FACEDETETHREAD_H

#include <QMutex>
#include <QImage>
#include <QList>

#include "FaceDete.h"

typedef struct t_student {
    QString id;
    QString name;
    QString major;
    int faceRect[4];
} Student;

class FaceDeteThread : public QThread
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
    bool shouldRun;
    Json::Value detectedResult;
    QList<Student> result;

protected:
    void run();

signals:
    void DetectFinished(Student res);

public slots:
    void ReceiveImg(QImage _img);
    void StopImmediately();
};

#endif // FACEDETETHREAD_H
