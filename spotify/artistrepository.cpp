#include "artistrepository.h"

ArtistRepository::ArtistRepository() {}
int ArtistRepository::save(const Account&input)
{
    Account temp=input;
    if(temp.getId()==0)
    {
        temp.setId(nextId++);
        artists.push_back(temp);
        return temp.getId();
    }
    for(size_t i=0;i<artists.size();i++)
    {
        if(artists[i].getId()==temp.getId())
        {
            artists[i]=temp;
            return temp.getId();
        }
    }
    artists.push_back(temp);
    return temp.getId();
}
bool ArtistRepository::remove(int id)
{
    for(auto it=artists.begin();it!=artists.end();it++)
    {
        if(it->getId()==id)
        {
            artists.erase(it);
            return true;
        }
    }
    return false;
}
optional<Account> ArtistRepository::search(int id)
{
    for(auto&a:artists)
    {
        if(a.getId()==id)return a;
    }
    return nullopt;
}
optional<Account> ArtistRepository::searchByUserName(const string&userName)
{
    for(auto&a:artists)
    {
        if(a.getUserName()==userName)return a;
    }
    return nullopt;
}