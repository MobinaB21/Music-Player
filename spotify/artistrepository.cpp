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
            if(!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if(line.empty()) continue;

            stringstream ss(line);
            string name, username, biography, id, role, password;

            if (!getline(ss, name, '&')) continue;
            if (!getline(ss, username, '&')) continue;
            if (!getline(ss, biography, '&')) continue;
            if (!getline(ss, id, '&')) continue;
            if (!getline(ss, role, '&')) continue;
            if (!getline(ss, password, '&')) continue;
            try
            {
                int parsedId = stoi(id);
                Account temp(name, username, biography, parsedId, role, password);
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
int ArtistRepository::getIdByName(string name)
{
    for(auto&a:artists)
    {
        if(a.getFullName()==name)return a.getId();
    }
    return 0;
}
vector<Account> ArtistRepository::getArtists(){return artists;}