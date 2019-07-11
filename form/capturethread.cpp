#include "capturethread.h"

CaptureThread::CaptureThread() : QThread ()
{

}

/*
 * 周期性向UI线程发送信号，以控制截取视频流、人脸跟踪、人脸检测的时机
 * Sleep 的时间需要仔细设置，否则可能会使UI线程不能及时得到人脸识别的结果反馈
 */
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
        QThread::msleep(85);

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
