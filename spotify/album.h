#ifndef ALBUM_H
#define ALBUM_H
#include<string>
using namespace std;
class Album
{
private:
    string albumName;
    int albumId;
    int artistId;
    string cover;
public:
    Album(string albumName,int albumId,int artistId);
    string getAlbumName()const;
    int getAlbumId()const;
    int getArtistId()const;
    string getCover()const;
    void setCover(const string &cover);
    void setAlbumName(string name);
    void setAlbumId(int id);
};

#endif // ALBUM_H
