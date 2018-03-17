#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "currentdevicemanager.h"

class DeviceInfo : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceInfo(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel *deviceModel;
    QLabel *deviceBrand;
    QLabel *deviceAndroidRelease;
    QPushButton *connectButton;

private slots:
    void onButtonClicked();
    void setDeviceInfo(Device *device);
    void clearDeviceInfo();
};

#endif // DEVICEINFO_H
