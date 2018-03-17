#include "musicplayer.h"
#include <QMediaPlaylist>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include "currentdevicemanager.h"

MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    QHBoxLayout *volumeLayout = new QHBoxLayout;
    QHBoxLayout *positionLayout = new QHBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    player = getPlayer();

    nameLabel = new QLabel(this);

    DSlider *volumeSlider = new DSlider(Qt::Horizontal, this);
    volumeSlider->setFixedWidth(130);
    volumeSlider->setMaximum(100);
    volumeLabel = new QLabel(this);
    volumeLabel->setFixedSize(15, 15);
    volumeLabel->setPixmap(QPixmap(":/images/volume.png"));
    volumeLabel->setScaledContents(true);
    QLabel *volumeNumLabel = new QLabel(this);

    positionSlider = new DSlider(Qt::Horizontal, this);
    positionSlider->setFixedWidth(130);
    durationLabel = new QLabel(this);
    positionLabel = new QLabel(this);

    resetMusicInfo();

    playButton = new QPushButton(this);
    QPushButton *stopButton = new QPushButton(this);
    QPushButton *nextButton = new QPushButton(this);
    QPushButton *preButton = new QPushButton(this);

    playButton->setIcon(QIcon(":/images/buttonPlay.png"));
    stopButton->setIcon(QIcon(":/images/buttonStop.png"));
    nextButton->setIcon(QIcon(":/images/buttonNext.png"));
    preButton->setIcon(QIcon(":/images/buttonPre.png"));

    connect(CurrentDeviceManager::getInstance(), &CurrentDeviceManager::deviceUnMounted, this, &MusicPlayer::stopPlay);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::setMusicName);

    connect(volumeSlider, &DSlider::valueChanged, player, &QMediaPlayer::setVolume);
    connect(player, &QMediaPlayer::volumeChanged, volumeNumLabel, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));
    connect(player, &QMediaPlayer::volumeChanged, this, &MusicPlayer::onVolumeChanged);

    connect(positionSlider, &DSlider::sliderPressed, this, &MusicPlayer::onPositionSliderPressed);
    connect(positionSlider, &DSlider::sliderReleased, this, &MusicPlayer::onPositionSliderReleased);
    connect(player, &QMediaPlayer::durationChanged, this, &MusicPlayer::setDurationLabel);
    connect(player, &QMediaPlayer::positionChanged, this, &MusicPlayer::setPositionLabel);
    connect(player, &QMediaPlayer::durationChanged, positionSlider, &DSlider::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, positionSlider, &DSlider::setValue);

    connect(player, &QMediaPlayer::stateChanged, this, &MusicPlayer::onPlayStateChanged);
    connect(playButton, &QPushButton::clicked, this, &MusicPlayer::playPause);
    connect(stopButton, &QPushButton::clicked, this, &MusicPlayer::stopPlay);
    connect(nextButton, &QPushButton::clicked, this, &MusicPlayer::playNext);
    connect(preButton, &QPushButton::clicked, this, &MusicPlayer::playPre);

    volumeLayout->setContentsMargins(0, 0, 0, 0);
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeNumLabel);

    positionLayout->setContentsMargins(0, 0, 0, 0);
    positionLayout->addWidget(positionSlider);
    positionLayout->addWidget(positionLabel);
    positionLayout->addWidget(durationLabel);

    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(preButton);
    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(nextButton);
    buttonsLayout->addStretch(1);

    mainVBLayout->addWidget(nameLabel);
    mainVBLayout->addLayout(volumeLayout);
    mainVBLayout->addLayout(positionLayout);
    mainVBLayout->addLayout(buttonsLayout);

    this->setLayout(mainVBLayout);

    // 设置一个空的播放列表,否则当第一次初始化MusicPlayer控件后点击上一曲或下一曲程序将会崩溃
    player->setPlaylist(new QMediaPlaylist(this));
    player->setVolume(50);
    volumeSlider->setValue(50);
}

QMediaPlayer *MusicPlayer::getPlayer()
{
    static QMediaPlayer s_player;
    return &s_player;
}

// 卸载设备前需要先清空playlist, 否则会因为资源占用导致卸载设备失败
void MusicPlayer::stopAndClear()
{
    getPlayer()->stop();
    getPlayer()->playlist()->clear();
}

void MusicPlayer::playPause()
{
    switch (player->state()) {
    case QMediaPlayer::PlayingState:
        player->pause();
        break;
    case QMediaPlayer::PausedState:
        player->play();
        break;
    default:
        break;
    }
}

void MusicPlayer::stopPlay()
{
    if (player->state() != QMediaPlayer::StoppedState) {
        player->stop();
    }
}

void MusicPlayer::playNext()
{
    player->pause();
    QMediaPlaylist *playList = player->playlist();
    playList->next();
    player->play();
}

void MusicPlayer::playPre()
{
    player->pause();
    QMediaPlaylist *playList = player->playlist();
    playList->previous();
    player->play();
}

void MusicPlayer::onPlayStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(QIcon(":/images/buttonPause.png"));
        break;
    case QMediaPlayer::PausedState:
        playButton->setIcon(QIcon(":/images/buttonPlay.png"));
        break;
    case QMediaPlayer::StoppedState:
        playButton->setIcon(QIcon(":/images/buttonPlay.png"));
        resetMusicInfo();
        break;
    default:
        break;
    }
}

void MusicPlayer::setMusicName(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::BufferedMedia) {
        QString title = player->metaData("Title").toString();
        if (title.isEmpty()) {
            qDebug() << "(MusicPalyer::setMusicName) get title metadata failed, set title to file name!";
            title = QFileInfo(player->currentMedia().canonicalUrl().fileName()).completeBaseName();
        }
        nameLabel->setText(title);
        qDebug() << "(MusicPalyer::setMusicName) playing music:" << title;
    }
}

void MusicPlayer::setDurationLabel(qint64 duration)
{
    if (duration != 0) {
        positionSlider->setMaximum(duration);
        int durationS = duration / 1000;
        int seconds = durationS % 60;
        int minutes = (durationS - seconds) / 60;
        durationLabel->setText(QString::number(minutes) + ":" + QString::number(seconds));
    }
}

void MusicPlayer::setPositionLabel(qint64 position)
{
    if (position != 0) {
        int positionS = position / 1000;
        int seconds = positionS % 60;
        int minutes = (positionS - seconds) / 60;
        positionLabel->setText(QString::number(minutes) + ":" + QString::number(seconds));
    }
}

void MusicPlayer::onPositionSliderReleased()
{
    connect(player, &QMediaPlayer::positionChanged, positionSlider, &DSlider::setValue);
    player->setPosition(positionSlider->value());
}

void MusicPlayer::onPositionSliderPressed()
{
    disconnect(player, &QMediaPlayer::positionChanged, positionSlider, &DSlider::setValue);
}

void MusicPlayer::onVolumeChanged(int volume)
{
    if (volume == 0) {
        volumeLabel->setPixmap(QPixmap(":/images/volumeMute.png"));
    } else {
        volumeLabel->setPixmap(QPixmap(":/images/volume.png"));
    }
}

void MusicPlayer::resetMusicInfo()
{
    nameLabel->setText("waiting for play...");
    durationLabel->setText("0:0");
    positionLabel->setText("0:0");
}
