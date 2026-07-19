#ifndef SONGREPOSITORY_H
#define SONGREPOSITORY_H
#include "abstractrepository.h"
#include "song.h"
#include<vector>
class SongRepository:public AbstractRepository<Song>
{
private:
    vector<Song>songs;
    int nextId=1;
public:
    static vector<pair<int,int>>playlistSongs;
    static vector<pair<int,int>>likedSongs;
    static vector<Song>favorite;
    SongRepository();
    int save(const Song&input)override;
    bool remove(int id)override;
    optional<Song>search(int id)override;
    vector<Song> singleSong(int id);
    vector<Song> getByAlbum(int id);
    vector<Song> getByArtist(int id);
    vector<Song> getByPlaylist(int id);
    vector<Song> getByLikedSongs(int id);
    vector<Song>getAllSongs();
    Song getByName(string name);
    void saveToFile(const Song&input);
    void loadFromFile();
    void removeFromFile(Song& target);
    static void saveLikedSongsToFile();
    static void loadLikedSongsFromFile();
    static void savePlaylistSongsToFile();
    static void loadPlaylistSongsFromFile();
    void removeLikedSongsFromFile(int listenerId);
    void removePlaylistSongsFromFile(int playlistId);
    optional<Song> getSongByName(string name);
};

#endif // SONGREPOSITORY_H
