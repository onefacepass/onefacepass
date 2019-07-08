#ifndef FACETRACKTHREAD_H
#define FACETRACKTHREAD_H


class FaceTrackThread : public QThread
{
    Q_OBJECT
public:
    FaceTrackThread();

protected:
    void run();

signals:
    void TrackFinished();

public slots:
    void ReceiveImg(const QImage& _img);
};

#endif // FACETRACKTHREAD_H
