﻿#include <QMutexLocker>

#include "facedetethread.h"


FaceDeteThread::FaceDeteThread() : canRun(true), detect(false)
{
    facedete = new FaceDete();

    facedete->SetPreloadPath("C:\\Workspace\\onefacepass\\sample");

    if (facedete->Loadregface() == 0) {
        qDebug() << "\033[31m" << "FaceDeteThread | facedete->Loadregface() == 0" << "\033[0m";
    }
    qRegisterMetaType<Student>("Student");
    qRegisterMetaType<QVector<QRect> >("QVector<QRect>");
    qRegisterMetaType<QVector<Student> >("QVector<Student>");
}

void FaceDeteThread::StopImmediately()
{
    QMutexLocker locker(&lock);
    canRun = false;
}

void FaceDeteThread::run()
{
//    while (1) {

        {
            QMutexLocker locker(&lock);
            if (!canRun) {
                return ;
            }
        }

#ifdef DEBUG_FACE
//    cv::imwrite("C:\\Users\\haoha\\Pictures\\mat.png", mat);
    qDebug() << "FaceDeteThread | detecting...";
#endif
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
            if (detect)
                emit DetectFinishedWihoutResult();
            else
                emit TrackFinishedWithoutResult();

            return;
        }

        for (unsigned int i = 0; i < detectedResult.size(); i ++) {
            Json::Value currRes = detectedResult[std::to_string(i)];

            if (detect) {
                QString id(currRes["id"].asCString());
                QString name(currRes["name"].asCString());
                QString major(currRes["major"].asCString());
                bool identifiable = currRes["identifiable"].asBool();
                QString path(currRes["pathInPreload"].asCString());

                resultComplete.push_back({identifiable,
                                          id,
                                          name,
                                          major,
                                          QRect(currRes["rect"][0].asInt(), currRes["rect"][1].asInt(),
                                                currRes["rect"][2].asInt()-currRes["rect"][0].asInt(), currRes["rect"][3].asInt()-currRes["rect"][1].asInt()),
                                          path});
            } else {
                resultOnlyTrack.push_back(QRect(currRes["rect"][0].asInt(), currRes["rect"][1].asInt(),
                        currRes["rect"][2].asInt()-currRes["rect"][0].asInt(), currRes["rect"][3].asInt()-currRes["rect"][1].asInt()));
            }
        }

        if (detect)
            emit DetectFinished(resultComplete);
        else
            emit TrackFinished(resultOnlyTrack);


        detectedResult.clear();
        resultComplete.clear();
        resultOnlyTrack.clear();
//    }
}

void FaceDeteThread::CanRun()
{
    QMutexLocker locker(&lock);
    canRun = true;
}

void FaceDeteThread::ReceiveImg(bool _detect, const QImage& image)
{
#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | received image, " << image.format() <<", detect: " << _detect;
#endif

    detect = _detect;

    cv::Mat mat_tmp = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    mat = cv::Mat(mat_tmp.rows, mat_tmp.cols, CV_8UC3 );
    int from_to[] = { 0,0, 1,1, 2,2 };
    cv::mixChannels( &mat_tmp, 1, &mat, 1, from_to, 3 );


#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | convert image successfully, "
             << cv::typeToString(mat.type()).c_str();
    //    cv::imshow("debug", mat);
    //    cv::imwrite("C:\\Users\\haoha\\Pictures\\mat.png", mat);
#endif
}
