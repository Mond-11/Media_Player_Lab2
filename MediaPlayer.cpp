#include "MediaPlayer.h"
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QStyle>
#include <QTime>
#include <QApplication>

MediaPlayer::MediaPlayer(QWidget *parent) : QWidget(parent) {
    setWindowTitle("QT Mediaplayer");
    resize(800, 600);

    // Ініціалізація медіа-компонентів
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    videoWidget = new QVideoWidget(this);

    QPalette palette = videoWidget->palette();
    palette.setColor(QPalette::Window, Qt::black);
    videoWidget->setPalette(palette);
    videoWidget->setAutoFillBackground(true);

    player->setAudioOutput(audioOutput);
    player->setVideoOutput(videoWidget);

    // Елементи керування відтворенням
    playButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaPlay), "", this);
    stopButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaStop), "", this);
    backwardButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekBackward), "-10s", this);
    forwardButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekForward), "+10s", this);

    // Елементи відкриття
    openFileButton = new QPushButton("Відкрити файл", this);
    openUrlButton = new QPushButton("Відкрити URL", this);

    // Повзунки та час
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 0);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    audioOutput->setVolume(0.5); // Qt6 використовує лінійну гучність від 0.0 до 1.0

    timeLabel = new QLabel("00:00 / 00:00", this);

    // Layouts
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(openFileButton);
    controlLayout->addWidget(openUrlButton);
    controlLayout->addWidget(backwardButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(forwardButton);
    controlLayout->addWidget(timeLabel);
    
    QHBoxLayout *volumeLayout = new QHBoxLayout;
    volumeLayout->addWidget(new QLabel("Гучність:", this));
    volumeLayout->addWidget(volumeSlider);
    controlLayout->addLayout(volumeLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(videoWidget, 1); // 1 = stretch, щоб відео займало весь простір
    mainLayout->addWidget(progressSlider);
    mainLayout->addLayout(controlLayout);

    // Events
    connect(openFileButton, &QPushButton::clicked, this, &MediaPlayer::openLocalFile);
    connect(openUrlButton, &QPushButton::clicked, this, &MediaPlayer::openUrl);
    connect(playButton, &QPushButton::clicked, this, &MediaPlayer::togglePlayback);
    connect(stopButton, &QPushButton::clicked, this, &MediaPlayer::stopPlayback);
    connect(backwardButton, &QPushButton::clicked, this, &MediaPlayer::seekBackward);
    connect(forwardButton, &QPushButton::clicked, this, &MediaPlayer::seekForward);

    connect(player, &QMediaPlayer::positionChanged, this, &MediaPlayer::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MediaPlayer::onDurationChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MediaPlayer::updateState);

    connect(progressSlider, &QSlider::sliderMoved, this, &MediaPlayer::setPosition);
    connect(volumeSlider, &QSlider::valueChanged, this, &MediaPlayer::setVolume);
}

void MediaPlayer::openLocalFile() {
    QUrl fileUrl = QFileDialog::getOpenFileUrl(this, "Оберіть медіафайл", QUrl(), 
        "Media Files (*.mp4 *.avi *.mkv *.mp3 *.wav *.ogg)");
    if (!fileUrl.isEmpty()) {
        player->setSource(fileUrl);
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        player->play();
    }
}

void MediaPlayer::openUrl() {
    bool ok;
    QString urlStr = QInputDialog::getText(this, "Відкрити URL", "Введіть посилання на медіа (напр., http://.../video.mp4):", QLineEdit::Normal, "", &ok);
    if (ok && !urlStr.isEmpty()) {
        player->setSource(QUrl(urlStr));
        player->play();
    }
}

void MediaPlayer::togglePlayback() {
    if (player->playbackState() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void MediaPlayer::stopPlayback() {
    player->stop();
}

void MediaPlayer::seekForward() {
    player->setPosition(qMin(player->position() + 10000, currentDuration));
}

void MediaPlayer::seekBackward() {
    player->setPosition(qMax(player->position() - 10000, 0LL));
}

void MediaPlayer::onPositionChanged(qint64 position) {
    if (!progressSlider->isSliderDown()) {
        progressSlider->setValue(position);
    }
    timeLabel->setText(formatTime(position) + " / " + formatTime(currentDuration));
}

void MediaPlayer::onDurationChanged(qint64 duration) {
    currentDuration = duration;
    progressSlider->setRange(0, duration);
}

void MediaPlayer::setPosition(int position) {
    player->setPosition(position);
}

void MediaPlayer::setVolume(int volume) {
    audioOutput->setVolume(volume / 100.0);
}

void MediaPlayer::updateState(QMediaPlayer::PlaybackState state) {
    if (state == QMediaPlayer::PlayingState) {
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

QString MediaPlayer::formatTime(qint64 ms) {
    int seconds = (ms / 1000) % 60;
    int minutes = (ms / 60000) % 60;
    int hours = (ms / 3600000) % 24;

    QTime time(hours, minutes, seconds);
    return hours > 0 ? time.toString("hh:mm:ss") : time.toString("mm:ss");
}