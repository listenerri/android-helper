#include "musicpagealbum.h"
#include "musicplayer.h"
#include "musicpage.h"
#include "currentdevicemanager.h"
#include "tools/taglibtool.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QListWidget>
#include <ddialog.h>
#include <QMenu>

DWIDGET_USE_NAMESPACE

MusicPageAlbum::MusicPageAlbum(QList<MusicPageMusicItem> *musicItems, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    ToolsBar *toolsBar = MusicPage::getToolsBar();
    m_backButton = new QPushButton(this);
    m_backButton->setIcon(QIcon(":/images/back.png"));
    m_backButton->hide();
    toolsBar->addLeftWidget(m_backButton);

    m_stack = new QStackedWidget(this);
    m_stack->setContentsMargins(0, 0, 0, 0);

    initMap(musicItems);
    initUI();

    Device *currentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();
    connect(currentDevice, &Device::musicFilesDeleted, this, &MusicPageAlbum::removeMusicItems);
    connect(m_backButton, &QPushButton::clicked, this, &MusicPageAlbum::backToAlbumList);

    mainVBLayout->setMargin(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);
    mainVBLayout->addWidget(m_stack);
    this->setLayout(mainVBLayout);
}

void MusicPageAlbum::initMap(QList<MusicPageMusicItem> *musicItems)
{
    foreach (MusicPageMusicItem musicItem, *musicItems) {
        QString album = musicItem.album();
        QString title = musicItem.title();
        QString filePath = musicItem.filePath();

        QListWidgetItem *listItem = new QListWidgetItem(title);
        // 设置行高
        listItem->setSizeHint(QSize(0, 30));
        listItem->setWhatsThis(filePath);
        QListWidget *list;
        QMediaPlaylist *playList;

        // 如果歌曲中没有专辑信息(长度为零)将其设置为"unknown"
        if (album.count() == 0) {
            album = "unknown";
        }
        if (m_albumMap.contains(album)) {
            list = m_albumMap.value(album);
            playList  = m_playlistMap.value(album);
        } else {
            list = new QListWidget(m_stack);
            playList = new QMediaPlaylist(this);
            m_albumMap.insert(album, list);
            m_playlistMap.insert(album, playList);
        }
        list->addItem(listItem);
        playList->addMedia(QUrl::fromLocalFile(filePath));
    }
}

void MusicPageAlbum::initUI()
{
    QListWidget *albumList = new QListWidget(m_stack);
    albumList->setViewMode(QListWidget::IconMode);
    albumList->setIconSize(DefaultIconSize);
    albumList->setAcceptDrops(false);
    connect(albumList, &QListWidget::itemDoubleClicked, this, &MusicPageAlbum::changeToList);
    // 让albumList作为第一个stack的控件
    m_stack->addWidget(albumList);

    QPixmap defaultAlbumPix(":/images/album.svg");
    QIcon defaultAlbumIcon(defaultAlbumPix.scaled(DefaultIconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QList<QString> albums = m_albumMap.keys();
    foreach (QString album, albums) {
        // 添加map中和album这个key对应的listwidget到mstack
        QListWidget * list = m_albumMap.value(album);
        list->setSelectionMode(QListWidget::ExtendedSelection);
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list, &QListWidget::customContextMenuRequested, this, &MusicPageAlbum::showMusicContextMenu);
        connect(list, &QListWidget::itemDoubleClicked, this, &MusicPageAlbum::playSelectedMusic);
        setListItemBackground(list);
        m_stack->addWidget(list);
        // 添加album到首页albumList
        QListWidgetItem *albumItem = new QListWidgetItem(album, albumList);
        albumItem->setSizeHint(QSize(150, 150));
        // 通过这个专辑的歌曲列表的第一个歌曲获取专辑封面
        QPixmap albumPix = TaglibTool::getAlbumPicture(list->item(0)->whatsThis());
        if (!albumPix.isNull()) {
            // 缩放下图片, 否则占用内存太大
            QIcon albumIcon(albumPix.scaled(DefaultIconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            albumItem->setIcon(albumIcon);
        } else {
            albumItem->setIcon(defaultAlbumIcon);
        }
        albumList->addItem(albumItem);
    }
}

void MusicPageAlbum::setListItemBackground(QListWidget *list)
{
    for (int row = 0; row < list->count(); ++row) {
        QListWidgetItem *item = list->item(row);
        if (row % 2 != 0) {
            item->setBackgroundColor(QColor("#f5f6f7"));
        } else {
            item->setBackgroundColor(QColor("#ffffff"));
        }
    }
}

// 获取当前选择的项目并通过信号传递给MusicPage类去删除文件
void MusicPageAlbum::removeSelectedMusics()
{
    if (m_stack->currentIndex() != 0) {
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
                qDebug() << "(MusicPageAlbum::removeSelectedMusics) cancel delete selection.";
            }
        } else {
            qDebug() << "(MusicPageAlbum::removeSelectedMusics) no selected music!";
        }
    }}

// 收到文件删除的信号后更新ui
void MusicPageAlbum::removeMusicItems(QStringList deletedMusicsPath)
{
    // 遍历每个列表中的每个项
    // index从1开始,是因为mstack第一个widget是albumList其不包含音乐
    for (int i = 1; i < m_stack->count(); ++i) {
        QListWidget * list = static_cast<QListWidget *>(m_stack->widget(i));
        QMediaPlaylist * playList = m_playlistMap.value(m_albumMap.key(list));
        // 获取要移除的行
        QList<int> rows;
        for (int row = 0; row < list->count(); ++row) {
            if (deletedMusicsPath.contains(list->item(row)->whatsThis())) {
                rows << row;
            }
        }
        // 移除
        for (int times = 0; times < rows.count(); ++times) {
            // 每移除一行后list的内容就会发生变动,移除行之后的行会向前填补,
            // 那么rows中记录的行数就不能定位到要删除的行, 因此行数需要减去之前移除过的次数
            delete list->takeItem(rows.at(times) - times);
            // 播放列表同理
            playList->removeMedia(rows.at(times) - times);
        }
        setListItemBackground(list);
    }
}

void MusicPageAlbum::playSelectedMusic()
{
    QListWidget * currentList = static_cast<QListWidget *>(m_stack->currentWidget());
    QMediaPlaylist * playList = m_playlistMap.value(m_albumMap.key(currentList));
    QMediaPlayer *player = MusicPlayer::getPlayer();
    player->stop();
    player->setPlaylist(playList);
    playList->setCurrentIndex(currentList->currentRow());
    player->play();
}

void MusicPageAlbum::changeToList(QListWidgetItem *item)
{
    QListWidget * albumList = static_cast<QListWidget *>(m_stack->widget(0));
    int row = albumList->row(item);
    // m_stack中的页面的index和albumList中的项对应
    // 但m_stack中的第一个页面是albumList, 因此row需要+1
    m_stack->setCurrentIndex(row + 1);
    m_backButton->show();
}

void MusicPageAlbum::backToAlbumList()
{
    m_stack->setCurrentIndex(0);
    m_backButton->hide();
}

void MusicPageAlbum::showMusicContextMenu()
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
