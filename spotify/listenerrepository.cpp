#include "listenerrepository.h"
#include<fstream>
#include<sstream>
ListenerRepository::ListenerRepository() {ListenerRepository::loadFromFile();}
int ListenerRepository::save(const Account&input)
{
    Account temp=input;
    if(temp.getId()==0)
    {
        temp.setId(nextId++);
        listeners.push_back(temp);
        saveToFile(temp);
        return temp.getId();
    }
    for(size_t i=0;i<listeners.size();i++)
    {
        if(listeners[i].getId()==temp.getId())
        {
            listeners[i]=temp;
            return temp.getId();
        }
    }
    listeners.push_back(temp);
    return temp.getId();
}
bool ListenerRepository::remove(int id)
{
    for(auto it=listeners.begin();it!=listeners.end();it++)
    {
        if(it->getId()==id)
        {
            listeners.erase(it);
            return true;
        }
    }
    return false;
}
optional<Account> ListenerRepository::search(int id)
{
    for(auto&l:listeners)
    {
        if(l.getId()==id)return l;
    }
    return nullopt;
}
optional<Account>ListenerRepository::searchByUserName(const string&userName)
{
    for(auto&l:listeners)
    {
        if(l.getUserName()==userName)return l;
    }
    return nullopt;
}
bool ListenerRepository::isLiked(int listenerId,int songId)
{
    if(likedSongs.find(listenerId)!=likedSongs.end())
    {
        vector<int>&songs=likedSongs[listenerId];
        for(auto&s:songs)
        {
            if(s==songId)return true;
        }
    }
    return false;
}
void ListenerRepository::updateLiked(int listenerId,int songId,bool isLiked)
{
    vector<int>&songs=likedSongs[listenerId];
    if(isLiked)
    {
        bool found=false;
        for(auto i=0;i<songs.size();i++)
        {
            if(songs[i]==songId)
            {
                found=true;
                break;
            }
        }
        if(!found)songs.push_back(songId);
    }
    else
    {
        int index=-1;
        for(auto i=0;i<songs.size();i++)
        {
            if(songs[i]==songId)
            {
                index=i;
                break;
            }
        }
        if(index!=-1)songs.erase(songs.begin()+index);
    }
}
void ListenerRepository::saveToFile(const Account&input)
{
    ofstream file("listener.txt",ios::out);
    if(file.is_open())
    {
        file<<input.getFullName()<<"&"<<input.getUserName()<<"&"<<input.getBiography()<<"&"<<input.getId()<<"&"<<input.getRole()<<"&"<<input.getPassword()<<"&"<<input.getProfilePhoto()<<"\n";
    }
    file.close();
}
void ListenerRepository::loadFromFile()
{
    ifstream file("listener.txt");
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
            string name, username, biography, id, role, password,profile;

            if (!getline(ss, name, '&')) continue;
            if (!getline(ss, username, '&')) continue;
            if (!getline(ss, biography, '&')) continue;
            if (!getline(ss, id, '&')) continue;
            if (!getline(ss, role, '&')) continue;
            if (!getline(ss, password, '&')) continue;
            if(!getline(ss,profile,'&'))continue;
            try {
                int parsedId = stoi(id);
                Account temp(name, username, biography, parsedId, role, password,profile);
                listeners.push_back(temp);
                if(parsedId >= nextId) nextId = parsedId + 1;
            }
            catch (const exception& e) {
                continue;
            }
        }
    }
    file.close();
}
void ListenerRepository::removeFromFile(int id)
{
    for(auto it=listeners.begin();it!=listeners.end();it++)
    {
        if(it->getId()==id)
        {
            listeners.erase(it);
            break;
        }
    }
    ofstream file("listener.txt",ios::out | ios::trunc);
    if(!file.is_open())return;
    file.clear();
    for(auto&l:listeners)
    {
        saveToFile(l);
    }
    file.close();
}
void ListenerRepository::updateListener(int listenerId,string&name,string& newUserName,string& newPassword,string&biography,string profile)
{
    for(auto& l:listeners)
    {
        if(l.getId()==listenerId)
        {
            l.setFullName(name);
            l.setUserName(newUserName);
            l.setPassword(newPassword);
            l.setBiography(biography);
            l.setProfilePhoto(profile);
            return;
        }
    }
}
 ListenerRepository::~ListenerRepository(){}

