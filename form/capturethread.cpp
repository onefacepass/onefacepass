#include "capturethread.h"

CaptureThread::CaptureThread()
{

}

//void CaptureThread::CaptureNotice()
//{

//}

void CaptureThread::run()
{
    while (true) {
        emit CaptureNotice();
        QThread::msleep(1000);
    }
}
