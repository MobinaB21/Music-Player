#ifndef ARTISTREPOSITORY_H
#define ARTISTREPOSITORY_H
#include"accountrepository.h"
#include<vector>
class ArtistRepository:public AccountRepository
{
private:
    vector<Account>artists;
    int nextId=1;
public:
    ArtistRepository();
    int save(const Account&input)override;
    bool remove(int id)override;
    optional<Account>search(int id)override;
    optional<Account>searchByUserName(const string&userName)override;
    void saveToFile(const Account&input)override;
    void loadFromFile()override;
};

#endif // ARTISTREPOSITORY_H
