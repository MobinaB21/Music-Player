#ifndef ALBUMREPOSITORY_H
#define ALBUMREPOSITORY_H
#include"abstractrepository.h"
#include"album.h"
#include<vector>
class AlbumRepository :public AbstractRepository<Album>
{
private:
    vector<Album>albumList;
public:
    AlbumRepository();
    int save(const Album&input)override;
    bool remove(int id)override;
    optional<Album> search(int id)override;
    vector<Album> albums(int artistId);
};

#endif // ALBUMREPOSITORY_H
