#include "quicreator.h"
#include "ui_quicreator.h"
#include "stuwidget.h"
#include "quiwidget.h"

Q_DECLARE_METATYPE(QCameraInfo)

QUICreator::QUICreator(QWidget *parent, const QString& config_file) :
    QMainWindow(parent),
    ui(new Ui::QUICreator)
{

    ui->setupUi(this);

    config = new QSettings(config_file, QSettings::IniFormat);


    checkConfig();

    initForm();

    initFaceAndPose();
}

QUICreator::~QUICreator()
{
    delete ui;

    if (faceThread) {
        faceThread->requestInterruption();
    }

    if (poseThread) {
        poseThread->requestInterruption();
    }
}

void QUICreator::initForm()
{
    initAction();
    initNav();

    initCamera();

    initOther();

    initStyle();

    initStudentWidget();

    ui->tabWidget->setCurrentIndex(0);
}

void QUICreator::initAction()
{
    QAction *actionAbout = new QAction(this);
    actionAbout->setText(tr("关于"));
    actionAbout->setToolTip(tr("关于一脸通"));
    connect(actionAbout, &QAction::triggered, this, &QUICreator::about);


    ui->menuHelp->addAction(actionAbout);
}

/*
 * @func: 初始化负责人脸识别的进程
 */
void QUICreator::initFaceAndPose()
{
#ifdef DEBUG
    qDebug() << "照片目录：" << config->value("FaceDetect/preload").toString();
    qDebug() << "测试用的证件照：" << config->value("Debug/photo").toString();
#endif
    faceThread = new FaceThread(config->value("FaceDetect/preload").toString());
    connect(faceThread, &FaceThread::DetectFinished, this, &QUICreator::faceDetectFinished);
    connect(faceThread, &FaceThread::TrackFinished, this, &QUICreator::faceTrackFinished);
    connect(faceThread, &FaceThread::DetectFinishedWithoutResult, this, &QUICreator::faceDetectFinishedWithoutResult);
    connect(faceThread, &FaceThread::TrackFinishedWithoutResult, this, &QUICreator::faceTrackFinishedWithoutResult);

    poseThread = new PoseThread();
    connect(poseThread, &PoseThread::DetectFinished, this, &QUICreator::poseDetectFinished);
    connect(poseThread, &PoseThread::TrackFinished, this, &QUICreator::poseTrackFinished);
    connect(poseThread, &PoseThread::DetectFinishedWithoutResult, this, &QUICreator::poseDetectFinishedWithoutResult);
    connect(poseThread, &PoseThread::TrackFinishedWithoutResult, this, &QUICreator::poseTrackFinishedWithoutResult);

    faceThread->start();
    poseThread->start();
}


/*
 * @func: 初始化竖直导航栏
 */
void QUICreator::initNav()
{
    //设置左边按钮菜单
    QList<QPushButton *> btns = ui->widgetVerticalMenu->findChildren<QPushButton *>();

    foreach (QPushButton *btn, btns) {
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this, &QUICreator::navBtnClicked);
    }
    ui->btnMenuDebug->click();
}

void QUICreator::initOther()
{
    // 启用人脸跟踪，当前该设置项不生效
    ui->checkboxFace->setChecked(true);
    // 显示姿势轮廓，当前该设置项不生效
    ui->checkboxPose->setChecked(false);
    // 向UI中更新日志，当前该设置项不生效
    ui->checkboxLog->setChecked(false);

    ui->widgetVerticalMenu->setProperty("nav", "left");
    ui->widgetPay->setProperty("nav", "top");

    ui->centralwidget->findChild<QWidget *>("widgetVideo")->setProperty("video", true);

//    ui->btnPay->setEnabled(false);

    connect(ui->btnReadyPay, &QPushButton::clicked, this, &QUICreator::btnReadyPayClicked);
    connect(ui->btnResetPay, &QPushButton::clicked, this, &QUICreator::resetConsumption);

    connect(ui->btnPay, &QPushButton::clicked, this, &QUICreator::btnPayClicked);
}

void QUICreator::about()
{

//    QUIWidget::showMessageBoxInfo(tr("关于一脸通"));
//    QMessageBox::about(this, tr("关于一脸通"), tr("关于一脸通"));
}

/*
 * @func: 初始化摄像头 & 初始化负载对视频流截图的进程
 */
void QUICreator::initCamera()
{
    captureThread = new CaptureThread();
//    connect(captureThread, &QThread::finished, captureThread, &QObject::deleteLater);

// 加载“设备”菜单：后面现场调试时，可能电脑上会有多个摄像头设备吧！
// TODO: 当前多摄像设备的支持是有Bug的，后续再考虑修复Bug或者删除该支持
//    QActionGroup *videoDevicesGroup = new QActionGroup(this);
//    videoDevicesGroup->setExclusive(true);
//    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
//    for (const QCameraInfo &cameraInfo : availableCameras) {
//        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
//        videoDeviceAction->setCheckable(true);
//        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
//        if (cameraInfo == QCameraInfo::defaultCamera())
//            videoDeviceAction->setChecked(true);

//        ui->menuDevice->addAction(videoDeviceAction);
//    }

//    connect(videoDevicesGroup, &QActionGroup::triggered, this, &QUICreator::updateCamera);

    // 使用默认摄像头
    setCamera(QCameraInfo::defaultCamera());


    connect(captureThread, &CaptureThread::CaptureNotice, this, &QUICreator::takeImage);

    connect(captureThread, &CaptureThread::DetectFaceNotice, this, &QUICreator::doDetect);
    connect(captureThread, &CaptureThread::TrackFaceNotice, this, &QUICreator::doTrack);

    captureThread->start();
}

void QUICreator::initStudentWidget()
{
    // 预先加载用来展示结果的图形控件
    for (int i = 0; i < MAX_RESULT; ++i) {
        stuWidgets.push_back(shared_ptr<StuWidget>(new StuWidget(this)));
    }
}

/*
 * @func: 对视频流进行截图
 *        这里会触发QCameraImageCapture::imageCaptured信号
 */
void QUICreator::takeImage()
{
    imageCapture->capture();
}

/*
 * @func: 切换摄像头
 */
void QUICreator::updateCamera(QAction *action)
{
    QUICreator::setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

/*
 * @func: 设置当前摄像头
 */
void QUICreator::setCamera(const QCameraInfo &cameraInfo)
{
    camera.reset(new QCamera(cameraInfo));

    imageCapture.reset(new QCameraImageCapture(camera.data()));

    connect(camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &QUICreator::displayCameraError);
    connect(imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &QUICreator::processCapturedImage);


    // 仅将截图保存到缓冲区
    if (imageCapture->isCaptureDestinationSupported(QCameraImageCapture::CaptureToBuffer) ) {
        imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    } else {
        QUIWidget::showMessageBoxInfo("暂不支持将截图保存至缓冲区！");
    }

    camera->setCaptureMode(QCamera::CaptureStillImage);

    camera->setViewfinder(ui->viewfinder);

    camera->start();

    // 设置摄像头分辨率
    QCameraViewfinderSettings set;
    set.setResolution(640, 480);
    camera->setViewfinderSettings(set);
}

/*
 * @func: 通知线程进行人脸识别和姿态识别
 */
void QUICreator::doDetect()
{
    if (this->img_tmp.isNull()) {
        return;
    }

    faceThread->ReceiveImg(true, this->img_tmp);
    // poseThread->ReceiveImg(true, this->img_tmp); // TODO
}

/*
 * @func: 通知线程进行人脸跟踪和姿态跟踪
 */
void QUICreator::doTrack()
{
    if (this->img_tmp.isNull()) {
        return;
    }

    faceThread->ReceiveImg(false, this->img_tmp);
    // poseThread->ReceiveImg(false, this->img_tmp);    // TODO
}


/*
 * @func: 处理人脸追踪结果：通知viewfinder画人脸框
 * @res: 人脸的范围坐标，可能是多个人脸
 */
void QUICreator::faceTrackFinished(QVector<QRect> res)
{
    ui->viewfinder->ReceiveRects(res);
    ui->viewfinder->update();
}

/*
 * @func: 处理人脸识别结果
 * @res: 由 FaceDetectThread::faceDetectFinished 发送过来的一组 Student
 */
void QUICreator::faceDetectFinished(QVector<Student> res)
{
    // 清除UI上已经显示的识别结果，TODO: 这里处理得不是很好
    for (int i = 0; i < MAX_RESULT; ++i) {
        stuWidgets[i]->setParent(nullptr);
    }

    // 向UI加载识别结果
    for (int i = 0; i < res.size(); i ++) {
        if (!res[i].identifiable)
            continue;
        if (i >= MAX_RESULT)
            break;

        stuWidgets[i]->setParent(this);
        stuWidgets[i]->setID(res[i].id);
        stuWidgets[i]->setName(res[i].name);
        stuWidgets[i]->setMajor(res[i].major);
//        stuWidgets[i]->setPhoto(config->value("Debug/photo").toString());    // todo: 当前使用唯一的测试图片
        stuWidgets[i]->setPhoto(res[i].path);
        ui->resultHorizontalLayout->addWidget(stuWidgets[i].get());
    }


    for(auto r : res) {
        qDebug() << r.identifiable << r.id << "\t" << r.name << "\t" << r.major;
    }
}

void QUICreator::faceTrackFinishedWithoutResult()
{
}

void QUICreator::faceDetectFinishedWithoutResult()
{
}


/*
 * @func: 处理姿态跟踪结果
 * TODO:
 */
void QUICreator::poseTrackFinished()
{

}

/*
 * @func: 处理姿态识别结果
 * TODO
 */
void QUICreator::poseDetectFinished()
{

}

/*
 * @func:
 * TODO
 */
void QUICreator::poseTrackFinishedWithoutResult()
{

}

/*
 * @func:
 * TODO
 */
void QUICreator::poseDetectFinishedWithoutResult()
{

}


/*
 * @func:       处理 takeImage 发送的信号
 * @requestId:  这是一个递增的序列，是QCameraCapture中实现的，当前没用
 * @_img:       截取的一帧视频流
 */
void QUICreator::processCapturedImage(int requestId, const QImage& _img)
{
    Q_UNUSED(requestId)

    this->img_tmp = _img;
}




void QUICreator::displayCameraError()
{
    QUIWidget::showMessageBoxError("Camera Error: " + camera->errorString());
}


/*
 * @func: 竖直导航栏的菜单切换
 */
void QUICreator::navBtnClicked()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    QString objName = btn->objectName();

    QList<QPushButton *> btns = ui->widgetVerticalMenu->findChildren<QPushButton *>();

    foreach (QPushButton *b, btns) {
        if (b == btn) {
            b->setChecked(true);
        } else {
            b->setChecked(false);
        }
    }

    if (objName == "btnMenuDebug") {
        ui->stackedWidgetDebug->setCurrentIndex(0);
    } else if (objName == "btnMenuWork") {
        ui->stackedWidgetDebug->setCurrentIndex(1);
    }

}


void QUICreator::initStyle()
{
    QString fileName = ":/qss/darkblue.css";

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            setStyle(str);
        }
    }
}



void QUICreator::setStyle(const QString &str)
{
    static QString qss;

    if (qss == str) {
        return;
    }

    qss = str;
    QString paletteColor = str.mid(20, 7);
    qApp->setPalette(QPalette(QColor(paletteColor)));
    qApp->setStyleSheet(str);
}

/*
 * @func: 向调试界面插入一条日志
 * @str: 待插入的日志，无需关注结尾是否有'\n'
 */
void QUICreator::insertLog(const QString& str)
{
    static QString log;
    if (log == str) {
        return;
    }

    QString time = "["
            + QString("%1").arg(qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")))
            + "] ";

    log = time + str;

    if (!log.endsWith("\n")) {
        log += "\n";
    }

    ui->plaintextLog->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    ui->plaintextLog->insertPlainText(log);
}

// 清零消费金额
void QUICreator::resetConsumption()
{
    ui->spinboxPay->setValue(0);
//    ui->btnPay->setEnabled(false);
}

void QUICreator::btnReadyPayClicked()
{
//    ui->btnPay->setEnabled(true);
//    ui->btnPay->setFocus();
}


/*
 * @func: 触发支付按钮后的动作放这里
 */
void QUICreator::btnPayClicked()
{
    // 是不是要在这里验证身份
    // takeImage();
}

void QUICreator::checkConfig()
{
    if (config->allKeys().size() == 0) {
        QUIWidget::ShowMessageBoxErrorAndExit("配置文件初始化失败！");
    } else if (!config->contains("FaceDetect/preload")) {
        QUIWidget::ShowMessageBoxErrorAndExit("配置参数preload错误！");
    }
#ifdef DEBUG_CONFIG
    qDebug() << "[CONFIG]";
    foreach (auto key, config->allKeys()) {
        qDebug() << key << "=" << config->value(key).toString();
    }
    qDebug() << "\n";
#endif
}


/*********************************************************
 *                      调试专用                          *
 *********************************************************/

void QUICreator::debug_show_supported_viewfinder_resolutions()
{
    QList<QSize> size = camera->supportedViewfinderResolutions();
    qDebug() << "supportedViewfinderResolutions" << size.length();
    for (auto s : size) {
        qDebug() << s.width() << " " << s.height();
    }
}

void QUICreator::debug_show_student_info(Student s)
{
    qDebug()<< "\033[32m" << s.identifiable << s.id << "\t" << s.name << "\t" << s.major << s.path;
}
