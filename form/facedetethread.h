#ifndef FACEDETETHREAD_H
#define FACEDETETHREAD_H

#include <QMutex>
#include <QImage>
#include <QVector>
#include <QRect>

#include "FaceDete.h"


typedef struct t_student {
    bool identifiable;
    QString id;
    QString name;
    QString major;
    QRect faceRect;
    QString path;
} Student;



//class FaceDeteThread :  public QObject, public QRunnable
class FaceDeteThread :  public QThread
{
    Q_OBJECT
public:
    FaceDeteThread(const QString& photoPath);

private:
    FaceDete* facedete;
    QImage img;
    cv::Mat mat;
    bool detect;
    Json::Value detectedResult;
    QVector<QRect> resultOnlyTrack;
    QVector<Student> resultComplete;

protected:
    void run();

signals:
    void DetectFinished(QVector<Student> res);
    void DetectFinishedWihoutResult();

    void TrackFinished(QVector<QRect> res);
    void TrackFinishedWithoutResult();

public slots:
    void ReceiveImg(bool _detect, const QImage& image);
};

#endif // FACEDETETHREAD_H
