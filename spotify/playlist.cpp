#include "playlist.h"

Playlist::Playlist(string name,int listenerId,int playlistId)
{
    this->name=name;
    this->listenerId=listenerId;
    this->playlistId=playlistId;
}
string Playlist::getName()const { return name;}
int Playlist::getListenerId()const{return listenerId;}
int Playlist::getPlaylistId()const{return playlistId;}
void Playlist::setListenerId(const int &listenerId){this->listenerId=listenerId;}
void Playlist::setPlaylistId(const int &playlistId){this->playlistId=playlistId;}
void Playlist::setPlaylistName(string&newName){this->name=newName;}

