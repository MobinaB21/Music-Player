#include "playlistrepository.h"

PlaylistRepository::PlaylistRepository() {}
int PlaylistRepository::save(const Playlist&input)
{
    for(auto&p:list)
    {
        if(p.getPlayListId()==input.getPlayListId())
        {
            p=input;
            return input.getPlayListId();
        }
    }
    list.push_back(input);
    return input.getPlayListId();
}
bool PlaylistRepository::remove(int id)
{
    for(auto it=list.begin();it!=list.end();it++)
    {
        if(it->getPlayListId()==id)
        {
            list.erase(it);
            return true;
        }
    }
    return false;
}
optional<Playlist> PlaylistRepository::search(int id)
{
    for(auto&p:list)
    {
        if(p.getPlayListId()==id)return p;
    }
    return nullopt;
}
void PlaylistRepository::insertSong(int playlistId,int songId)
{
    for(auto &p: playlistSongs)
    {
        if(p.first==playlistId && p.second==songId)return;
    }
    playlistSongs.push_back({playlistId,songId});
}
void PlaylistRepository::removeSong(int playlistId,int songId)
{
    for(auto it=playlistSongs.begin();it!=playlistSongs.end();it++)
    {
        if(it->first==playlistId && it->second==songId)
        {
            playlistSongs.erase(it);
            return;
        }
    }
}
vector<Playlist> PlaylistRepository::playlists(int listenerId)
{
    vector<Playlist>result;
    for(auto &p:list)
    {
        if(p.getListenerId()==listenerId)
        {
            result.push_back(p);
        }
    }
    return result;
}