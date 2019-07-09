#include "stuwidget.h"
#include "ui_stuwidget.h"
#include "qfontdatabase.h"

stuWidget::stuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stuWidget)
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

    ui->labDamp->setFont(iconFont);
    ui->labDamp->setText(QChar(0xf043));
}

stuWidget::~stuWidget()
{
    delete ui;
}

void stuWidget::setDeviceName(const QString &deviceName)
{
    ui->labDeviceName->setText(deviceName);
}

void stuWidget::setDeviceTemp(double tempValue)
{
    ui->labCurrentTemp->setText(QString("温度 : %1 ℃").arg(tempValue));
}

void stuWidget::setDeviceDamp(double dampValue)
{
    ui->labCurrentDamp->setText(QString("温度 : %1 %RH").arg(dampValue));
}

void stuWidget::setCountAll(int countAll)
{
    ui->labCountAll->setText(QString("总条数 : %1").arg(countAll));
}

void stuWidget::setCountNoRead(int countNoRead)
{
    ui->labCountNoRead->setText(QString("未读数 : %1").arg(countNoRead));
}
