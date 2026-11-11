#pragma once

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

class QVideoWidget;
class QPushButton;
class QSlider;
class QLabel;

class MediaPlayer : public QWidget {
    Q_OBJECT

public:
    explicit MediaPlayer(QWidget *parent = nullptr);

private slots:
    void openLocalFile();
    void openUrl();
    void togglePlayback();
    void stopPlayback();
    void seekForward();
    void seekBackward();

    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void setPosition(int position);
    void setVolume(int volume);
    void updateState(QMediaPlayer::PlaybackState state);

private:
    QString formatTime(qint64 ms);

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QVideoWidget *videoWidget;

    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *backwardButton;
    QPushButton *openFileButton;
    QPushButton *openUrlButton;

    QSlider *progressSlider;
    QSlider *volumeSlider;
    QLabel *timeLabel;

    qint64 currentDuration = 0;
};
