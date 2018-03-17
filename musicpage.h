#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>
#include <QList>
#include "tools/device.h"
#include "musicpagemusicitem.h"
#include "toolsbar.h"
#include "musicpageartist.h"
#include "musicpagealbum.h"
#include "musicpagemusics.h"
#include "musicpagedirs.h"

class MusicPage : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPage(QWidget *parent = nullptr);
    static ToolsBar *getToolsBar();

signals:

private slots:
    void resetPagesConnect(int currentPageIndex);
    void removeMusicItems(QStringList deletedMusicsPath);

private:
    ToolsBar *toolsBar;
    Device *CurrentDevice;
    QList<MusicPageMusicItem> m_musicItems;
    MusicPageArtist *m_artistPage;
    MusicPageAlbum *m_albumPage;
    MusicPageMusics *m_musicsPage;
    MusicPageDirs *m_dirsPage;

private:
    void initMusicItems();
};

#endif // MUSICPAGE_H
