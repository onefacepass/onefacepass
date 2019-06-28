#include "quicreator.h"
#include "ui_quicreator.h"

#include "quiwidget.h"

Q_DECLARE_METATYPE(QCameraInfo)

QUICreator::QUICreator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QUICreator)
{

    ui->setupUi(this);
    this->initForm();
    this->initFace();
}

QUICreator::~QUICreator()
{
    delete ui;
    if (facedete) {
        facedete->UninitEngine();

        delete facedete;
        facedete = nullptr;
    }
}

void QUICreator::initForm()
{
    initHistoryWidget();
    initAction();
    initNav();

    initCamera();

    initOther();

    initStyle();

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

void QUICreator::initFace()
{
    qDebug() << "init face started";
    facedete = new FaceDete();

    facedete->SetAPPID("a4e18xLPPvPkB76rXtYM5GVraNduE3Q7vUnGPFLfhSj");
    facedete->SetSDKKey("Fbu8Y5KNdMGpph8MrJc4GWceasdTeoGuCx3Qd4oRP6vs");
    facedete->GetVersion();
    facedete->Activation();
    facedete->InitEngine();

    facedete->SetPreloadPath("C:\\Workspace\\onefacepass\\face_recognition\\face_module\\sample");

    facedete->Loadregface();

    if (facedete->Loadregface() == -1) {
        QUIWidget::showMessageBoxError("Error path!");
    }

}

void QUICreator::about()
{

//    QUIWidget::showMessageBoxInfo(tr("关于一脸通"));
//    QMessageBox::about(this, tr("关于一脸通"), tr("关于一脸通"));
}

// 加载摄像头
void QUICreator::initCamera()
{
    captureThread = new CaptureThread();

    // 加载“设备”菜单：后面现场调试时，可能电脑上会有多个摄像头设备吧！
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);

        ui->menuDevice->addAction(videoDeviceAction);
    }

    connect(videoDevicesGroup, &QActionGroup::triggered, this, &QUICreator::updateCamera);
    connect(ui->checkboxCamera, &QCheckBox::stateChanged, this, &QUICreator::startAndStopCamera);

    setCamera(QCameraInfo::defaultCamera());
    ui->checkboxCamera->setChecked(true);

    connect(captureThread, &CaptureThread::CaptureNotice, this, &QUICreator::takeImage);
    captureThread->start();
}

void QUICreator::takeImage()
{
    imageCapture->capture();
}

void QUICreator::startAndStopCamera()
{
    if (ui->checkboxCamera->isChecked()) {
        camera->start();
        ui->viewfinder->startCamera();
    } else {
        camera->stop();
        ui->viewfinder->stopCamera();
    }
}

// 切换摄像头
void QUICreator::updateCamera(QAction *action)
{
    QUICreator::setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

// 设置当前使用的摄像头
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
}

void QUICreator::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);

    Json::Value detectedResult;

    cv::Mat mat = QImage2Mat(img);

    facedete->DetectFaces(mat, detectedResult);

    if (detectedResult.size()) {
        qDebug() << "有识别结果";
    }

    for (unsigned int i = 0; i < detectedResult.size(); i ++) {
        Json::Value currRes = detectedResult[std::to_string(i)];
        QString id(currRes["id"].asCString());
        QString name(currRes["name"].asCString());
        QString major(currRes["major"].asCString());

        // 获取面部图像的范围坐标
        for (int j = 0; j < 4; j ++) {
            faceRect[j] = currRes["rect"].asInt();
        }

        qDebug() << "[ID]" << id;
        qDebug() << "[Name]" << name;
        qDebug() << "[Major]" << major;
        qDebug() << "\n";

    }

    detectedResult.clear();
}

cv::Mat QUICreator::QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(), src.width(), CV_8UC3, (uchar*)src.bits(), src.bytesPerLine());
    cv::Mat res;

    cvtColor(tmp, res, COLOR_BGR2RGB);

    return res;
};

void QUICreator::displayCameraError()
{
    QUIWidget::showMessageBoxError("Camera Error: " + camera->errorString());
}

// 加载历史记录
void QUICreator::initHistoryWidget()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->historyTableWidget->setColumnCount(5);
    ui->historyTableWidget->setColumnWidth(0, static_cast<int>(width * 0.06));
    ui->historyTableWidget->setColumnWidth(1, static_cast<int>(width * 0.04));
    ui->historyTableWidget->setColumnWidth(2, static_cast<int>(width * 0.08));
    ui->historyTableWidget->setColumnWidth(3, static_cast<int>(width * 0.08));
    ui->historyTableWidget->setColumnWidth(4, static_cast<int>(width * 0.08));
    ui->historyTableWidget->verticalHeader()->setDefaultSectionSize(25);

    QStringList headText;
    headText << "学号" << "姓名" << "学院" << "专业" << "上次见面";
    ui->historyTableWidget->setHorizontalHeaderLabels(headText);
    ui->historyTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->historyTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->historyTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->historyTableWidget->setAlternatingRowColors(true);
    ui->historyTableWidget->verticalHeader()->setVisible(false);
    ui->historyTableWidget->horizontalHeader()->setStretchLastSection(true);

    //设置行高
    ui->historyTableWidget->setRowCount(10);

    for (int i = 0; i < 10; i++) {
        ui->historyTableWidget->setRowHeight(i, 24);

        QTableWidgetItem *itemDeviceID = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem *itemDeviceName = new QTableWidgetItem(QString("名字%1").arg(i + 1));
        QTableWidgetItem *itemDeviceAddr = new QTableWidgetItem(QString("学院%1").arg(i + 1));
        QTableWidgetItem *itemContent = new QTableWidgetItem(QString("专业%1").arg(i + 1));
        QTableWidgetItem *itemTime = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        ui->historyTableWidget->setItem(i, 0, itemDeviceID);
        ui->historyTableWidget->setItem(i, 1, itemDeviceName);
        ui->historyTableWidget->setItem(i, 2, itemDeviceAddr);
        ui->historyTableWidget->setItem(i, 3, itemContent);
        ui->historyTableWidget->setItem(i, 4, itemTime);
    }
}

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
    ui->checkboxFace->setChecked(false);
    ui->checkboxPose->setChecked(false);
    ui->checkboxLog->setChecked(false);


    ui->widgetVerticalMenu->setProperty("nav", "left");
//    ui->widgetBottom->setProperty("form", "bottom");
    ui->widgetTop->setProperty("nav", "top");

    ui->centralwidget->findChild<QWidget *>("widgetVideo")->setProperty("video", true);

    ui->btnPay->setEnabled(false);

    connect(ui->btnReadyPay, &QPushButton::clicked, this, &QUICreator::btnReadyPayClicked);
    connect(ui->btnResetPay, &QPushButton::clicked, this, &QUICreator::resetConsumption);

    debugFunc();
}

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

/* @func: 在摄像头图像上显示日志，可能会有不到1s的延迟
 * @log: 要显示的日志
 */
void QUICreator::displayLogOnCamera(const QString &log)
{
    ui->viewfinder->insertLog(log);
}

void QUICreator::debugFunc()
{
    insertLog("log1");
    insertLog("log2");
    insertLog("log3");
}

/* @func: 向调试界面插入一条日志
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
    ui->btnPay->setEnabled(false);
}

void QUICreator::btnReadyPayClicked()
{
    ui->btnPay->setEnabled(true);
    ui->btnPay->setFocus();
}


/* @func: 触发支付按钮后的动作放这里
 */
void QUICreator::btnPayClicked()
{
    // 是不是要在这里验证身份
}
