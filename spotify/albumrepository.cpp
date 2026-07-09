#include "albumrepository.h"

AlbumRepository::AlbumRepository() {}
int AlbumRepository::save(const Album&input)
{
    for(auto &a:albumList)
    {
        if(a.getAlbumId()==input.getAlbumId())
        {
            a=input;
            return input.getAlbumId();
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