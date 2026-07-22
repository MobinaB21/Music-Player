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
optional<Account> ArtistRepository::searchByName(string &name)
{
    for(auto&a:artists)
    {
        if(a.getFullName()==name)return a;
    }
    return nullopt;
}
void ArtistRepository::saveToFile(const Account&input)
{
    ofstream file("artist.txt",ios::out | ios::trunc);
    if(file.is_open())
    {for(auto&a:artists)
        {
            file<<a.getFullName()<<"&"<<a.getUserName()<<"&"<<a.getBiography()<<"&"<<a.getId()<<"&"<<a.getRole()<<"&"<<a.getPassword()<<"&"<<a.getProfilePhoto()<<"\n";
        }
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
            if(line.empty()) continue;

            stringstream ss(line);
            string name, username, biography, id, role, password,profilePhoto;

            if (!getline(ss, name, '&')) continue;
            if (!getline(ss, username, '&')) continue;
            if (!getline(ss, biography, '&')) continue;
            if (!getline(ss, id, '&')) continue;
            if (!getline(ss, role, '&')) continue;
            if (!getline(ss, password, '&')) continue;
            if(!getline(ss,profilePhoto,'&'))continue;
            try
            {
                int parsedId = stoi(id);
                Account temp(name, username, biography, parsedId, role, password,profilePhoto);
                artists.push_back(temp);
                if(parsedId >= nextId) nextId = parsedId + 1;
            }
            catch (const exception& e) {
                continue;
            }
        }
    }
    file.close();
}
void ArtistRepository::removeFromFile(int id)
{
    for(auto it=artists.begin();it!=artists.end();it++)
    {
        if(it->getId()==id)
        {
            artists.erase(it);
            break;
        }
    }
    ofstream file("artist.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    for(auto&a:artists)
    {
        saveToFile(a);
    }
    file.close();
}
int ArtistRepository::getIdByName(string name)
{
    for(auto&a:artists)
    {
        if(a.getFullName()==name)return a.getId();
    }
    return 0;
}
void ArtistRepository::updateArtist(int listenerId,string&name,string& newUserName,string& newPassword,string&biography,string profile)
{
    for(auto& a:artists)
    {
        if(a.getId()==listenerId)
        {
            a.setFullName(name);
            a.setUserName(newUserName);
            a.setPassword(newPassword);
            a.setBiography(biography);
            a.setProfilePhoto(profile);
            return;
        }
    }
}
vector<Account> ArtistRepository::getArtists(){return artists;}