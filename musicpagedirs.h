#ifndef MUSICPAGEDIRS_H
#define MUSICPAGEDIRS_H

#include <QListWidget>
#include <QMediaPlaylist>
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>
#include "musicpagemusicitem.h"

class MusicPageDirs : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPageDirs(QList<MusicPageMusicItem> *musicItems, QWidget *parent = nullptr);

signals:

public slots:
    void removeSelectedMusics();
    void removeMusicItems(QStringList deletedMusicsPath);
    void playSelectedMusic();

private slots:
    void changeToList(int row);
    void backToDirs();
    void showMusicContextMenu();

private:
    void initMap(QList<MusicPageMusicItem> *musicItems);
    void initStack();
    void initDirTable(QTableWidget *dirTable);
    void setTableRowBackgroud(QTableWidget *dirTable);
    void setListItemStyle(QListWidget *list, bool setItemHigh = false);

private:
    QPushButton *m_backButton;
    QStackedWidget *m_stack;
    QMap<QString, QListWidget *> m_dirMap;
    QMap<QString, QMediaPlaylist *> m_playlistMap;
};

#endif // MUSICPAGEDIRS_H
