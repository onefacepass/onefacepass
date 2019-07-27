#ifndef FACEDETETHREAD_H
#define FACEDETETHREAD_H

#include <QMutex>
//#include <QWaitCondition>
#include <QImage>
#include <QVector>
#include <QRect>
#include <QQueue>

#include "FaceDete.h"


typedef struct t_student {
    bool identifiable;
    QString id;
    QString name;
    QString major;
    QRect faceRect;
    QString path;
} Student;



//class FaceThread :  public QObject, public QRunnable
class FaceThread :  public QThread
{
    Q_OBJECT
public:

    ~FaceThread();

    static FaceThread* Instance();

    // 一定要先设置好Preload再使用
    void SetPreloadPath(const QString &path);

private:
    FaceThread();

    static FaceThread *self;

    FaceDete* facedete;
    QPair<QImage, bool> t;

    Json::Value detectedResult;
    QVector<QRect> resultOnlyTrack;
    QVector<Student> resultComplete;

    QQueue<QPair<QImage, bool>> tasks;

    QMutex lock;
//    QWaitCondition cond;

protected:
    void run();

signals:
    void DetectFinished(QVector<Student> res);
    void DetectFinishedWithoutResult();

    void TrackFinished(QVector<QRect> res);
    void TrackFinishedWithoutResult();

public slots:
    void ReceiveImg(bool _detect, const QImage& image);
};

#endif // FACEDETETHREAD_H
