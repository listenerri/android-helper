#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QLabel>
#include <QPushButton>
#include <dslider.h>

DWIDGET_USE_NAMESPACE

class MusicPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlayer(QWidget *parent = nullptr);

    static QMediaPlayer *getPlayer();
    static void stopAndClear();

signals:

private slots:
    void playPause();
    void stopPlay();
    void playNext();
    void playPre();
    void onPlayStateChanged(QMediaPlayer::State state);
    void setMusicName(QMediaPlayer::MediaStatus status);
    void setDurationLabel(qint64 duration);
    void setPositionLabel(qint64 position);
    void onPositionSliderReleased();
    void onPositionSliderPressed();
    void onVolumeChanged(int volume);

private:
    void resetMusicInfo();

private:
    QMediaPlayer *player;
    QLabel *nameLabel;
    QLabel *volumeLabel;
    QLabel *durationLabel;
    QLabel *positionLabel;
    DSlider *positionSlider;
    QPushButton *playButton;
};

#endif // MUSICPLAYER_H
