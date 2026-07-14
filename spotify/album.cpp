#include "album.h"

Album::Album(string albumName,int albumId,int artistId)
{
    this->albumName=albumName;
    this->albumId=albumId;
    this->artistId=artistId;
}
string Album::getAlbumName()const{return albumName;}
int Album::getAlbumId()const{ return albumId;}
int Album::getArtistId()const{return artistId;}
string Album::getCover()const{return cover;}
void Album::setCover(const string &cover){this->cover=cover;}
void Album::setAlbumName(string name){albumName=name;}
