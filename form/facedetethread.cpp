#include <QMutexLocker>

#include "facedetethread.h"


FaceDeteThread::FaceDeteThread() : shouldRun(true)
{
    facedete = new FaceDete();
}

void FaceDeteThread::StopImmediately()
{
    QMutexLocker locker(&lock);
    shouldRun = false;
}

void FaceDeteThread::run()
{
//    while (1) {

        {
            QMutexLocker locker(&lock);
            if (!shouldRun) {
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

        qRegisterMetaType<Student>("Student");

        if (result.empty()) {
            emit DetectFinished({"null", "", "", {0, 0, 0, 0}});
            return;
        }


        // TODO: 人脸识别结果大于1的情况
        if (result.size() > 1) {

        }

        emit DetectFinished(result.front());

//        {
//            QMutexLocker locker(&lock);
//            if (!shouldRun) {
//                return ;
//            }
//        }


//    }


#ifdef DEBUG
        qDebug() << "successfully generaged detection result";
#endif
}

void FaceDeteThread::ReceiveImg(QImage _img)
{
#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | received image";
#endif

    switch (_img.format()) {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | convert image from ARGB32 to BGR";
#endif
        mat = cv::Mat(_img.height(), _img.width(), CV_8UC4, (void*)_img.constBits(), _img.bytesPerLine());
        cv::cvtColor(mat,mat,COLOR_RGBA2BGR);
        break;
    case QImage::Format_RGB888:
#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | convert image from RGB888 to BGR888";
#endif
        mat = cv::Mat(_img.height(), _img.width(), CV_8UC3, (void*)_img.constBits(), _img.bytesPerLine());
        cv::cvtColor(mat, mat, CV_RGB2BGR);
        break;
    default:
        qDebug() << "FaceDeteThread | " << _img.format();
        break;
    }

#ifdef DEBUG_FACE
    qDebug() << "FaceDeteThread | convert image successfully";
    qDebug() << "FaceDeteThread | " << cv::typeToString(mat.type()).c_str();
#endif

}
