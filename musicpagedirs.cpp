#include "musicpagedirs.h"
#include "musicpage.h"
#include "musicplayer.h"
#include "currentdevicemanager.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <ddialog.h>

DWIDGET_USE_NAMESPACE

MusicPageDirs::MusicPageDirs(QList<MusicPageMusicItem> *musicItems, QWidget *parent) : QWidget(parent)
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
    initStack();

    Device *currentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();
    connect(currentDevice, &Device::musicFilesDeleted, this, &MusicPageDirs::removeMusicItems);
    connect(m_backButton, &QPushButton::clicked, this, &MusicPageDirs::backToDirs);

    mainVBLayout->setMargin(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);
    mainVBLayout->addWidget(m_stack);
    this->setLayout(mainVBLayout);
}

void MusicPageDirs::initMap(QList<MusicPageMusicItem> *musicItems)
{
    foreach (MusicPageMusicItem musicItem, *musicItems) {
        QString dir = musicItem.parentDir();
        QString title = musicItem.title();
        QString filePath = musicItem.filePath();

        QListWidgetItem *listItem = new QListWidgetItem(title);
        listItem->setWhatsThis(filePath);
        QListWidget *list;
        QMediaPlaylist *playList;

        if (m_dirMap.contains(dir)) {
            list = m_dirMap.value(dir);
            playList  = m_playlistMap.value(dir);
        } else {
            list = new QListWidget(m_stack);
            playList = new QMediaPlaylist(this);
            m_dirMap.insert(dir, list);
            m_playlistMap.insert(dir, playList);
        }
        list->addItem(listItem);
        playList->addMedia(QUrl::fromLocalFile(filePath));
    }
}

void MusicPageDirs::initStack()
{
    QTableWidget *dirTable = new QTableWidget(m_stack);
    initDirTable(dirTable);
    connect(dirTable, &QTableWidget::cellDoubleClicked, this, &MusicPageDirs::changeToList);
    // 让dirTable作为第一个stack的控件
    m_stack->addWidget(dirTable);

    QList<QString> dirs = m_dirMap.keys();
    int row = 0;
    foreach (QString dir, dirs) {
        // 添加map中和dir这个key对应的listwidget到mstack
        QListWidget * list = m_dirMap.value(dir);
        list->setSelectionMode(QListWidget::ExtendedSelection);
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list, &QListWidget::customContextMenuRequested, this, &MusicPageDirs::showMusicContextMenu);
        connect(list, &QListWidget::itemDoubleClicked, this, &MusicPageDirs::playSelectedMusic);
        setListItemStyle(list, true);
        m_stack->addWidget(list);
        // 设置dirTable每一行的内容(文件夹名称和包含的音乐的数量)
        QTableWidgetItem *nameItem = new QTableWidgetItem;
        QTableWidgetItem *countItem = new QTableWidgetItem;
        nameItem->setText(dir);
        nameItem->setIcon(QIcon(":/images/dir.png"));
        countItem->setText(QString::number(m_dirMap.value(dir)->count()));
        dirTable->setItem(row, 0, nameItem);
        dirTable->setItem(row, 1, countItem);
        row++;
    }
    setTableRowBackgroud(dirTable);
}

void MusicPageDirs::initDirTable(QTableWidget *dirTable)
{
    dirTable->setIconSize(QSize(30, 30));
    dirTable->setRowCount(m_dirMap.count());
    dirTable->setColumnCount(2);
    dirTable->setSelectionBehavior(QTableWidget::SelectRows);
    dirTable->setShowGrid(false);
    dirTable->setWordWrap(false);
    dirTable->setEditTriggers(QTableWidget::NoEditTriggers);
    dirTable->setColumnWidth(0, 600);
    dirTable->setColumnWidth(1, 180);
    dirTable->setHorizontalHeaderLabels(QStringList() << "Name" << "Count");
    dirTable->verticalHeader()->setHidden(true);
    dirTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    dirTable->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MusicPageDirs::setTableRowBackgroud(QTableWidget *dirTable)
{
    for (int row = 0; row < dirTable->rowCount(); ++row) {
        for (int col = 0; col < dirTable->columnCount(); ++col) {
            QTableWidgetItem *tableItem = dirTable->item(row, col);
            if (row % 2 != 0) {
                tableItem->setBackgroundColor(QColor("#f5f6f7"));
            } else {
                tableItem->setBackgroundColor(QColor("#ffffff"));
            }
        }
    }
}

void MusicPageDirs::setListItemStyle(QListWidget *list, bool setItemHigh)
{
    for (int row = 0; row < list->count(); ++row) {
        QListWidgetItem *item = list->item(row);
        // 设置行高
        if (setItemHigh) {
            item->setSizeHint(QSize(0, 30));
        }
        //设置背景
        if (row % 2 != 0) {
            item->setBackgroundColor(QColor("#f5f6f7"));
        } else {
            item->setBackgroundColor(QColor("#ffffff"));
        }
    }
}

// 获取当前选择的项目并通过信号传递给MusicPage类去删除文件
void MusicPageDirs::removeSelectedMusics()
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
                qDebug() << "(MusicPageDirs::removeSelectedMusics) cancel delete selection.";
            }
        } else {
            qDebug() << "(MusicPageDirs::removeSelectedMusics) no selected music!";
        }
    }
}

// 收到文件删除的信号后更新ui
void MusicPageDirs::removeMusicItems(QStringList deletedMusicsPath)
{
    // 遍历每个列表中的每个项
    // index从1开始,是因为mstack第一个widget是dirTabel不是listwidget
    for (int i = 1; i < m_stack->count(); ++i) {
        QListWidget * list = static_cast<QListWidget *>(m_stack->widget(i));
        QMediaPlaylist * playList = m_playlistMap.value(m_dirMap.key(list));
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
        setListItemStyle(list);
    }
}

void MusicPageDirs::playSelectedMusic()
{
    QListWidget * currentList = static_cast<QListWidget *>(m_stack->currentWidget());
    QMediaPlaylist * playList = m_playlistMap.value(m_dirMap.key(currentList));
    QMediaPlayer *player = MusicPlayer::getPlayer();
    player->stop();
    player->setPlaylist(playList);
    playList->setCurrentIndex(currentList->currentRow());
    player->play();
}

void MusicPageDirs::changeToList(int row)
{
    // m_stack中的页面的index和dirTable中的行对应
    // 但m_stack中的第一个页面是dirTab, 因此row需要+1
    m_stack->setCurrentIndex(row + 1);
    m_backButton->show();
}

void MusicPageDirs::backToDirs()
{
    m_stack->setCurrentIndex(0);
    m_backButton->hide();
}

void MusicPageDirs::showMusicContextMenu()
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
