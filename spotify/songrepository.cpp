#include "songrepository.h"
#include<fstream>
#include<sstream>
vector<pair<int,int>>SongRepository::playlistSongs;
vector<pair<int,int>>SongRepository::likedSongs;
SongRepository::SongRepository() {loadFromFile();}
int SongRepository::save(const Song&input)
{
    Song temp=input;
    if(temp.getSongId()==0)
    {
        temp.setSongId(nextId++);
        songs.push_back(temp);
        saveToFile(temp);
        return temp.getSongId();
    }
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
void SongRepository::saveToFile(const Song&input)
{
    ofstream file("song.txt",ios::app);
    if(file.is_open())
    {
        file<<input.getSongName()<<"&"<<input.getReleaseYear()<<"&"<<input.getGenre()<<"&"<<input.getAudioFile()<<"&"<<input.getSongId()<<"&"<<input.getArtistId()<<input.getAlbumId()<<"\n";
    }
    file.close();
}
void SongRepository::loadFromFile()
{
    ifstream file("artist.txt");
    if(file.is_open())
    {
        string line;
        while(getline(file,line))
        {
            if(line.empty())continue;
            stringstream ss(line);
            string songName,releaseYear,genre,audioFileName,songId,artistId,albumId;
            getline(ss,songName,'&');
            getline(ss,releaseYear,'&');
            getline(ss,genre,'&');
            getline(ss,audioFileName,'&');
            getline(ss,songId,'&');
            getline(ss,artistId,'&');
             getline(ss,albumId,'&');
            Song temp(songName,stoi(releaseYear),genre,audioFileName,stoi(songId),stoi(artistId),stoi(albumId));
            songs.push_back(temp);
            if(stoi(songId)>=nextId)nextId=stoi(songId)+1;
        }
    }
    file.close();
}