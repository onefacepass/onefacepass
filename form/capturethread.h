#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H


class CaptureThread : public QThread
{
    Q_OBJECT
public:
    CaptureThread();

protected:
    void run();

signals:
    void CaptureNotice();
};

#endif // CAPTURETHREAD_H
