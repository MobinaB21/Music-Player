#ifndef PLAYLIST_H
#define PLAYLIST_H
#include<string>
using namespace std;
class Playlist
{
private:
    std::string name;
    int listenerId;
    int playListId;
public:
    Playlist(string name,int listenerId,int playListId );
    string getName()const;
    int getListenerId()const;
    int getPlayListId()const;
    void setListenerId(const int &id);
    void setPlayListId(const int &playListId);
};

#endif // PLAYLIST_H
