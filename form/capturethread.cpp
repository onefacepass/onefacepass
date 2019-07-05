#include "capturethread.h"

CaptureThread::CaptureThread() : QThread ()
{

}

//void CaptureThread::CaptureNotice()
//{

//}

void CaptureThread::run()
{
    while (true) {
        QThread::msleep(1000);
        emit CaptureNotice();
    }
}
