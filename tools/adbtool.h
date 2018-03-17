#ifndef ADBTOOL_H
#define ADBTOOL_H

#include <QObject>
#include <QProcess>
#include <QMap>

class AdbTool : public QObject
{
    Q_OBJECT
public:
    enum ProcessType {DetectDevice, GetDeviceDetailInfo};
    explicit AdbTool(QObject *parent = nullptr);

public:
    void startProcess(AdbTool::ProcessType processType);
    void startProcess(AdbTool::ProcessType processType, QString deviceSerialNumber);

signals:
    void processExecFailed(AdbTool::ProcessType processType);
    void deviceConnectInfoReady(QStringList deviceList);
    void deviceDetailInfoReady(QMap<QString, QString> resultStringMap);

public slots:

private:
    QProcess m_adbProcess;
    ProcessType m_processType;
    int m_deviceCount = 0;

private:
    void doStart(QString arg, QString deviceSerialNumber);
    void handleConnectInfo(QString resultString);
    void tryToRemoveExtraInfo(QStringList &resultStringList);
    void handleDeviceDetailInfo(QString resultString);

private slots:
    void handleResult(int exitCode, QProcess::ExitStatus exitStatus);
    void onErrorOccurred(QProcess::ProcessError error);
};

#endif // ADBTOOL_H
