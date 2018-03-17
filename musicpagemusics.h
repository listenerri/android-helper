#ifndef MUSICPAGEMUSICS_H
#define MUSICPAGEMUSICS_H

#include <QTableWidget>
#include <QMediaPlaylist>
#include "musicpagemusicitem.h"

class MusicPageMusics : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPageMusics(QList<MusicPageMusicItem> *musicItems, QWidget *parent = nullptr);

signals:

public slots:
    void removeSelectedMusics();
    void removeMusicItems(QStringList deletedMusicsPath);
    void playSelected();

private:
    void initMusicTable(QList<MusicPageMusicItem> *musicItems, int m_columnCount);
    void setRowBackgroud();

private slots:
    void showContextMenu();

private:
    QTableWidget *m_musicsTable;
    QMediaPlaylist m_playList;
    int m_columnCount;
};

#endif // MUSICPAGEMUSICS_H
