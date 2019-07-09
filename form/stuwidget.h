#ifndef STUWIDGET_H
#define STUWIDGET_H

#include <QWidget>

namespace Ui {
class stuWidget;
}

class stuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit stuWidget(QWidget *parent = 0);
    ~stuWidget();

    void setDeviceName(const QString &deviceName);
    void setDeviceTemp(double tempValue);
    void setDeviceDamp(double dampValue);
    void setCountAll(int countAll);
    void setCountNoRead(int countNoRead);

private:
    Ui::stuWidget *ui;
};

#endif // STUWIDGET_H
