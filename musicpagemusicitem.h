#ifndef MUSICPAGEMUSICITEM_H
#define MUSICPAGEMUSICITEM_H

#include <QObject>

class MusicPageMusicItem : public QObject
{
    Q_OBJECT
public:
    explicit MusicPageMusicItem(QString filePath, QObject *parent = nullptr);
    explicit MusicPageMusicItem(QString filePath, QString parentDir, QObject *parent = nullptr);
    MusicPageMusicItem(const MusicPageMusicItem &other);

    MusicPageMusicItem& operator=(const MusicPageMusicItem &other);
    bool operator ==(const MusicPageMusicItem &other) const;

    QString filePath() const;
    void setFilePath(const QString &filePath);

    QString parentDir() const;
    void setParentDir(const QString &parentDir);

    QString title() const;
    void setTitle(const QString &title);

    QString artist() const;
    void setArtist(const QString &artist);

    QString album() const;
    void setAlbum(const QString &album);

    QString year() const;
    void setYear(const QString &year);

    QString time() const;
    void setTime(const QString &time);

private:
    QString m_filePath, m_parentDir, m_title, m_artist, m_album, m_year, m_time;

signals:

public slots:
};

#endif // MUSICPAGEMUSICITEM_H
