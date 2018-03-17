#include "musicpageartist.h"
#include "splitline.h"
#include "currentdevicemanager.h"
#include "musicplayer.h"
#include <QHBoxLayout>
#include <QDebug>
#include <ddialog.h>
#include <QMenu>

DWIDGET_USE_NAMESPACE

MusicPageArtist::MusicPageArtist(QList<MusicPageMusicItem> *musicItems, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainHBLayout = new QHBoxLayout(this);
    m_artistList = new QListWidget(this);
    m_stack = new QStackedWidget(this);

    initMap(musicItems);
    initUI();

    Device *currentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();
    connect(currentDevice, &Device::musicFilesDeleted, this, &MusicPageArtist::removeMusicItems);
    connect(m_artistList, &QListWidget::currentRowChanged, m_stack, &QStackedWidget::setCurrentIndex);

    mainHBLayout->setSpacing(0);
    mainHBLayout->setContentsMargins(0, 0, 0, 0);

    mainHBLayout->addWidget(m_artistList);
    mainHBLayout->addWidget(new SplitLine(SplitLine::Vertical, this));
    mainHBLayout->addWidget(m_stack);

    this->setLayout(mainHBLayout);
}

void MusicPageArtist::initMap(QList<MusicPageMusicItem> *musicItems)
{
    foreach (MusicPageMusicItem musicItem, *musicItems) {
        QString artist = musicItem.artist();
        QString title = musicItem.title();
        QString filePath = musicItem.filePath();

        QListWidgetItem *listItem = new QListWidgetItem(title);
        listItem->setWhatsThis(filePath);
        QListWidget *list;
        QMediaPlaylist *playList;

        // 如果歌曲中没有歌手信息(长度为零)将其设置为"unknown"
        if (artist.count() == 0) {
            artist = "unknown";
        }
        if (m_musicListMap.contains(artist)) {
            list = m_musicListMap.value(artist);
            playList  = m_playlistMap.value(artist);
        } else {
            list = new QListWidget(m_stack);
            playList = new QMediaPlaylist(this);
            m_musicListMap.insert(artist, list);
            m_playlistMap.insert(artist, playList);
        }
        list->addItem(listItem);
        playList->addMedia(QUrl::fromLocalFile(filePath));
    }
}

void MusicPageArtist::initUI()
{
    QList<QString> artistList = m_musicListMap.keys();
    foreach (QString artist, artistList) {
        m_artistList->addItem(new QListWidgetItem(artist));
        QListWidget * list = m_musicListMap.value(artist);
        list->setSelectionMode(QListWidget::ExtendedSelection);
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list, &QListWidget::customContextMenuRequested, this, &MusicPageArtist::showContextMenu);
        connect(list, &QListWidget::itemDoubleClicked, this, &MusicPageArtist::playSelectedMusic);
        setListItemStyle(list, true);
        m_stack->addWidget(list);
    }
    m_artistList->setFixedWidth(200);
    setListItemStyle(m_artistList, true, true);
}

void MusicPageArtist::setListItemStyle(QListWidget *list, bool setItemHigh, bool isArtistList)
{
    for (int row = 0; row < list->count(); ++row) {
        QListWidgetItem *item = list->item(row);
        // 设置行高
        if (setItemHigh) {
            if (isArtistList) {
                item->setSizeHint(QSize(0, 40));
            } else {
                item->setSizeHint(QSize(0, 30));
            }
        }
        //设置背景
        if (row % 2 != 0) {
            item->setBackgroundColor(QColor("#f5f6f7"));
        } else {
            item->setBackgroundColor(QColor("#ffffff"));
        }
    }
}

void MusicPageArtist::showContextMenu()
{
    QMenu contextMenu(this);
    contextMenu.addAction("Play");
    contextMenu.addSeparator();
    contextMenu.addAction("Delete");
    QAction *selectedAction = contextMenu.exec(QCursor::pos());
    if (selectedAction != 0) {
        if (selectedAction->text() == "Play") {
            playSelectedMusic();
        } else if (selectedAction->text() == "Delete") {
            removeSelectedMusics();
        }
    }
}

// 获取当前选择的项目并通过信号传递给MusicPage类去删除文件
void MusicPageArtist::removeSelectedMusics()
{
    QListWidget * currentList = static_cast<QListWidget *>(m_stack->currentWidget());
    QList<QListWidgetItem *> selectedMusicItems = currentList->selectedItems();
    QStringList selectedMusicsPath;
    QString selectedMusicsName;
    foreach (QListWidgetItem *item, selectedMusicItems) {
        QString path = item->whatsThis();
        if (!selectedMusicsPath.contains(path)) {
            selectedMusicsPath << path;
            selectedMusicsName += item->text() + "\n";
        }
    }
    if (selectedMusicsPath.count() > 0) {
        DDialog delDialog("Delete:", selectedMusicsName, this);
        delDialog.addButton("Cancel");
        delDialog.addButton("Delete", true, DDialog::ButtonWarning);
        if (delDialog.exec() == DDialog::Accepted) {
            CurrentDeviceManager::getInstance()->getCurrentDevice()->deleteMusicFiles(selectedMusicsPath);
        } else {
            qDebug() << "(MusicPageArtist::removeSelectedMusics) cancel delete selection.";
        }
    } else {
        qDebug() << "(MusicPageArtist::removeSelectedMusics) no selected music!";
    }
}

// 收到文件删除的信号后更新ui
void MusicPageArtist::removeMusicItems(QStringList deletedMusicsPath)
{
    for (int i = 0; i < m_stack->count(); ++i) {
        QListWidget * list = static_cast<QListWidget *>(m_stack->widget(i));
        QMediaPlaylist * playList = m_playlistMap.value(m_musicListMap.key(list));
        // 获取要移除的行
        QList<int> rows;
        for (int row = 0; row < list->count(); ++row) {
            if (deletedMusicsPath.contains(list->item(row)->whatsThis())) {
                rows << row;
            }
        }
        // 移除
        for (int times = 0; times < rows.count(); ++times) {
            // 每移除一行后musicTable的内容就会发生变动,移除行之后的行会向前填补,
            // 那么rows中记录的行数就不能定位到要删除的行, 因此行数需要减去之前移除过的次数
            delete list->takeItem(rows.at(times) - times);
            // 播放列表同理
            playList->removeMedia(rows.at(times) - times);
        }
        setListItemStyle(list);
    }
}

void MusicPageArtist::playSelectedMusic()
{
    QListWidget * currentList = static_cast<QListWidget *>(m_stack->currentWidget());
    QMediaPlaylist * playList = m_playlistMap.value(m_musicListMap.key(currentList));
    QMediaPlayer *player = MusicPlayer::getPlayer();
    player->stop();
    player->setPlaylist(playList);
    playList->setCurrentIndex(currentList->currentRow());
    player->play();
}
