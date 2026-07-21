#include "playbacklist.h"

PlaybackList::PlaybackList(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.8);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            next();
        }
    });
}
PlaybackList::~PlaybackList() {}

void PlaybackList::loadQueue(const std::vector<Song> &songs, int startIndex)
{
    queue = songs;
    if (!queue.empty() && startIndex >= 0 && startIndex < static_cast<int>(queue.size())) {
        currentIndex = startIndex;
        QString filePath = QString::fromStdString(queue[currentIndex].getFilePath());
        player->setSource(QUrl::fromLocalFile(filePath));
        player->play();
    }
}
void PlaybackList::play()
{
    if (currentIndex != -1 && !queue.empty()) {
        player->play();
    }
}
void PlaybackList::pause()
{
    player->pause();
}
void PlaybackList::stop()
{
    player->stop();
}
void PlaybackList::next()
{
    if (currentIndex + 1 < static_cast<int>(queue.size())) {
        currentIndex++;
        QString filePath = QString::fromStdString(queue[currentIndex].getFilePath());
        player->setSource(QUrl::fromLocalFile(filePath));
        player->play();
    }
}
void PlaybackList::previous()
{
    if (currentIndex > 0) {
        currentIndex--;
        QString filePath = QString::fromStdString(queue[currentIndex].getFilePath());
        player->setSource(QUrl::fromLocalFile(filePath));
        player->play();
    }
}
Song PlaybackList::getCurrentSong() const
{
    if (currentIndex >= 0 && currentIndex < static_cast<int>(queue.size())) {
        return queue[currentIndex];
    }
    return Song();
}
void PlaybackList::playSong(const Song &song)
{
    queue.clear();
    queue.push_back(song);
    currentIndex=0;
    QString filePath=QString::fromStdString(song.getFilePath());
    player->setSource(QUrl::fromLocalFile(filePath));
    player->play();
}