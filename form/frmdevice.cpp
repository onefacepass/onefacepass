#include <QDateTime>
#include "frmdevice.h"
#include "ui_frmdevice.h"
#include "qfontdatabase.h"

frmDevice::frmDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDevice)
{
    ui->setupUi(this);

    //引入图形字体
    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(15);

    ui->labDevice->setFont(iconFont);
    ui->labDevice->setText(QChar(0xf06c));

    ui->labTemp->setFont(iconFont);
    ui->labTemp->setText(QChar(0xf0eb));

//    ui->labDamp->setFont(iconFont);
//    ui->labDamp->setText(QChar(0xf043));
}

frmDevice::~frmDevice()
{
    delete ui;
}

void frmDevice::setDeviceName(const QString &deviceName)
{
    ui->labDeviceName->setText(deviceName);
}

void frmDevice::setDeviceTemp(double tempValue)
{
    ui->labCurrentTemp->setText(QString("照片预留位"));
}

void frmDevice::setCountAll(int countAll)
{
    ui->labCountAll->setText(QString("注册时间："));
}

void frmDevice::setCountNoRead(int countNoRead)
{
    ui->labCountNoRead->setText(QString("%1").arg(qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))));
}
