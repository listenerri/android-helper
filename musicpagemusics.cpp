#include "musicpagemusics.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QDebug>
#include <ddialog.h>
#include <QMenu>
#include "musicplayer.h"
#include "currentdevicemanager.h"

DWIDGET_USE_NAMESPACE

MusicPageMusics::MusicPageMusics(QList<MusicPageMusicItem> *musicItems, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    m_musicsTable = new QTableWidget(this);
    m_columnCount = 5;

    m_musicsTable->setRowCount(musicItems->count());
    m_musicsTable->setColumnCount(m_columnCount);
    m_musicsTable->setSelectionBehavior(QTableWidget::SelectRows);
    m_musicsTable->setShowGrid(false);
    m_musicsTable->setWordWrap(false);
    m_musicsTable->setEditTriggers(QTableWidget::NoEditTriggers);
    m_musicsTable->setColumnWidth(0, 200);
    m_musicsTable->setColumnWidth(1, 180);
    m_musicsTable->setColumnWidth(2, 180);
    m_musicsTable->setHorizontalHeaderLabels(QStringList() << "Name" << "Artist" << "Album" << "Year" << "Time");
    m_musicsTable->verticalHeader()->setHidden(true);
    m_musicsTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_musicsTable->setContextMenuPolicy(Qt::CustomContextMenu);

    initMusicTable(musicItems, m_columnCount);

    Device *currentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();
    connect(currentDevice, &Device::musicFilesDeleted, this, &MusicPageMusics::removeMusicItems);
    connect(m_musicsTable, &QTableWidget::cellDoubleClicked, this, &MusicPageMusics::playSelected);
    connect(m_musicsTable, &QTableWidget::customContextMenuRequested, this, &MusicPageMusics::showContextMenu);

    mainVBLayout->setContentsMargins(0, 0, 0, 0);
    mainVBLayout->addWidget(m_musicsTable);
    this->setLayout(mainVBLayout);
}

void MusicPageMusics::initMusicTable(QList<MusicPageMusicItem> *musicItems, int columnCount)
{
    int row = 0;
    foreach (MusicPageMusicItem musicItem, *musicItems) {
        QString path = musicItem.filePath();
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem *tableItem = new QTableWidgetItem;
            switch (column) {
            case 0:
                tableItem->setText(musicItem.title());
                break;
            case 1:
                tableItem->setText(musicItem.artist());
                break;
            case 2:
                tableItem->setText(musicItem.album());
                break;
            case 3:
                tableItem->setText(musicItem.year());
                break;
            case 4:
                tableItem->setText(musicItem.time());
                break;
            default:
                break;
            }
            tableItem->setWhatsThis(path);
            m_musicsTable->setItem(row, column, tableItem);
        }
        m_playList.addMedia(QUrl::fromLocalFile(path));
        row++;
    }
    setRowBackgroud();
}

void MusicPageMusics::setRowBackgroud()
{
    for (int row = 0; row < m_musicsTable->rowCount(); ++row) {
        for (int col = 0; col < m_columnCount; ++col) {
            QTableWidgetItem *tableItem = m_musicsTable->item(row, col);
            if (row % 2 != 0) {
                tableItem->setBackgroundColor(QColor("#f5f6f7"));
            } else {
                tableItem->setBackgroundColor(QColor("#ffffff"));
            }
        }
    }
}

void MusicPageMusics::showContextMenu()
{
    QMenu contextMenu(this);
    contextMenu.addAction("Play");
    contextMenu.addSeparator();
    contextMenu.addAction("Delete");
    QAction *selectedAction = contextMenu.exec(QCursor::pos());
    if (selectedAction != 0) {
        if (selectedAction->text() == "Play") {
            playSelected();
        } else if (selectedAction->text() == "Delete") {
            removeSelectedMusics();
        }
    }
}

// 获取当前选择的项目并通过信号传递给MusicPage类去删除文件
void MusicPageMusics::removeSelectedMusics()
{
    QList<QTableWidgetItem *> selectedMusicItems = m_musicsTable->selectedItems();
    QStringList selectedMusicsPath;
    QString selectedMusicsName;
    foreach (QTableWidgetItem *item, selectedMusicItems) {
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
            qDebug() << "(MusicPageMusics::removeSelectedMusics) cancel delete selection.";
        }
    } else {
        qDebug() << "(MusicPageMusics::removeSelectedMusics) no selected music!";
    }
}

// 收到文件删除的信号后更新ui
void MusicPageMusics::removeMusicItems(QStringList deletedMusicsPath)
{
    // 获取要移除的行
    QList<int> rows;
    for (int row = 0; row < m_musicsTable->rowCount(); ++row) {
        QTableWidgetItem *item = m_musicsTable->item(row, 0);
        if (deletedMusicsPath.contains(item->whatsThis())) {
            rows << row;
        }
    }
    // 移除
    for (int times = 0; times < rows.count(); ++times) {
        // 每移除一行后musicTable的内容就会发生变动,移除行之后的行会向前填补,
        // 那么rows中记录的行数就不能定位到要删除的行, 因此行数需要减去之前移除过的次数
        m_musicsTable->removeRow(rows[times] - times);
        // 播放列表同理
        m_playList.removeMedia(rows[times] - times);
    }
    setRowBackgroud();
}

void MusicPageMusics::playSelected()
{
    QMediaPlayer *player = MusicPlayer::getPlayer();
    player->stop();
    player->setPlaylist(&m_playList);
    m_playList.setCurrentIndex(m_musicsTable->currentRow());
    player->play();
}
