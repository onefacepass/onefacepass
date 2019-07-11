#include <QMutexLocker>

#include "facedetethread.h"


FaceDeteThread::FaceDeteThread(const QString& photoPath)
{
    facedete = new FaceDete();

    facedete->SetPreloadPath(photoPath.toStdString());

    facedete->SetConfLevel(static_cast<MFloat>(0.8));

    if (facedete->Loadregface() < 0) {
        // 没做完整的处理：过早的优化是罪恶之源
        qDebug() << "\033[31m" << "FaceDeteThread | facedete->Loadregface() < 0" << "\033[0m";
    }


    qRegisterMetaType<QVector<QRect> >("QVector<QRect>");
    qRegisterMetaType<QVector<Student> >("QVector<Student>");
}

FaceDeteThread::~FaceDeteThread()
{
    tasks.clear();
}

void FaceDeteThread::run()
{


    while(!isInterruptionRequested()) {

        while (!tasks.empty()) {

#ifdef DEBUG_FACE
            qDebug() << "FaceDeteThread | detecting...";
#endif
            { // lock begin
                QMutexLocker locker(&lock);
                t = tasks.dequeue();    // t: <图片 QImage, 是否进行检测 bool>
                if (tasks.size() > 5) {
                    continue;
                }
#ifdef DEBUG_FACE
                qDebug() << "取到任务，"
                         << "剩余：" << tasks.size();
#endif
            } // lock end

            cv::Mat mat_tmp = cv::Mat(t.first.height(), t.first.width(), CV_8UC4, const_cast<uchar*>(t.first.bits()), t.first.bytesPerLine());
            cv::Mat mat = cv::Mat(mat_tmp.rows, mat_tmp.cols, CV_8UC3 );
            int from_to[] = { 0,0, 1,1, 2,2 };
            cv::mixChannels( &mat_tmp, 1, &mat, 1, from_to, 3 );


            if (facedete->DetectFaces(mat, detectedResult)) {
                qDebug() << "FaceDeteThread | something is wrong while detecting";
            }

#ifdef DEBUG_FACE
        qDebug() << "FaceDeteThread | detection finished";
#endif

            if (detectedResult.empty()) {
#ifdef DEBUG_FACE
        qDebug() << "\033[31m" << "FaceDeteThread | detectedResult empty! "
                 << detectedResult.size() << "\033[0m";
#endif
                if (t.second)
                    emit DetectFinishedWihoutResult();
                else
                    emit TrackFinishedWithoutResult();

                continue;
            }

            for (unsigned int i = 0; i < detectedResult.size(); i ++) {
                Json::Value currRes = detectedResult[std::to_string(i)];

                if (t.second) {   //--- 人脸识别
                    QString id(currRes["id"].asString().data());
                    QString name(currRes["name"].asString().data());
                    QString major(currRes["major"].asString().data());
                    bool identifiable = currRes["identifiable"].asBool();
                    QString path(currRes["pathInPreload"].asString().data());

                    // 检测模式下返回完整的人脸识别信息
                    resultComplete.push_back({identifiable,
                                              id,
                                              name,
                                              major,
                                              QRect(currRes["rect"][0].asInt(), currRes["rect"][1].asInt(),
                                                    currRes["rect"][2].asInt()-currRes["rect"][0].asInt(), currRes["rect"][3].asInt()-currRes["rect"][1].asInt()),
                                              path});
                } else {    //--- 仅人脸跟踪
                    // 跟踪模式下只返回人脸位置
                    resultOnlyTrack.push_back(QRect(currRes["rect"][0].asInt(), currRes["rect"][1].asInt(),
                            currRes["rect"][2].asInt()-currRes["rect"][0].asInt(), currRes["rect"][3].asInt()-currRes["rect"][1].asInt()));
                }
            }

            if (t.second)
                emit DetectFinished(resultComplete);
            else
                emit TrackFinished(resultOnlyTrack);


            detectedResult.clear();
            resultComplete.clear();
            resultOnlyTrack.clear();

        }

        // 等待tasks非空
        // TODO: 有点暴力了
        while (tasks.empty()) {
            qDebug() << "等待10ms";
            QThread::msleep(10);
        }

    }
}

void FaceDeteThread::ReceiveImg(bool _detect, const QImage& image)
{
    QMutexLocker locker(&lock);

    tasks.enqueue({image, _detect});
}
