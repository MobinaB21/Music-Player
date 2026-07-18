#include "playlistrepository.h"
#include<fstream>
#include<sstream>
PlaylistRepository::PlaylistRepository() {loadFromFile();}
int PlaylistRepository::save(const Playlist&input)
{
    Playlist temp=input;
    if(temp.getPlaylistId()==0)
    {
        temp.setPlaylistId(nextId++);
        list.push_back(temp);
        saveToFile(temp);
        return temp.getPlaylistId();
    }
    for(auto &s: list)
    {
        if(s.getPlaylistId()==input.getPlaylistId())
        {
            s=input;
            return s.getPlaylistId();
        }
    }
    list.push_back(input);
    return input.getPlaylistId();
}
bool PlaylistRepository::remove(int id)
{
    for(auto it=list.begin();it!=list.end();it++)
    {
        if(it->getPlaylistId()==id)
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
        if(p.getPlaylistId()==id)return p;
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
void PlaylistRepository::saveToFile(Playlist&input)
{
    ofstream file("playlist.txt",ios::out);
    if(file.is_open())
    {
        file<<input.getName()<<"&"<<input.getListenerId()<<"&"<<input.getPlaylistId()<<"\n";
    }
    file.close();
}
void PlaylistRepository::loadFromFile()
{
    ifstream file("playlist.txt");
    if(file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.empty()) continue;
            stringstream ss(line);
            string name, listenerId, playlistId;

            getline(ss, name, '&');
            getline(ss, listenerId, '&');
            getline(ss, playlistId, '&');

            if (name.empty() || listenerId.empty() || playlistId.empty()) continue;

            try {
                int lId = std::stoi(listenerId);
                int pId = std::stoi(playlistId);

                Playlist temp(name, lId, pId);
                list.push_back(temp);

                if (pId >= nextId) nextId = pId + 1;
            }
            catch (...) {
                continue;
            }
        }
    }
    file.close();
}
void PlaylistRepository::removeFromFile(Playlist& target)
{
    for(auto it=list.begin();it!=list.end();it++)
    {
        if(it->getName()==target.getName() && it->getPlaylistId()==target.getPlaylistId())
        {
            list.erase(it);
            break;
        }
    }
    ofstream file("playlist.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    for(auto&l:list)
    {
        saveToFile(l);
    }
    file.close();
}
int PlaylistRepository::getIdByName(string playlistName)
{
    for(auto&l:list)
    {
        if(l.getName()==playlistName)return l.getPlaylistId();
    }
    return -1;
}
void PlaylistRepository::updateName(int playlistId,string&newName)
{
    for(auto&l:list)
    {
        if(l.getPlaylistId()==playlistId)
        {
            l.setPlaylistName(newName);
            break;
        }
    }
}