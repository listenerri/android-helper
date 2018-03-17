#ifndef MTPTOOL_H
#define MTPTOOL_H

#include <QObject>
#include <QProcess>
#include <QDir>

class MtpTool : public QObject
{
    Q_OBJECT
public:
    explicit MtpTool(QObject *parent = nullptr);
    ~MtpTool();
    static QDir MountPoint;
    QString m_storagePath;
    QString m_sdCardPath;
    bool m_hasSDCard;

public:
    void mount(QDir mountPoint = MountPoint);
    void unmount(bool force = false, QDir mountPoint = MountPoint);

signals:
    void mountProcessExecFailed();
    void unmountProcessExecFailed();
    void deviceMounted(QString storagePath, QString sdCardPath, bool hasSDCard);
    void deviceMountFailed(QString why);
    void deviceUnmounted();

public slots:

private:
    QProcess m_mountProcess;
    QProcess m_unmountProcess;
    int tryCount;
    int timerID;

private:
    void mountFailed(QString why);
    void getStoragePath(QFileInfoList storageInfoList);

private slots:
    void handleMount();
    void handleUnMount(int exitCode, QProcess::ExitStatus exitStatus);
    void onMountErrorOccurred(QProcess::ProcessError error);
    void onUnMountErrorOccurred(QProcess::ProcessError error);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MTPTOOL_H
