#include "capturethread.h"

CaptureThread::CaptureThread() : QThread ()
{

}

void CaptureThread::run()
{
    int i = 0;
    while (true) {
        // 注意：截图的信号要在Track和Detect两个信号之前发出！
        emit CaptureNotice();
#ifdef DEBUG_THREAD
//        qDebug() << "\033[32m" << "emit CaptureNotice" << "\033[0m";
#endif

        i = i % 10 + 1;
        QThread::msleep(200);

        if (i % 10 != 0) {
            emit TrackFaceNotice();
#ifdef DEBUG_THREAD
//            qDebug() << "\033[32m" << "emit TrackFaceNotice" << "\033[0m";
#endif
        } else {
            emit DetectFaceNotice();
#ifdef DEBUG_THREAD
//            qDebug() << "\033[32m" << "emit DetectFaceNotice" << "\033[0m";
#endif
        }
    }
}
