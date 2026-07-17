#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H
#include"playlist.h"
#include"abstractrepository.h"
#include<vector>
class PlaylistRepository :public AbstractRepository<Playlist>
{
private:
    vector<Playlist>list;
    vector<pair<int,int>>playlistSongs;
    int nextId=1;
public:
    PlaylistRepository();
    int save(const Playlist &input)override;
    bool remove(int id)override;
    optional<Playlist>search(int id)override;
    void insertSong(int playlistId,int songId);
    void removeSong(int playlistId,int songId);
    vector<Playlist> playlists(int listenerId);
    void saveToFile(Playlist&input);
    void loadFromFile();
    void removeFromFile(Playlist&target);
    int getIdByName(string playlistName);

};

#endif // PLAYLISTREPOSITORY_H
