#include "leftsidebar.h"
#include <QVBoxLayout>
#include "splitline.h"
#include "musicplayer.h"

LeftSideBar::LeftSideBar(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    navListView = new NavListView(this);
    deviceInfo = new DeviceInfo(this);
    MusicPlayer *musicPlayer = new MusicPlayer(this);

    mainVBLayout->setContentsMargins(0,0,0,0);
    mainVBLayout->setMargin(0);
    mainVBLayout->setSpacing(0);

    mainVBLayout->addWidget(navListView, 1);
    mainVBLayout->addWidget(new SplitLine(SplitLine::Horizontal, this));
    mainVBLayout->addWidget(musicPlayer);
    mainVBLayout->addWidget(new SplitLine(SplitLine::Horizontal, this));
    mainVBLayout->addWidget(deviceInfo);

    this->setLayout(mainVBLayout);
    this->setFixedWidth(navListView->width());
    this->setContentsMargins(0, 0, 0, 0);
}

LeftSideBar::~LeftSideBar()
{
}
