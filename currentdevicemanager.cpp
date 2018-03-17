#include <QDebug>
#include "currentdevicemanager.h"

CurrentDeviceManager *CurrentDeviceManager::getInstance()
{
    static CurrentDeviceManager currentDeviceManager;
    return &currentDeviceManager;
}

CurrentDeviceManager::CurrentDeviceManager(QObject *parent) : QObject(parent)
{
    m_findFilesThreadCount = 0;

    connect(&m_deviceProvider, &DeviceProvider::deviceCountChanged, this, &CurrentDeviceManager::onCDMDeviceCountChanged);
    connect(&m_deviceProvider, &DeviceProvider::newDeviceStateInvalid, this, &CurrentDeviceManager::onNewDeviceStateInvalid);
    connect(&m_deviceProvider, &DeviceProvider::detectDeviceFailed, this, &CurrentDeviceManager::onDetectDeviceFailed);

    connect(&m_mtpTool, &MtpTool::deviceMounted, this, &CurrentDeviceManager::onDeviceMounted);
    connect(&m_mtpTool, &MtpTool::deviceMountFailed, this, &CurrentDeviceManager::onDeviceMountFailed);
    connect(&m_mtpTool, &MtpTool::deviceUnmounted, this, &CurrentDeviceManager::onDeviceUnmounted);
    connect(&m_mtpTool, &MtpTool::mountProcessExecFailed, this, &CurrentDeviceManager::onMountProcessExecFailed);
    connect(&m_mtpTool, &MtpTool::unmountProcessExecFailed, this, &CurrentDeviceManager::onUnMountProcessExecFailed);
}

Device *CurrentDeviceManager::getCurrentDevice()
{
    return &m_currentDevice;
}

MtpTool *CurrentDeviceManager::getMtpTool()
{
    return &m_mtpTool;
}

void CurrentDeviceManager::onCDMDeviceCountChanged(QList<Device> devices)
{
    // 如果当前设备已不在已插入设备列表中
    if (!devices.contains(m_currentDevice)) {
        if (m_currentDevice.getIsMounted()) {
            // 卸载已移除的设备
            cancelFindFiles();
            qDebug() << "(CurrentDeviceManager::onCDMDeviceCountChanged) unmount previous device";
            m_mtpTool.unmount();
        }

        // 且已插入设备列表不为空
        if (!devices.isEmpty()) {
            // 选取列表中的第一个设备作为当前设备
            m_currentDevice = devices[0];
            // 挂载mtp
            qDebug() << "(CurrentDeviceManager::onCDMDeviceCountChanged) mount new device";
            m_mtpTool.mount();
        } else {
            // 将当前设备信息重置为空
            m_currentDevice.resetInfo();
            qDebug() << "(CurrentDeviceManager::onCDMDeviceCountChanged) emit allDevicePlugOuted!!!";
            emit allDevicePlugOuted();
        }
    }
}

void CurrentDeviceManager::onNewDeviceStateInvalid()
{
    // 转发DeviceProvider的信号
    emit newDeviceStateInvalid();
}

void CurrentDeviceManager::onDetectDeviceFailed()
{
    // 转发DeviceProvider的信号
    emit detectDeviceFailed();
}

void CurrentDeviceManager::cancelFindFiles()
{
    if (m_findFilesThread.isRunning() || m_findFilesSDThread.isRunning()) {
        qDebug() << "(CurrentDeviceManager::cancelFindFiles) cancel find file!!!";
        FindMMFiles::CancelFlag = true;
    } else {
        qDebug() << "(CurrentDeviceManager::cancelFindFiles) find file thread is not running!!!";
    }
}

void CurrentDeviceManager::onFindFilesFinished(int allMMFilesCount, QFileInfoList musicFiles)
{
    m_currentDevice.setAllMMFilesCount(m_currentDevice.getAllMMFilesCount() + allMMFilesCount);
    m_currentDevice.setMusicFiles(m_currentDevice.getMusicFiles() << musicFiles);
    m_findFilesThread.quit();
    m_findFilesThreadCount--;
    delete m_findFiles;
    qDebug() << "(CurrentDeviceManager::onFindFilesFinished) storage all multimedia files count: " << allMMFilesCount;
    qDebug() << "(CurrentDeviceManager::onFindFilesFinished) storage music files count: " << musicFiles.count();
    tryToEmitCurrentDeviceChanged();
}

void CurrentDeviceManager::onFindFilesSDFinished(int allMMFilesCount, QFileInfoList musicFiles)
{
    m_currentDevice.setAllMMFilesCount(m_currentDevice.getAllMMFilesCount() + allMMFilesCount);
    m_currentDevice.setMusicFiles(m_currentDevice.getMusicFiles() << musicFiles);
    m_findFilesSDThread.quit();
    m_findFilesThreadCount--;
    delete m_findFilesSD;
    qDebug() << "(CurrentDeviceManager::onFindFilesSDFinished) sd card all multimedia files count: " << allMMFilesCount;
    qDebug() << "(CurrentDeviceManager::onFindFilesSDFinished) sd card music files count: " << musicFiles.count();
    tryToEmitCurrentDeviceChanged();
}

void CurrentDeviceManager::tryToEmitCurrentDeviceChanged()
{
    if (m_findFilesThreadCount <= 0) {
        qDebug() << "(CurrentDeviceManager::tryToEmitCurrentDeviceChanged) all multimedia files count: " << m_currentDevice.getAllMMFilesCount();
        qDebug() << "(CurrentDeviceManager::tryToEmitCurrentDeviceChanged) music files count: " << m_currentDevice.getMusicFiles().count();
        qDebug() << "(CurrentDeviceManager::tryToEmitCurrentDeviceChanged) emit currentDeviceChanged!!!";
        emit currentDeviceChanged(&m_currentDevice);
    }
}

void CurrentDeviceManager::onFindFilesCanceled()
{
    qDebug() << "(CurrentDeviceManager::onFindFilesCanceled) find file thread canceled!!!";
    m_findFilesThread.quit();
    m_findFilesThreadCount--;
    delete m_findFiles;
}

void CurrentDeviceManager::onFindFilesSDCanceled()
{
    qDebug() << "(CurrentDeviceManager::onFindFilesSDCanceled) find file sd card thread canceled!!!";
    m_findFilesSDThread.quit();
    m_findFilesThreadCount--;
    delete m_findFilesSD;
}

void CurrentDeviceManager::onDeviceMounted(QString storagePath, QString sdcardPath, bool hasSDCard)
{
    // 转发MtpTool的信号
    emit deviceMounted();

    m_currentDevice.setIsMounted(true);
    m_currentDevice.setHasSDCard(hasSDCard);
    m_currentDevice.setStoragePath(storagePath);
    m_currentDevice.setSdcardPath(sdcardPath);
    // 启动获取手机文件的线程
    FindMMFiles::CancelFlag = false;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    m_findFiles = new FindMMFiles;
    connect(this, &CurrentDeviceManager::startFindFiles, m_findFiles, &FindMMFiles::doFindMMFiles);
    connect(m_findFiles, &FindMMFiles::findMMFilesFinished, this, &CurrentDeviceManager::onFindFilesFinished);
    connect(m_findFiles, &FindMMFiles::findMMFilesCanceled, this, &CurrentDeviceManager::onFindFilesCanceled);
    m_findFiles->moveToThread(&m_findFilesThread);
    m_findFilesThread.start();
    m_findFilesThreadCount++;
    emit startFindFiles(storagePath);
    // SD卡
    if (m_currentDevice.getHasSDCard()) {
        m_findFilesSD = new FindMMFiles;
        connect(this, &CurrentDeviceManager::startFindFilesSD, m_findFilesSD, &FindMMFiles::doFindMMFiles);
        connect(m_findFilesSD, &FindMMFiles::findMMFilesFinished, this, &CurrentDeviceManager::onFindFilesSDFinished);
        connect(m_findFilesSD, &FindMMFiles::findMMFilesCanceled, this, &CurrentDeviceManager::onFindFilesSDCanceled);
        m_findFilesSD->moveToThread(&m_findFilesSDThread);
        m_findFilesSDThread.start();
        m_findFilesThreadCount++;
        emit startFindFilesSD(sdcardPath);
    }
}

void CurrentDeviceManager::onDeviceMountFailed(QString why)
{
    // 转发MtpTool的信号
    emit deviceMountFailed(why);
}

void CurrentDeviceManager::onDeviceUnmounted()
{
    m_currentDevice.setIsMounted(false);

    // 转发MtpTool的信号
    emit deviceUnMounted();
}

void CurrentDeviceManager::onMountProcessExecFailed()
{
    // 转发MtpTool的信号
    emit mountProcessExecFailed();
}

void CurrentDeviceManager::onUnMountProcessExecFailed()
{
    // 转发MtpTool的信号
    emit unmountProcessExecFailed();
}
