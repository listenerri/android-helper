#include "musicpagemusicitem.h"

MusicPageMusicItem::MusicPageMusicItem(QString filePath, QObject *parent)
{
    m_filePath = filePath;
}

MusicPageMusicItem::MusicPageMusicItem(QString filePath, QString parentDir, QObject *parent) : QObject(parent)
{
    m_filePath = filePath;
    m_parentDir = parentDir;
}

MusicPageMusicItem::MusicPageMusicItem(const MusicPageMusicItem &other)
{
    m_filePath = other.filePath();
    m_parentDir = other.parentDir();
    m_title = other.title();
    m_artist = other.artist();
    m_album = other.album();
    m_year = other.year();
    m_time = other.time();
}

MusicPageMusicItem &MusicPageMusicItem::operator=(const MusicPageMusicItem &other)
{
    if (this == &other) {
        return *this;
    }
    m_filePath = other.filePath();
    m_parentDir = other.parentDir();
    m_title = other.title();
    m_artist = other.artist();
    m_album = other.album();
    m_year = other.year();
    m_time = other.time();
    return *this;
}

bool MusicPageMusicItem::operator ==(const MusicPageMusicItem &other) const
{
    return m_filePath == other.filePath();
}

QString MusicPageMusicItem::filePath() const
{
    return m_filePath;
}

void MusicPageMusicItem::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

QString MusicPageMusicItem::parentDir() const
{
    return m_parentDir;
}

void MusicPageMusicItem::setParentDir(const QString &parentDir)
{
    m_parentDir = parentDir;
}

QString MusicPageMusicItem::title() const
{
    return m_title;
}

void MusicPageMusicItem::setTitle(const QString &title)
{
    m_title = title;
}

QString MusicPageMusicItem::artist() const
{
    return m_artist;
}

void MusicPageMusicItem::setArtist(const QString &artist)
{
    m_artist = artist;
}

QString MusicPageMusicItem::album() const
{
    return m_album;
}

void MusicPageMusicItem::setAlbum(const QString &album)
{
    m_album = album;
}

QString MusicPageMusicItem::year() const
{
    return m_year;
}

void MusicPageMusicItem::setYear(const QString &year)
{
    m_year = year;
}

QString MusicPageMusicItem::time() const
{
    return m_time;
}

void MusicPageMusicItem::setTime(const QString &time)
{
    m_time = time;
}
