#include "posethread.h"

PoseThread::PoseThread()
{

}

/*
 *
 */
void PoseThread::run()
{
    while (!isInterruptionRequested()) {

        while (!tasks.empty()) {
            {
                QMutexLocker locker(&lock);
                t = tasks.dequeue();
                // t: <QImage, bool> 其中bool标记当前图片用来跟踪位置，还是用来识别
            }
        }

        // ---------- TODO ----------------

    }
}


void PoseThread::ReceiveImg(bool _detect, const QImage& image)
{
    QMutexLocker locker(&lock);

    tasks.enqueue({image, _detect});
}
