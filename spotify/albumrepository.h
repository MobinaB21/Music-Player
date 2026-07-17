#ifndef ALBUMREPOSITORY_H
#define ALBUMREPOSITORY_H
#include"abstractrepository.h"
#include"album.h"
#include<vector>
class AlbumRepository :public AbstractRepository<Album>
{
private:
    vector<Album>albumList;
    int nextId=1;
public:
    AlbumRepository();
    int save(const Album&input)override;
    bool remove(int id)override;
    optional<Album> search(int id)override;
    vector<Album> albums(int artistId);
    void saveToFile(const Album&input);
    void removeFromFile(Album& target);
    void loadFromFile();
    int getIdByName(string name);
};

#endif // ALBUMREPOSITORY_H
