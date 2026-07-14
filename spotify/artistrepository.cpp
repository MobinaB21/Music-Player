#include "artistrepository.h"
#include<fstream>
#include<sstream>
ArtistRepository::ArtistRepository() {ArtistRepository::loadFromFile();}
int ArtistRepository::save(const Account&input)
{
    Account temp=input;
    if(temp.getId()==0)
    {
        temp.setId(nextId++);
        artists.push_back(temp);
        saveToFile(temp);
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
void ArtistRepository::saveToFile(const Account&input)
{
    ofstream file("artist.txt",ios::app);
    if(file.is_open())
    {
        file<<input.getFullName()<<"&"<<input.getUserName()<<"&"<<input.getBiography()<<"&"<<input.getId()<<"&"<<input.getRole()<<"&"<<input.getPassword()<<"\n";
    }
    file.close();
}
void ArtistRepository::loadFromFile()
{
    ifstream file("artist.txt");
    if(file.is_open())
    {
        string line;
        while(getline(file,line))
        {
            if(line.empty())continue;
            stringstream ss(line);
            string name,username,biography,id,role,password;
            getline(ss,name,'&');
            getline(ss,username,'&');
            getline(ss,biography,'&');
            getline(ss,id,'&');
            getline(ss,role,'&');
            getline(ss,password,'&');
            Account temp(name,username,biography,stoi(id),role,password);
            artists.push_back(temp);
            if(stoi(id)>=nextId)nextId=stoi(id)+1;
        }
    }
    file.close();
}