#include "stuwidget.h"
#include "ui_stuwidget.h"
#include "qfontdatabase.h"

StuWidget::StuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StuWidget)
{
    ui->setupUi(this);


    setFixedWidth(150);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);


    ui->labPhoto->setAlignment(Qt::AlignCenter);
    ui->labMajor->setAlignment(Qt::AlignCenter);

    //引入图形字体
    int fontId = QFontDatabase::addApplicationFont(":/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPixelSize(15);

    ui->labIconId->setFont(iconFont);
    ui->labIconId->setText(QChar(0xf015));

    ui->labIconName->setFont(iconFont);
    ui->labName->setText(QChar(0xf007));
}

StuWidget::~StuWidget()
{
    delete ui;
}

void StuWidget::setID(const QString &id)
{
    ui->labID->setText(QString("学号: %1").arg(id));
//    ui->labID->setText(id);
}

void StuWidget::setName(const QString &name)
{
    ui->labName->setText(QString("姓名: %1").arg(name));
//    ui->labName->setText(name);
}

void StuWidget::setMajor(const QString &major)
{
    ui->labMajor->setText(major);
//    ui->labMajor->setText(QString("专业: %1").arg(major));
}

void StuWidget::setPhoto(const QString &photo)
{
    QImage *pho = new QImage();
    pho->load(photo);
    *pho = pho->scaled(130,173);

    ui->labPhoto->setPixmap(QPixmap::fromImage(*pho));

}
