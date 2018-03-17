#ifndef MUSICPAGEALBUM_H
#define MUSICPAGEALBUM_H

#include <QListWidget>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include "musicpagemusicitem.h"

class MusicPageAlbum : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPageAlbum(QList<MusicPageMusicItem> *musicItems, QWidget *parent = nullptr);

signals:

public slots:
    void removeSelectedMusics();
    void removeMusicItems(QStringList deletedMusicsPath);
    void playSelectedMusic();

private slots:
    void changeToList(QListWidgetItem *item);
    void backToAlbumList();
    void showMusicContextMenu();

private:
    void initMap(QList<MusicPageMusicItem> *musicItems);
    void initUI();
    void setListItemBackground(QListWidget *list);

private:
    const QSize DefaultIconSize = QSize(100, 100);
    QPushButton *m_backButton;
    QStackedWidget *m_stack;
    QMap<QString, QListWidget *> m_albumMap;
    QMap<QString, QMediaPlaylist *> m_playlistMap;
};

#endif // MUSICPAGEALBUM_H
