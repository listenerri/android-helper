#ifndef DEVICEPROVIDER_H
#define DEVICEPROVIDER_H

#include <QObject>
#include <QList>
#include "device.h"
#include "tools/adbtool.h"

class DeviceProvider : public QObject
{
    Q_OBJECT
public:
    explicit DeviceProvider(QObject *parent = nullptr);
    QList<Device> m_devices;

signals:
    void deviceCountChanged(QList<Device> devices);
    void newDeviceStateInvalid();
    void detectDeviceFailed();

public slots:

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

private:
    AdbTool m_adbTool;
    int m_timerId;
    // 保存那些已插入但是状态不正常的设备的SerialNumber
    QStringList m_stateInvalidSNList;

private slots:
    void onDeviceConnectInfoReady(QStringList deviceIndicatorList);
    void onAdbProcessExecFailed(AdbTool::ProcessType processType);
    void onDeviceDetailInfoReady(QMap<QString, QString> detailInfoMap);

private:
    void addDevice(QString deviceIndicator);
    QStringList splitIndicator(QString deviceIndicator);
    QString getSerialNumber(QString deviceIndicator);
    bool isDeviceStateValid(QString deviceIndicator);
};

#endif // DEVICEPROVIDER_H
