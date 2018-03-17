#ifndef MUSICPAGEARTIST_H
#define MUSICPAGEARTIST_H

#include <QMap>
#include <QListWidget>
#include <QStackedWidget>
#include <QMediaPlaylist>
#include "musicpagemusicitem.h"

class MusicPageArtist : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPageArtist(QList<MusicPageMusicItem> *musicItems, QWidget *parent = nullptr);

signals:

public slots:
    void removeSelectedMusics();
    void removeMusicItems(QStringList deletedMusicsPath);
    void playSelectedMusic();

private:
    void initMap(QList<MusicPageMusicItem> *musicItems);
    void initUI();
    void setListItemStyle(QListWidget *list, bool setItemHigh = false, bool isArtistList = false);

private slots:
    void showContextMenu();

private:
    QMap<QString, QListWidget *> m_musicListMap;
    QMap<QString, QMediaPlaylist *> m_playlistMap;
    QListWidget *m_artistList;
    QStackedWidget *m_stack;
};

#endif // MUSICPAGEARTIST_H
