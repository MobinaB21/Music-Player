#ifndef ABSTRACTREPOSITORY_H
#define ABSTRACTREPOSITORY_H
#include<optional>
template<class T>
class AbstractRepository
{
public:
    AbstractRepository();
   virtual int save(const T&input)=0;
    virtual bool remove(int id)=0;
    virtual std::optional<T> search(int id)=0;
    virtual ~AbstractRepository();
};

#endif // ABSTRACTREPOSITORY_H
