#include "musicpage.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>
#include "currentdevicemanager.h"
#include "splitline.h"
#include "musicpagenav.h"
#include "tools/taglibtool.h"

MusicPage::MusicPage(QWidget *parent) : QWidget(parent)
{
    initMusicItems();

    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);

    toolsBar = getToolsBar();
    MusicPageNav *nav = new MusicPageNav;
    toolsBar->addCenterWidget(nav);

    QStackedWidget *stackPages = new QStackedWidget(this);
    m_artistPage = new MusicPageArtist(&m_musicItems);
    m_albumPage = new MusicPageAlbum(&m_musicItems);
    m_musicsPage = new MusicPageMusics(&m_musicItems);
    m_dirsPage = new MusicPageDirs(&m_musicItems);
    stackPages->addWidget(m_artistPage);
    stackPages->addWidget(m_albumPage);
    stackPages->addWidget(m_musicsPage);
    stackPages->addWidget(m_dirsPage);
    stackPages->setCurrentIndex(2);

    connect(nav, &MusicPageNav::currentIndexChanged, stackPages, &QStackedWidget::setCurrentIndex);
    connect(stackPages, &QStackedWidget::currentChanged, this, &MusicPage::resetPagesConnect);
    connect(CurrentDevice, &Device::musicFilesDeleted, this, &MusicPage::removeMusicItems);

    // musicPage为默认page所以现在只连接删除按钮到musicPage, 其他页面在resetPagesConnect槽中处理
    connect(toolsBar, &ToolsBar::deleteButtonClicked, m_musicsPage, &MusicPageMusics::removeSelectedMusics);

    mainVBLayout->setSpacing(0);
    mainVBLayout->setContentsMargins(0, 0, 0, 0);

    mainVBLayout->addWidget(toolsBar);
    mainVBLayout->addWidget(new SplitLine(SplitLine::Horizontal, this));
    mainVBLayout->addWidget(stackPages);

    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainVBLayout);
}

ToolsBar *MusicPage::getToolsBar()
{
    static ToolsBar *toolsBar = new ToolsBar;
    return toolsBar;
}

void MusicPage::initMusicItems()
{
    CurrentDevice = CurrentDeviceManager::getInstance()->getCurrentDevice();

    // 获取音频文件的信息保存起来传递给子控件使用
    foreach (auto musicFile, CurrentDevice->getMusicFiles()) {
        QString musicFileName = musicFile.completeBaseName();
        QString musicFilePath = musicFile.absoluteFilePath();
        QString musicFileParentDir = musicFile.absoluteDir().dirName();
        MusicPageMusicItem musicItem(musicFilePath, musicFileParentDir);
        QStringList musicMetaData = TaglibTool::getMetaData(musicFilePath);
        // 如果获取音频文件的元数据信息失败, 将歌曲名设置为文件名
        if (musicMetaData.count() == 0) {
            qDebug() << "(MusicPage::initMusicItems) get meta data failed for:" << musicFilePath;
            musicItem.setTitle(musicFileName);
            m_musicItems << musicItem;
        } else {
            // 有些音频文件的元数据信息中没有歌曲名, 将其设置为文件名
            if (musicMetaData[0].length() == 0) {
                musicMetaData[0] = musicFileName;
            }
            // 有些音频文件的元数据信息中没有年份(值为"0"), 将其设置为空
            if (musicMetaData[3] == "0") {
                musicMetaData[3] = "";
            }
            musicItem.setTitle(musicMetaData[0]);
            musicItem.setArtist(musicMetaData[1]);
            musicItem.setAlbum((musicMetaData[2]));
            musicItem.setYear(musicMetaData[3]);
            musicItem.setTime(musicMetaData[4]);
            m_musicItems << musicItem;
        }
    }
}

// 确保只有当前page与toolsbar中的删除按钮进行信号槽连接
void MusicPage::resetPagesConnect(int currentPageIndex)
{
    disconnect(toolsBar, &ToolsBar::deleteButtonClicked, 0, 0);

    switch (currentPageIndex) {
    case 0:
        connect(toolsBar, &ToolsBar::deleteButtonClicked, m_artistPage, &MusicPageArtist::removeSelectedMusics);
        break;
    case 1:
        connect(toolsBar, &ToolsBar::deleteButtonClicked, m_albumPage, &MusicPageAlbum::removeSelectedMusics);
        break;
    case 2:
        connect(toolsBar, &ToolsBar::deleteButtonClicked, m_musicsPage, &MusicPageMusics::removeSelectedMusics);
        break;
    case 3:
        connect(toolsBar, &ToolsBar::deleteButtonClicked, m_dirsPage, &MusicPageDirs::removeSelectedMusics);
        break;
    default:
        break;
    }
}

// 从维护的musicitems中移除已删除的music
void MusicPage::removeMusicItems(QStringList deletedMusicsPath)
{
    foreach (QString path, deletedMusicsPath) {
        m_musicItems.removeAll(MusicPageMusicItem(path));
    }
}
