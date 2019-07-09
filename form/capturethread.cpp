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

        i = i % 10 + 1;
        QThread::msleep(100);

        if (i % 10 != 0) {
            emit TrackFaceNotice();
        } else {
            emit DetectFaceNotice();
        }
    }
}
