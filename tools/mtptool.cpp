#include "mtptool.h"
#include "musicplayer.h"
#include <QDebug>

QDir MtpTool::MountPoint = QDir::homePath() + "/.local/share/android-helper/mtpmount";

MtpTool::MtpTool(QObject *parent) : QObject(parent)
{
    m_storagePath = "";
    m_sdCardPath = "";
    m_hasSDCard = false;

    connect(&m_mountProcess, &QProcess::errorOccurred, this, &MtpTool::onMountErrorOccurred);
    connect(&m_mountProcess, &QProcess::started, this, &MtpTool::handleMount);

    connect(&m_unmountProcess, &QProcess::errorOccurred, this, &MtpTool::onUnMountErrorOccurred);
    connect(&m_unmountProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &MtpTool::handleUnMount);
}

MtpTool::~MtpTool()
{
    qDebug() << "(~MtpTool) unmount device!";
    m_unmountProcess.setProgram("fusermount");
    m_unmountProcess.setArguments(QStringList("-u") << MountPoint.absolutePath());
    m_unmountProcess.start();
    m_unmountProcess.waitForFinished();
}

void MtpTool::mount(QDir mountPoint)
{
    MountPoint = mountPoint;
    if (!MountPoint.exists()) {
        MountPoint.mkpath(MountPoint.absolutePath());
    }
    if (m_mountProcess.state() == QProcess::NotRunning) {
        m_mountProcess.setProgram("go-mtpfs");
        m_mountProcess.setArguments(QStringList(MountPoint.absolutePath()));
        m_mountProcess.start();
    } else {
        qDebug() << "(MtpTool::mount) mount process is running!";
    }
}

void MtpTool::unmount(bool force, QDir mountPoint)
{
    qDebug() << "(MtpTool::unmount) stop play music and clear playlist before unmount device";
    MusicPlayer::stopAndClear();

    MountPoint = mountPoint;
    m_unmountProcess.setProgram("fusermount");
    m_unmountProcess.setArguments(QStringList("-u") << MountPoint.absolutePath());
    if (force) {
        qDebug() << "(MtpTool::unmount) force unmount device!";
        m_unmountProcess.start();
        m_unmountProcess.waitForFinished();
    }else if (m_mountProcess.state() == QProcess::Running) {
        m_unmountProcess.start();
        m_unmountProcess.waitForFinished();
    } else {
        qDebug() << "(MtpTool::unmount) mount process not running!";
    }
}

void MtpTool::handleMount()
{
    qDebug() << "(MtpTool::handleMount) start try to get storage path.";
    timerID = startTimer(100);
}

void MtpTool::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    tryCount++;
    qDebug() << "(MtpTool::timerEventt) try to get storage path times:" << tryCount;
    QFileInfoList storageInfoList = MountPoint.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    if (storageInfoList.count() != 0) {
        tryCount = 0;
        getStoragePath(storageInfoList);
        killTimer(timerID);
    }
    if (tryCount >= 3) {
        tryCount = 0;
        killTimer(timerID);
        mountFailed("no storage found!");
    }
}

void MtpTool::getStoragePath(QFileInfoList storageInfoList)
{
    // TODO: 需要更好的方式判断内部存储和sd卡
    foreach (auto fileInfo, storageInfoList) {
        if (fileInfo.fileName().contains("设备")) {
            m_storagePath = fileInfo.absoluteFilePath();
        } else {
            m_sdCardPath = fileInfo.absoluteFilePath();
            m_hasSDCard = true;
        }
    }
    if (m_storagePath.length() == 0) {
        mountFailed("can not get storage path!");
        return;
    } else if (m_hasSDCard && m_sdCardPath.length() == 0) {
        mountFailed("can not get sdcard path!");
        return;
    } else {
        qDebug() << "(MtpTool::getStoragePath) device mounted!";
        emit deviceMounted(m_storagePath, m_sdCardPath, m_hasSDCard);
    }
}

void MtpTool::mountFailed(QString why)
{
    qDebug() << "(MtpTool::mountFailed) device mount failed! because " << why;
    emit deviceMountFailed(why);
    unmount();
}

void MtpTool::handleUnMount(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
        qDebug() << "(MtpTool::handleUnMount) device unMounted!";
        emit deviceUnmounted();
    } else {
        qDebug() << "(MtpTool::handleUnMount) fusermount exec failed!!!!" << "exitCode: " << exitCode << "exitStatus: " << exitStatus;
        emit unmountProcessExecFailed();
    }
}

void MtpTool::onMountErrorOccurred(QProcess::ProcessError error)
{
    qDebug() << "(MtpTool::onErrorOccurred) go-mtpfs exec failed!!!!" << "error: " << error;
    emit mountProcessExecFailed();
}

void MtpTool::onUnMountErrorOccurred(QProcess::ProcessError error)
{
    qDebug() << "(MtpTool::onErrorOccurred) fusermount exec failed!!!!" << "error: " << error;
    emit unmountProcessExecFailed();
}
