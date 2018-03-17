#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QMap>
#include <QFileInfoList>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QString dIndicator = "", QObject *parent = nullptr);
    Device(const Device &other);

    Device& operator=(const Device &other);
    bool operator ==(const Device &other) const;

    void resetInfo();

    QString getDeviceIndicator() const;
    void setDeviceIndicator(const QString &value);

    QString getModel() const;
    void setModel(const QString &value);

    QString getBrand() const;
    void setBrand(const QString &value);

    QString getAndroidRelease() const;
    void setAndroidRelease(const QString &value);

    QMap<QString, QString> getDetailInfo() const;
    void setDetailInfo(const QMap<QString, QString> &value);

    bool getHasSDCard() const;
    void setHasSDCard(bool value);

    QString getStoragePath() const;
    void setStoragePath(const QString &value);

    QString getSdcardPath() const;
    void setSdcardPath(const QString &value);

    int getAllMMFilesCount() const;
    void setAllMMFilesCount(int value);

    QFileInfoList getMusicFiles() const;
    void setMusicFiles(const QFileInfoList &value);

    bool getIsMounted() const;
    void setIsMounted(bool value);

signals:
    void musicFilesDeleted(QStringList deletedFilesPath);
    void musicFilesDeletedFailed(QStringList failedFilesPath);

public slots:
    void deleteMusicFiles(QStringList filesPath);

private:
    QString deviceIndicator;
    QString model;
    QString brand;
    QString androidRelease;
    QMap<QString, QString> detailInfo;
    bool isMounted;
    bool hasSDCard;
    QString storagePath;
    QString sdcardPath;
    int allMMFilesCount;
    QFileInfoList musicFiles;
};

#endif // DEVICE_H
