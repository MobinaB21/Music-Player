#include "songrepository.h"
#include<fstream>
#include<sstream>
vector<pair<int,int>>SongRepository::playlistSongs;
vector<pair<int,int>>SongRepository::likedSongs;
 vector<Song>SongRepository::favorite;
SongRepository::SongRepository() {
    loadFromFile();
    loadLikedSongsFromFile();
    loadPlaylistSongsFromFile();
}
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
Song SongRepository::getByName(string name)
{
    for(auto&s:songs)
    {
        if(s.getSongName()==name)return s;
    }
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
        file<<input.getSongName()<<"&"<<input.getReleaseYear()<<"&"<<input.getGenre()<<"&"<<input.getAudioFile()<<"&"<<input.getSongId()<<"&"<<input.getArtistId()<<"&"<<input.getAlbumId()<<"&"<<input.getFilePath()<<"\n";
    }
    file.close();
}
void SongRepository::loadFromFile()
{
    ifstream file("song.txt");
    if(file.is_open())
    {
        string line;
        while(getline(file,line))
        {
            if(line.empty())continue;
            stringstream ss(line);
            string songName,releaseYear,genre,audioFileName,songId,artistId,albumId,filePath;
            getline(ss,songName,'&');
            getline(ss,releaseYear,'&');
            getline(ss,genre,'&');
            getline(ss,audioFileName,'&');
            getline(ss,songId,'&');
            getline(ss,artistId,'&');
            getline(ss,albumId,'&');
            getline(ss,filePath,'&');
            Song temp(songName,stoi(releaseYear),genre,audioFileName,stoi(songId),stoi(artistId),stoi(albumId),filePath);
            songs.push_back(temp);
            if(stoi(songId)>=nextId)nextId=stoi(songId)+1;
        }
    }
    file.close();
}
void SongRepository::removeFromFile(Song& target)
{
    for(auto it=songs.begin();it!=songs.end();it++)
    {
        if(it->getSongName()==target.getSongName() && it->getSongId()==target.getSongId())
        {
            songs.erase(it);
            break;
        }
    }
    ofstream file("song.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    for(auto&s:songs)
    {
        saveToFile(s);
    }
    file.close();
}
void SongRepository::saveLikedSongsToFile()
{
    ofstream file("likedSongs.txt", ios::out | ios::trunc);
    if (file.is_open())
    {
        for (const auto& pair : likedSongs)
        {
            file << pair.first << "&" << pair.second << "\n";
        }
    }
    file.close();
}
void SongRepository::removeLikedSongsFromFile(int listenerId)
{
    for(auto it=likedSongs.begin();it!=likedSongs.end();)
    {
        if(it->first==listenerId)
        {
            it=likedSongs.erase(it);
        }
        else it++;
    }
    ofstream file("likedSongs.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    saveLikedSongsToFile();
    file.close();
}
void SongRepository::loadLikedSongsFromFile()
{
    likedSongs.clear();
    ifstream file("likedSongs.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.empty()) continue;
            stringstream ss(line);
            string userIdStr, songIdStr;
            getline(ss, userIdStr, '&');
            getline(ss, songIdStr, '&');
            try
            {
                likedSongs.push_back({stoi(userIdStr), stoi(songIdStr)});
            } catch (...) {}
        }
    }
    file.close();
}
void SongRepository::savePlaylistSongsToFile()
{
    ofstream file("playlist_songs.txt", ios::out | ios::trunc);
    if (file.is_open())
    {
        for (const auto& pair : playlistSongs)
        {
            file << pair.first << "&" << pair.second << "\n";
        }
    }
    file.close();
}
void SongRepository::loadPlaylistSongsFromFile()
{
    playlistSongs.clear();
    ifstream file("playlist_songs.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.empty()) continue;
            stringstream ss(line);
            string playlistIdStr, songIdStr;
            getline(ss, playlistIdStr, '&');
            getline(ss, songIdStr, '&');
            try
            {
                playlistSongs.push_back({stoi(playlistIdStr), stoi(songIdStr)});
            } catch (...) {}
        }
    }
    file.close();
}
void SongRepository::removePlaylistSongsFromFile(int playlistId)
{
    for(auto it=playlistSongs.begin();it!=playlistSongs.end();)
    {
        if(it->first==playlistId)
        {
            it= playlistSongs.erase(it);
        }
        else it++;
    }
    ofstream file("playlist_songs.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    savePlaylistSongsToFile();
    file.close();
}
vector<Song> SongRepository::getAllSongs() {return songs;}
optional<Song> SongRepository::getSongByName(string name)
{
    for(auto&s:songs)
    {

        if(s.getSongName()==name)return s;
    }
    return nullopt;
}