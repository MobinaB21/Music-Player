#ifndef PLAYLIST_H
#define PLAYLIST_H
#include<string>
using namespace std;
class Playlist
{
private:
    std::string name;
    int listenerId;
    int playlistId;
public:
    Playlist(string name,int listenerId,int playlistId );
    string getName()const;
    int getListenerId()const;
    int getPlaylistId()const;
    void setListenerId(const int &id);
    void setPlaylistId(const int &playListId);
};

#endif // PLAYLIST_H
