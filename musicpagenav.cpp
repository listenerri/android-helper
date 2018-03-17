#include "musicpagenav.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

MusicPageNav::MusicPageNav(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainHBlayout = new QHBoxLayout(this);
    QPushButton *artistButton = new QPushButton("Artist", this);
    QPushButton *albumButton = new QPushButton("Album", this);
    QPushButton *musicsButton = new QPushButton("Musics", this);
    QPushButton *dirsButton = new QPushButton("Dirs", this);

    mainHBlayout->addWidget(artistButton);
    mainHBlayout->addWidget(albumButton);
    mainHBlayout->addWidget(musicsButton);
    mainHBlayout->addWidget(dirsButton);
    mainHBlayout->setSpacing(0);
    this->setLayout(mainHBlayout);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(artistButton, 0);
    m_buttonGroup->addButton(albumButton, 1);
    m_buttonGroup->addButton(musicsButton, 2);
    m_buttonGroup->addButton(dirsButton, 3);
    connect(m_buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),\
            this, &MusicPageNav::onButtonClicked);
}

void MusicPageNav::onButtonClicked(int id)
{
    emit currentIndexChanged(id);
}
