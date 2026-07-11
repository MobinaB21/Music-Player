#ifndef SONG_H
#define SONG_H
#include<string>
using namespace std;
class Song
{
private:
    string songName;
    int releaseYear;
    string genre;
    string audioFileName;
    int songId;
    int artistId;
    int albumId;
    string songImage;
public:
    Song(string songName,int releaseYear,string genre,string audioFileName,int songId,int artistId,int albumId);

    string getSongName()const;
    int getReleaseYear()const;
    string getGenre()const;
    string getAudioFile()const;
    int getSongId()const;
    int getArtistId()const;
    int getAlbumId()const;
    string getSongImage()const;
    void setSongId(int id);
    void setSongImage(const string &songImage);
};

#endif // SONG_H
