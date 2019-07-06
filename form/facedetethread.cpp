#include <QMutexLocker>

#include "facedetethread.h"


FaceDeteThread::FaceDeteThread() : canRun(true)
{
    facedete = new FaceDete();
    qRegisterMetaType<Student>("Student");
}

void FaceDeteThread::StopImmediately()
{
    QMutexLocker locker(&lock);
    canRun = false;
}

void FaceDeteThread::run()
{


    while (1) {

        {
            QMutexLocker locker(&lock);
            if (!canRun) {
                return ;
            }
        }

#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | detecting...";
#endif

        if (facedete->DetectFaces(mat, detectedResult)) {
            qDebug() << "something is wrong while detecting";
        }

#ifdef DEBUG_FACE
        qDebug() << "FaceDeteThread | detection finished";
#endif

        if (detectedResult.empty()) {
            emit DetectFinished({"null", "", "", {0, 0, 0, 0}});
            return;
        }


        for (unsigned int i = 0; i < detectedResult.size(); i ++) {
            Json::Value currRes = detectedResult[std::to_string(i)];

            QString id(currRes["id"].asCString());
            QString name(currRes["name"].asCString());
            QString major(currRes["major"].asCString());

            result.push_back({id, name, major, {currRes["rect"][0].asInt(),
                                                currRes["rect"][1].asInt(),
                                                currRes["rect"][2].asInt(),
                                                currRes["rect"][3].asInt()}});
        }
        detectedResult.clear();

        // TODO: 人脸识别结果大于1的情况
        if (result.size() > 1) {

        }

        emit DetectFinished(result.front());
    }
}

void FaceDeteThread::CanRun()
{
    QMutexLocker locker(&lock);
    canRun = true;
}

void FaceDeteThread::ReceiveImg(const QImage& image)
{
#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | received image, " << image.format();
#endif

    cv::Mat mat_tmp = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    mat = cv::Mat(mat_tmp.rows, mat_tmp.cols, CV_8UC3 );
    int from_to[] = { 0,0, 1,1, 2,2 };
    cv::mixChannels( &mat_tmp, 1, &mat, 1, from_to, 3 );


#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | convert image successfully";
    qDebug() << "FaceDeteThread | " << cv::typeToString(mat.type()).c_str();
#endif
//    cv::imshow("debug", mat);
    cv::imwrite("C:\\Users\\haoha\\Pictures\\mat.png", mat);
}
