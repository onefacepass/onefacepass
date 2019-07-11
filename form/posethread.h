#ifndef POSEHREAD_H
#define POSEHREAD_H

#include <QThread>
#include <QMutex>

class PoseThread : public QThread
{
    Q_OBJECT
public:
    PoseThread();

protected:
    void run();

private:
    QMutex lock;
    QPair<QImage, bool> t;
    QQueue<QPair<QImage, bool> > tasks;


public slots:
    void ReceiveImg(bool _detect, const QImage& image);

signals:
    // TODO: 需要补充传递回主线程的数据类型
    void TrackFinished();
    void DetectFinished();

    void TrackFinishedWithoutResult();
    void DetectFinishedWithoutResult();
};

#endif // POSEHREAD_H
