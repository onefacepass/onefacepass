#ifndef STUWIDGET_H
#define STUWIDGET_H

#include <QWidget>

namespace Ui {
class StuWidget;
}

class StuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StuWidget(QWidget *parent = 0);
    ~StuWidget();

    void setName(const QString &name);
    void setID(const QString &id);
    void setMajor(const QString &major);
    void setPhoto(const QString &photo);

private:
    Ui::StuWidget *ui;
};

#endif // STUWIDGET_H
