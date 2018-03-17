#ifndef CURRENTDEVICEMANAGER_H
#define CURRENTDEVICEMANAGER_H

#include <QFileInfoList>
#include <QObject>
#include <QThread>
#include "tools/deviceprovider.h"
#include "tools/mtptool.h"
#include "tools/findmmfiles.h"

class CurrentDeviceManager : public QObject
{
    Q_OBJECT
public:
    static CurrentDeviceManager *getInstance();

    Device *getCurrentDevice();
    MtpTool *getMtpTool();

signals:
    void currentDeviceChanged(Device *m_currentDevice);
    void allDevicePlugOuted();
    void startFindFiles(QString path);
    void startFindFilesSD(QString path);
    // 转发DeviceProvider的信号
    void newDeviceStateInvalid();
    void detectDeviceFailed();
    // 转发MtpTool的信号
    void deviceMounted();
    void deviceMountFailed(QString why);
    void deviceUnMounted();
    void mountProcessExecFailed();
    void unmountProcessExecFailed();

public slots:
    void cancelFindFiles();

private:
    explicit CurrentDeviceManager(QObject *parent = nullptr);
    explicit CurrentDeviceManager(const CurrentDeviceManager &other);
    CurrentDeviceManager &operator=(const CurrentDeviceManager &other);

    DeviceProvider m_deviceProvider;
    Device m_currentDevice;

    MtpTool m_mtpTool;

    QThread m_findFilesThread;
    QThread m_findFilesSDThread;
    FindMMFiles *m_findFiles;
    FindMMFiles *m_findFilesSD;
    int m_findFilesThreadCount;

private:
    void tryToEmitCurrentDeviceChanged();

private slots:
    void onCDMDeviceCountChanged(QList<Device> devices);
    void onNewDeviceStateInvalid();
    void onDetectDeviceFailed();

    void onFindFilesFinished(int allMMFilesCount, QFileInfoList musicFiles);
    void onFindFilesCanceled();
    void onFindFilesSDFinished(int allMMFilesCount, QFileInfoList musicFiles);
    void onFindFilesSDCanceled();

    void onDeviceMounted(QString storagePath, QString sdcardPath, bool hasSDCard);
    void onDeviceMountFailed(QString why);
    void onDeviceUnmounted();
    void onMountProcessExecFailed();
    void onUnMountProcessExecFailed();
};

#endif // CURRENTDEVICEMANAGER_H
