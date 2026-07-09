#include "song.h"

Song::Song(string songName,int releaseYear,string genre,string audioFileName,int songId,int artistId,int albumId)
{
    this->songName=songName;
    this->releaseYear=releaseYear;
    this->genre=genre;
    this->audioFileName=audioFileName;
    this->songId=songId;
    this->artistId=artistId;
    this->albumId=albumId;
}
string Song::getSongName()const{return songName;}
int Song::getReleaseYear()const{return releaseYear;}
string Song::getGenre()const{return genre;}
string Song::getAudioFile()const{return audioFileName;}
int Song::getSongId()const{return songId;}
int Song::getArtistId()const{return artistId;}
int Song::getAlbumId()const{return albumId;}
string Song::getSongImage()const{return songImage;}

void Song::setSongImage(const string &songImage){this->songImage=songImage;}