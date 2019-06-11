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
}

QUICreator::~QUICreator()
{
    delete ui;
}

void QUICreator::initForm()
{
    initHistoryWidget();
    initNav();
    initCamera();
    initOther();

    initStyle();
    QTimer::singleShot(100, this, SLOT(initPanelWidget()));

    ui->tabWidget->setCurrentIndex(7);
}

// 加载摄像头
void QUICreator::initCamera()
{
    this->camera = new Camera();


    // 加载“设备”菜单：后面现场调试时，可能电脑上会有多个摄像头设备吧！
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    const QList<QCameraInfo> availableCameras = this->camera->get_available_camera_infos();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);

        ui->menu_device->addAction(videoDeviceAction);
    }

    connect(videoDevicesGroup, &QActionGroup::triggered, this, &QUICreator::updateCamera);
    connect(ui->checkbox_camera, &QCheckBox::stateChanged, this, &QUICreator::startAndStopCamera);
//    connect(ui->captureWidget, &QTabWidget::currentChanged, this, &Camera::updateCaptureMode);

    setCamera(this->camera->get_default_camera());
    ui->checkbox_camera->setChecked(true);
}


void QUICreator::startAndStopCamera()
{
    if (ui->checkbox_camera->isChecked()) {
        camera->start();
    } else {
        camera->stop();
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
    this->camera->set_current_camera(cameraInfo);
    connect(this->camera->get_current_camera(), QOverload<QCamera::Error>::of(&QCamera::error), this, &QUICreator::displayCameraError);
    this->camera->setViewFinder(ui->viewfinder);
    this->camera->start();
}



void QUICreator::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), this->camera->get_current_camera()->errorString());
}

// 加载历史记录
void QUICreator::initHistoryWidget()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->history_table_widget->setColumnCount(5);
    ui->history_table_widget->setColumnWidth(0, static_cast<int>(width * 0.06));
    ui->history_table_widget->setColumnWidth(1, static_cast<int>(width * 0.04));
    ui->history_table_widget->setColumnWidth(2, static_cast<int>(width * 0.08));
    ui->history_table_widget->setColumnWidth(3, static_cast<int>(width * 0.08));
    ui->history_table_widget->setColumnWidth(4, static_cast<int>(width * 0.08));
    ui->history_table_widget->verticalHeader()->setDefaultSectionSize(25);

    QStringList headText;
    headText << "学号" << "姓名" << "学院" << "专业" << "上次见面";
    ui->history_table_widget->setHorizontalHeaderLabels(headText);
    ui->history_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->history_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->history_table_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->history_table_widget->setAlternatingRowColors(true);
    ui->history_table_widget->verticalHeader()->setVisible(false);
    ui->history_table_widget->horizontalHeader()->setStretchLastSection(true);

    //设置行高
    ui->history_table_widget->setRowCount(300);

    for (int i = 0; i < 300; i++) {
        ui->history_table_widget->setRowHeight(i, 24);

        QTableWidgetItem *itemDeviceID = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem *itemDeviceName = new QTableWidgetItem(QString("名字%1").arg(i + 1));
        QTableWidgetItem *itemDeviceAddr = new QTableWidgetItem(QString("学院%1").arg(i + 1));
        QTableWidgetItem *itemContent = new QTableWidgetItem(QString("专业%1").arg(i + 1));
        QTableWidgetItem *itemTime = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        ui->history_table_widget->setItem(i, 0, itemDeviceID);
        ui->history_table_widget->setItem(i, 1, itemDeviceName);
        ui->history_table_widget->setItem(i, 2, itemDeviceAddr);
        ui->history_table_widget->setItem(i, 3, itemContent);
        ui->history_table_widget->setItem(i, 4, itemTime);
    }
}

void QUICreator::initNav()
{
    //设置左边按钮菜单
    QList<QPushButton *> btns = ui->widget_vertical_menu->findChildren<QPushButton *>();

    foreach (QPushButton *btn, btns) {
        btn->setCheckable(true);
//        connect(btn, SIGNAL(clicked(bool)), this, SLOT(navBtnClicked()));
        connect(btn, &QPushButton::clicked, this, &QUICreator::navBtnClicked);
    }
    ui->btn_menu_debug->click();
}

void QUICreator::initOther()
{
    ui->rbtn1->setChecked(true);

    ui->checkbox_face->setChecked(false);
    ui->checkbox_pose->setChecked(false);
    ui->checkbox_log->setChecked(false);


    ui->widget_vertical_menu->setProperty("nav", "left");
//    ui->widgetBottom->setProperty("form", "bottom");
    ui->widgetTop->setProperty("nav", "top");

    ui->centralwidget->findChild<QWidget *>("widget_video")->setProperty("video", true);

    ui->btn_pay->setEnabled(false);

    connect(ui->btn_ready_pay, &QPushButton::clicked, this, &QUICreator::btnReadyPayClicked);
    connect(ui->btn_reset_pay, &QPushButton::clicked, this, &QUICreator::resetConsumption);

//    ui->btn_pay->setFixedHeight(70);
//    ui->widget_video->setStyleSheet("border: 1px solid #00bb9e;");
    debugFunc();
}

void QUICreator::navBtnClicked()
{
    QPushButton *btn = static_cast<QPushButton*>(sender());
    QString objName = btn->objectName();

    QList<QPushButton *> btns = ui->widget_vertical_menu->findChildren<QPushButton *>();

    foreach (QPushButton *b, btns) {
        if (b == btn) {
            b->setChecked(true);
        } else {
            b->setChecked(false);
        }
    }

    if (objName == "btn_menu_debug") {
        ui->stacked_widget_debug->setCurrentIndex(0);
    } else if (objName == "btn_menu_work") {
        ui->stacked_widget_debug->setCurrentIndex(1);
    }

}

void QUICreator::on_btnInfo_clicked()
{
    QUIWidget::showMessageBoxInfo("恭喜你,中了500万大奖!请到飞扬青云处领取大奖!");
}

void QUICreator::on_btnQuestion_clicked()
{
    QUIWidget::showMessageBoxQuestion("确定不要当前的500万大奖吗?");
}

void QUICreator::on_btnError_clicked()
{
    QUIWidget::showMessageBoxError("领取500万大奖发生错误,请重试!");
}

void QUICreator::on_btnInput_clicked()
{
    bool ok;
    QString txt = QUIWidget::showInputBox(ok, "请输入姓名:");

    if (ok) {
        QUIWidget::showMessageBoxInfo(QString("您输入的姓名是 : %1").arg(txt));
    }
}

void QUICreator::on_btnInputPwd_clicked()
{
    bool ok;
    QString txt = QUIWidget::showInputBox(ok, "请输入领取500万大奖密码:", 0, 0, "领取500万大奖密码", true);

    if (ok) {
        QUIWidget::showMessageBoxInfo(QString("您输入的密码是 : %1").arg(txt));
    }
}

void QUICreator::on_btnInputcbox_clicked()
{
    bool ok;
    QStringList defaultValue;

    for (int i = 1; i <= 50; i++) {
        defaultValue.append(QString::number(i));
    }

    QString txt = QUIWidget::showInputBox(ok, "请选择学号:", 1, 0, defaultValue.join("|"));

    if (ok) {
        QUIWidget::showMessageBoxInfo(QString("您选择的学号是 : %1").arg(txt));
    }
}

void QUICreator::on_btnWidget_clicked()
{
    //两种方式弹出都可以
#if 0
    QUIWidget *qui = new QUIWidget;
    qui->setTitle("新窗体测试");
    qui->setAlignment(Qt::AlignCenter);
    qui->setMainWidget(new QWidget);
    qui->show();
#else
    QUIWidget qui;
    qui.setTitle("新窗体测试");
    qui.setAlignment(Qt::AlignCenter);
    qui.setMainWidget(new QWidget);
    qui.setOnlyCloseBtn();
    qui.exec();
#endif
}




void QUICreator::initStyle()
{
    QString fileName = ":/qss/darkblue.css";

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            currentFile = fileName;
            currentQSS = str;
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

void QUICreator::debugFunc()
{
    insertAnLog("log1");
    insertAnLog("log2");
    insertAnLog("log3");
}

/* @func: 向调试界面插入一条日志
 * @str: 待插入的日志，无需关注结尾是否有'\n'
 */
void QUICreator::insertAnLog(const QString& str)
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

    ui->plaintext_log->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    ui->plaintext_log->insertPlainText(log);
}

// 清零消费金额
void QUICreator::resetConsumption()
{
    ui->spinbox_pay->setValue(0);
    ui->btn_pay->setEnabled(false);
}

void QUICreator::btnReadyPayClicked()
{
    ui->btn_pay->setEnabled(true);
    ui->btn_pay->setFocus();
}


/* @func: 触发支付按钮后的动作放这里
 *
 */
void QUICreator::btnPayClicked()
{
    // 是不是要在这里验证身份
}
