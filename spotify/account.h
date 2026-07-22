#ifndef ACCOUNT_H
#define ACCOUNT_H
#include<string>
using namespace std;
class Account
{
private:
    string fullName;
    string userName;
    string biography;
    int id;
    string role;
    string password;
    string profilePhoto;
public:
    Account(string fullName,string userName,string biography,int id,string role,string password,string profilePhoto);

    string getFullName()const;
    string getUserName()const;
    string getBiography()const;
    int getId()const;
    string getRole()const;
    string getPassword()const;
    string getProfilePhoto()const;
    void setFullName(const string & fullName);
    void setUserName(const string &userName);
    void setBiography(const string &biograpy);
    void setId(const int &id);
    void setRole(const string &role);
    void setPassword(const string &password);
    void setProfilePhoto(string & image);
};

#endif // ACCOUNT_H
