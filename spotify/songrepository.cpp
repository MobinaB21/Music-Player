#include "songrepository.h"
vector<pair<int,int>>SongRepository::playlistSongs;
vector<pair<int,int>>SongRepository::likedSongs;
SongRepository::SongRepository() {}
int SongRepository::save(const Song&input)
{
    for(auto &s: songs)
    {
        if(s.getSongId()==input.getSongId())
        {
            s=input;
            return s.getSongId();
        }
    }
    songs.push_back(input);
    return input.getSongId();
}
bool SongRepository::remove(int id)
{
    for(auto it=songs.begin();it!=songs.end();it++)
    {
        if(it->getSongId()==id)
        {
            songs.erase(it);
            auto p = playlistSongs.begin();
            while (p != playlistSongs.end())
            {
                if (p->second == id)p = playlistSongs.erase(p);
                else p++;
            }
            auto l = likedSongs.begin();
            while (l != likedSongs.end())
            {
                if (l->second == id)l = likedSongs.erase(l);
                else l++;
            }
            return true;
        }
    }
    return false;
}
optional<Song> SongRepository::search(int id)
{
    for(const auto&s : songs)
    {
        if(s.getSongId()==id)return s;
    }
    return nullopt;
}
vector<Song> SongRepository::singleSong(int id)
{
    vector<Song>result;
    for(auto &s:songs)
    {
        if(s.getArtistId()==id && s.getAlbumId()==0)result.push_back(s);
    }
    return result;
}
vector<Song> SongRepository::getByAlbum(int id)
{
    vector<Song>result;
    for(auto &s:songs)
    {
        if(s.getAlbumId()==id)result.push_back(s);
    }
    return result;
}
vector<Song> SongRepository::getByArtist(int id)
{
    vector<Song>result;
    for(auto &s:songs)
    {
        if(s.getArtistId()==id)result.push_back(s);
    }
    return result;
}
vector<Song> SongRepository::getByPlaylist(int id)
{
    vector<Song>result;
    for(auto&p:playlistSongs)
    {
        if(p.first==id)
        {
            auto answer=search(p.second);
            if(answer.has_value())result.push_back(answer.value());
        }
    }
    return result;
}
vector<Song> SongRepository::getByLikedSongs(int id)
{
    vector<Song>result;
    for(auto&l:likedSongs)
    {
        if(l.first==id)
        {
            auto answer=search(l.second);
            if(answer.has_value())result.push_back(answer.value());
        }
    }
    return result;
}
