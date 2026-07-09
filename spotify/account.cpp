#include "account.h"
using namespace std;
Account::Account(string fullName,string userName,string biography,int id,string role,string password)
{
    this->fullName=fullName;
    this->userName=userName;
    this->biography=biography;
    this->id=id;
    this->role=role;
    this->password=password;
}
string Account::getFullName()const{return fullName;}
string Account::getUserName()const{return userName;}
string Account::getBiography()const{return biography;}
int Account::getId()const{return id;}
string Account::getRole()const{return role;}
string Account::getPassword()const {return password;}

void Account::setFullName(const string &fullName){this->fullName=fullName;}
void Account::setUserName(const string &userName){this->userName=userName;}
void Account::setBiography(const string &biography){this->biography=biography;}
void Account::setId(const int &id){this->id=id;}
void Account::setRole(const string &role){this->role=role;}
void Account::setPassword(const string &password){this->password=password;}