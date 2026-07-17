#include "albumrepository.h"
#include<fstream>
#include<sstream>
AlbumRepository::AlbumRepository() {loadFromFile();}
int AlbumRepository::save(const Album&input)
{
    Album temp=input;
    if(temp.getAlbumId()==0)
    {
        temp.setAlbumId(nextId++);
        albumList.push_back(temp);
        saveToFile(temp);
        return temp.getAlbumId();
    }
    for(auto &s: albumList)
    {
        if(s.getAlbumId()==input.getAlbumId())
        {
            s=input;
            return s.getAlbumId();
        }
    }
    albumList.push_back(input);
    return input.getAlbumId();
}
bool AlbumRepository::remove(int id)
{
    for(auto it=albumList.begin();it!=albumList.end();it++)
    {
        if(it->getAlbumId()==id)
        {
            albumList.erase(it);
            return true;
        }
    }
    return false;
}
optional<Album> AlbumRepository::search(int id)
{
    for(auto &a:albumList)
    {
        if(a.getAlbumId()==id)return a;
    }
    return nullopt;
}
vector<Album> AlbumRepository::albums(int artistId)
{
    vector<Album>result;
    for(auto &a:albumList)
    {
        if(a.getArtistId()==artistId)result.push_back(a);
    }
    return result;
}
void AlbumRepository::saveToFile(const Album&input)
{
    ofstream file("album.txt",ios::app);
    if(file.is_open())
    {
        file<<input.getAlbumName()<<"&"<<input.getAlbumId()<<"&"<<input.getArtistId()<<"\n";
    }
    file.close();
}
void AlbumRepository::loadFromFile()
{
    ifstream file("album.txt");
    if(file.is_open())
    {
        string line;
        while(getline(file,line))
        {
            if(line.empty())continue;
            stringstream ss(line);
            string albumName,artistId,albumId;
            getline(ss,albumName,'&');
            getline(ss,albumId,'&');
            getline(ss,artistId,'&');
            Album temp(albumName,stoi(albumId),stoi(artistId));
            albumList.push_back(temp);
            if(stoi(albumId)>=nextId)nextId=stoi(albumId)+1;
        }
    }
    file.close();
}
void AlbumRepository::removeFromFile(Album& target)
{
    for(auto it=albumList.begin();it!=albumList.end();it++)
    {
        if(it->getAlbumName()==target.getAlbumName() && it->getAlbumId()==target.getAlbumId())
        {
            albumList.erase(it);
            break;
        }
    }
    ofstream file("album.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    for(auto&s:albumList)
    {
        saveToFile(s);
    }
    file.close();
}
int AlbumRepository::getIdByName(string name)
{
    for(auto&a:albumList)
    {
        if(a.getAlbumName()==name)return a.getAlbumId();
    }
    return 0;
}