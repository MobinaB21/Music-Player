#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H
#include"abstractrepository.h"
#include"account.h"
class AccountRepository :public AbstractRepository<Account>
{
public:
    AccountRepository();
    int save(const Account&input)override=0;
    bool remove(int id)override=0;
    optional<Account>search(int id)override=0;
    virtual optional<Account> searchByUserName(const string&userName)=0;
    virtual ~AccountRepository();
};

#endif // ACCOUNTREPOSITORY_H
