#ifndef LISTENERREPOSITORY_H
#define LISTENERREPOSITORY_H
#include"accountrepository.h"
#include<vector>
#include<map>
class ListenerRepository:public AccountRepository
{
private:
    vector<Account>listeners;
    map<int,vector<int>>likedSongs;
    int nextId=1;
public:
    ListenerRepository();
    int save(const Account&input)override;
    bool remove(int id)override;
    optional<Account>search(int id)override;
    optional<Account>searchByUserName(const string&userName)override;
    void updateLiked(int listenerId,int songId,bool isLiked);
    bool isLiked(int listenerId,int songId);
    void saveToFile(const Account&input)override;
    void loadFromFile()override;
    void removeFromFile(int id);
    void updateListener(int listenerId,string&name,string& newUserName,string& newPassword,string &biograghy);
    ~ListenerRepository()override;
};

#endif // LISTENERREPOSITORY_H
