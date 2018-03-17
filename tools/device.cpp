#include "device.h"
#include <QDebug>

Device::Device(QString dIndicator, QObject *parent) : QObject(parent)
{
    deviceIndicator = dIndicator;
    isMounted = false;
    hasSDCard = false;
    allMMFilesCount = 0;
}

Device::Device(const Device &other)
{
    deviceIndicator = other.deviceIndicator;
    model = other.model;
    brand = other.brand;
    androidRelease = other.androidRelease;
    detailInfo = other.detailInfo;
    isMounted = other.isMounted;
    hasSDCard = other.hasSDCard;
    storagePath = other.storagePath;
    sdcardPath = other.sdcardPath;
    allMMFilesCount = other.allMMFilesCount;
    musicFiles = other.musicFiles;
}

Device &Device::operator=(const Device &other)
{
    if (this == &other) {
        return *this;
    }
    deviceIndicator = other.deviceIndicator;
    model = other.model;
    brand = other.brand;
    androidRelease = other.androidRelease;
    detailInfo = other.detailInfo;
    isMounted = other.isMounted;
    hasSDCard = other.hasSDCard;
    storagePath = other.storagePath;
    sdcardPath = other.sdcardPath;
    allMMFilesCount = other.allMMFilesCount;
    musicFiles = other.musicFiles;
    return *this;
}

bool Device::operator ==(const Device &other) const
{
    return  deviceIndicator == other.deviceIndicator;
}

void Device::resetInfo()
{
    deviceIndicator = "";
    model = "";
    brand = "";
    androidRelease = "";
    detailInfo.clear();
    isMounted = false;
    hasSDCard = false;
    storagePath = "";
    sdcardPath = "";
    allMMFilesCount = 0;
    musicFiles.clear();
}

QString Device::getDeviceIndicator() const
{
    return deviceIndicator;
}

void Device::setDeviceIndicator(const QString &value)
{
    deviceIndicator = value;
}

QString Device::getModel() const
{
    return model;
}

void Device::setModel(const QString &value)
{
    model = value;
}

QString Device::getBrand() const
{
    return brand;
}

void Device::setBrand(const QString &value)
{
    brand = value;
}

QString Device::getAndroidRelease() const
{
    return androidRelease;
}

void Device::setAndroidRelease(const QString &value)
{
    androidRelease = value;
}

QMap<QString, QString> Device::getDetailInfo() const
{
    return detailInfo;
}

void Device::setDetailInfo(const QMap<QString, QString> &value)
{
    detailInfo = value;
}

bool Device::getHasSDCard() const
{
    return hasSDCard;
}

void Device::setHasSDCard(bool value)
{
    hasSDCard = value;
}

QString Device::getStoragePath() const
{
    return storagePath;
}

void Device::setStoragePath(const QString &value)
{
    storagePath = value;
}

QString Device::getSdcardPath() const
{
    return sdcardPath;
}

void Device::setSdcardPath(const QString &value)
{
    sdcardPath = value;
}

int Device::getAllMMFilesCount() const
{
    return allMMFilesCount;
}

void Device::setAllMMFilesCount(int value)
{
    allMMFilesCount = value;
}

QFileInfoList Device::getMusicFiles() const
{
    return musicFiles;
}

void Device::setMusicFiles(const QFileInfoList &value)
{
    musicFiles = value;
}

bool Device::getIsMounted() const
{
    return isMounted;
}

void Device::setIsMounted(bool value)
{
    isMounted = value;
}

// 删除filesPath指定的所有音频文件
void Device::deleteMusicFiles(QStringList filesPath)
{
    QStringList deletedFilesPath;
    QStringList failedFilesPath;
    foreach (QString filePath, filesPath) {
        if (QFile::remove(filePath)) {
            qDebug() << "(Device::deleteMusicFiles) music file deleted:" << filePath;
            deletedFilesPath << filePath;
            musicFiles.removeAll(QFileInfo(filePath));
            allMMFilesCount--;
        } else {
            failedFilesPath << filePath;
        }
    }
    if (deletedFilesPath.count() != 0) {
        emit musicFilesDeleted(deletedFilesPath);
    }
    if (failedFilesPath.count() != 0) {
        emit musicFilesDeletedFailed(failedFilesPath);
    }
}
