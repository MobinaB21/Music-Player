#ifndef PLAYBACKLIST_H
#define PLAYBACKLIST_H
#include<QObject>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QUrl>
#include<vector>
#include"Song.h"
class PlaybackList :public QObject
{
private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    vector<Song>queue;
    int currentIndex=-1;
public:
    explicit PlaybackList(QObject *parent=nullptr);
    ~PlaybackList();
    void loadQueue(const vector<Song>&songs,int startIndex=0);
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    Song getCurrentSong() const;
    void playSong(const Song &song);
};

#endif // PLAYBACKLIST_H
