#include "playlist.h"

Playlist::Playlist(string name,int listenerId,int playListId)
{
    this->name=name;
    this->listenerId=listenerId;
    this->playListId=playListId;
}
string Playlist::getName()const { return name;}
int Playlist::getListenerId()const{return listenerId;}
int Playlist::getPlayListId()const{return playListId;}
void Playlist::setListenerId(const int &listenerId){this->listenerId=listenerId;}
void Playlist::setPlayListId(const int &playListId){this->playListId=playListId;}
