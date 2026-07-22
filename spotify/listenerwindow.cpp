#include "listenerwindow.h"
#include "ui_listenerwindow.h"
#include<QMessageBox>
#include"songrepository.h"
#include"albumrepository.h"
#include"artistrepository.h"
#include"playlistrepository.h"
#include<QInputDialog>
#include<algorithm>
#include"listenerrepository.h"
#include<QFileDialog>
ListenerWindow::ListenerWindow(int listenerId,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenerWindow)

{
    ui->setupUi(this);
    ui->comboGenreFilter->addItem("All");
    ui->comboGenreFilter->addItems({"Pop", "Rock", "Rap", "Jazz", "Classical","Hip Hop"});
    ui->comboYearFilter->addItem("All");
    for(int year=2026;year>=0;year--)
    {
         ui->comboYearFilter->addItem(QString::number(year));
    }
    connect(ui->lineSearch, &QLineEdit::textChanged, this, &ListenerWindow::filterSongs);
    connect(ui->comboGenreFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterSongs);
    connect(ui->comboYearFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterSongs);
    connect(ui->lineSearch, &QLineEdit::textChanged, this, &ListenerWindow::filterMusics);
    connect(ui->comboGenreFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterMusics);
    connect(ui->comboYearFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterMusics);
    setAttribute(Qt::WA_DeleteOnClose);
    this->listenerId=listenerId;
    SongRepository::favorite.clear();
    SongRepository tempSongRepo;
    for (auto& pair : SongRepository::likedSongs)
    {
        if (pair.first == this->listenerId)
        {
            auto target = tempSongRepo.search(pair.second);
            if (target.has_value())
            {
                SongRepository::favorite.push_back(target.value());
            }
        }
    }
    loadArtists();
    loadPlaylists();
    connect(ui->listWidgetArtists, &QListWidget::itemClicked, this, &ListenerWindow::on_listWidgetArtists_itemClicked);
    connect(ui->listWidgetAlbums, &QListWidget::itemClicked, this, &ListenerWindow::on_listWidgetAlbums_itemClicked);
    ListenerRepository tempListener;
    auto listener=tempListener.search(listenerId);
    ui->lblImage->setFixedSize(30,30);
    QString profilePhoto=QString::fromStdString(listener.value().getProfilePhoto());
    QPixmap pixmap(profilePhoto);
    ui->lblImage->setPixmap(pixmap.scaled(ui->lblImage->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->lblName->setText("Welcom back "+QString::fromStdString(listener.value().getFullName()));
    player=new PlaybackList(this);
}
ListenerWindow::~ListenerWindow()
{
    delete ui;
}
void ListenerWindow::setLoginWindow(LoginWindow * lw){ login=lw;}
void ListenerWindow::loadArtists()
{
    ui->listWidgetArtists->clear();
    ArtistRepository artist;
    vector<Account>allArtists=artist.getArtists();
    for(auto&a:allArtists)
    {
        QSize iconSize(70,70);
        ui->listWidgetArtists->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(a.getProfilePhoto());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(a.getFullName()));
        ui->listWidgetArtists->addItem(item);
    }
    ui->listWidgetArtists->sortItems(Qt::AscendingOrder);
}
void ListenerWindow::loadPlaylists()
{
    ui->listWidgetPlaylist->clear();
    PlaylistRepository playlistRepo;
    vector<Playlist>allPlaylists=playlistRepo.playlists(this->listenerId);
    QSize iconSize(70,70);
    ui->listWidgetPlaylist->setIconSize(iconSize);
    QString coverPath="F:/screan shots/Screenshot 2026-07-22 124921.png";
    QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString("Favorites"));
    ui->listWidgetPlaylist->addItem(item);
    sort(allPlaylists.begin(), allPlaylists.end(), [](const Playlist& a, const Playlist& b) {
        string name1 = a.getName();
        string name2 = b.getName();
        transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
        return name1 < name2;
    });
    for(auto&a:allPlaylists)
    {
        QSize iconSize(70,70);
        ui->listWidgetPlaylist->setIconSize(iconSize);
        QString coverPath="F:/screan shots/photo28667259506.jpg";
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(a.getName()));
        ui->listWidgetPlaylist->addItem(item);
    }
}
void ListenerWindow::on_pushButtonLike_clicked()
{
    SongRepository temp3;
    QListWidgetItem *selectedSong=ui->listWidget_3->currentItem();
    if(!selectedSong)
    {
        QMessageBox::warning(this,"warning","Please select a song ");
        return;
    }
    QListWidgetItem *selectedArtist=ui->listWidgetArtists->currentItem();
    QListWidgetItem *selectedAlbum=ui->listWidgetAlbums->currentItem();
    QString songName=selectedSong->text();
    auto aim=temp3.getSongByName(songName.toStdString());
    if(!aim.has_value())return;
    ArtistRepository temp1;
    QString artistName=selectedArtist->text();
    int artistId=temp1.getIdByName(artistName.toStdString());
    AlbumRepository temp2;
    vector<Album>albumList=temp2.albums(artistId);
    QString albumName=selectedAlbum->text();
    int albumId=temp2.getIdByName(albumName.toStdString());
    vector<Song>songs=temp3.getByAlbum(albumId);
    ui->listWidgetMusic->sortItems(Qt::AscendingOrder);
    int foundIndex = -1;
    for (size_t i = 0; i < SongRepository::likedSongs.size(); i++) {
        if (SongRepository::likedSongs[i].first == this->listenerId && SongRepository::likedSongs[i].second == aim.value().getSongId()) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex != -1)QMessageBox::information(this, "Favorites","You liked this song before");
    else
    {
        SongRepository::likedSongs.push_back({this->listenerId, aim.value().getSongId()});
        SongRepository::favorite.push_back(aim.value());
        QMessageBox::information(this, "Favorites","Song added to favorites");
    }

    temp3.saveLikedSongsToFile();
    if (ui->listWidgetPlaylist->currentItem())
    {
        on_listWidgetPlaylist_itemClicked(ui->listWidgetPlaylist->currentItem());
    }
}
void ListenerWindow::on_pushButtonLogout_clicked()
{
    if(login)
    {
        login->show();
    }
    this->close();
}
void ListenerWindow::on_pushButtonAddSong_clicked()
{
    SongRepository temp3;
    QListWidgetItem *selectedSong=ui->listWidget_3->currentItem();
    if(!selectedSong)
    {
        QMessageBox::warning(this,"warning","Please select one of the songs");
        return;
    }
    QListWidgetItem *selectedArtist=ui->listWidgetArtists->currentItem();
    QListWidgetItem *selectedAlbum=ui->listWidgetAlbums->currentItem();
    QString songName=selectedSong->text();
    auto aim=temp3.getSongByName(songName.toStdString());
    if(!aim.has_value())return;
    ArtistRepository temp1;
    QString artistName=selectedArtist->text();
    int artistId=temp1.getIdByName(artistName.toStdString());
    AlbumRepository temp2;
    vector<Album>albumList=temp2.albums(artistId);
    QString albumName=selectedAlbum->text();
    int albumId=temp2.getIdByName(albumName.toStdString());
    vector<Song>songs=temp3.getByAlbum(albumId);
    PlaylistRepository playlist;
    vector<Playlist>list=playlist.playlists(this->listenerId);
    if(list.empty())
    {
        QMessageBox::warning(this,"warning","There is no playlists");
        return;
    }
    QStringList items;
    for(auto&p:list)
    {
        if(p.getName()!="Favorites")
        {
            items<<QString::fromStdString(p.getName());
        }
    }
    bool ok;
    QString selectItem=QInputDialog::getItem(this,"Select Playlist","Please select a playlist to add this song",items,0,false,&ok);
    if(ok && !selectItem.isEmpty())
    {
        int id=-1;
        for(auto&p:list)
        {
            if(p.getName()==selectItem.toStdString())
            {
                id=p.getPlaylistId();
                break;
            }
        }
        vector<Song>selectedPlaylistSongs=temp3.getByPlaylist(id);
        for(auto&s:selectedPlaylistSongs)
        {
            if(s.getSongName()==aim.value().getSongName())
            {
                QMessageBox::warning(this,"warning","You added this song to this playlist before");
                return;
            }
        }
        SongRepository::playlistSongs.push_back({id,aim.value().getSongId()});
        SongRepository::savePlaylistSongsToFile();
        QMessageBox::information(this,"information","Song added to playlist succussfully");
    }
}
void ListenerWindow::on_listWidgetArtists_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetAlbums->clear();
    ui->listWidget_3->clear();
    string artistName=item->text().toStdString();
    ArtistRepository artist;
    auto temp=artist.searchByName(artistName);
    if(!temp.has_value())return;
    int artistId=temp.value().getId();
    AlbumRepository album;
    vector<Album>albums=album.albums(artistId);
    vector<string>albumNames;
    for(auto&a:albums)
    {
        albumNames.push_back(a.getAlbumName());
    }
    SongRepository song;
    vector<Song>singleSongs=song.singleSong(artistId);
    for(auto& a:albumNames)
    {
        int albumId=album.getIdByName(a);
        auto realAlbum=album.search(albumId);
        QSize iconSize(70,70);
        ui->listWidgetAlbums->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(realAlbum.value().getCover());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(a));
        ui->listWidgetAlbums->addItem(item);
    }
    QSize iconSize(70,70);
    ui->listWidgetAlbums->setIconSize(iconSize);
    QString coverPath="F:/screan shots/Screenshot 2026-07-21 190410.png";
    QListWidgetItem *singleSong=new QListWidgetItem(QIcon(coverPath),"Singles");
    ui->listWidgetAlbums->addItem(singleSong);
     ui->listWidgetAlbums->sortItems(Qt::AscendingOrder);
}
void ListenerWindow::on_listWidgetAlbums_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_3->clear();
    string albumName=item->text().toStdString();
    AlbumRepository album;
    int albumId=album.getIdByName(albumName);
    SongRepository temp;
    vector<Song> songs=temp.getByAlbum(albumId);
    QString sortCriteria = ui->comboSort->currentText();
    if (sortCriteria == "Name")
    {
        std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
            return a.getSongName() < b.getSongName();
        });
    }
    else if (sortCriteria == "Year")
    {
        std::sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
            return a.getReleaseYear() > b.getReleaseYear();
        });
    }
    for(auto&s:songs)
    {
        QSize iconSize(70,70);
        ui->listWidget_3->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(s.getSongImage());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(s.getSongName()));
        ui->listWidget_3->addItem(item);
    }
}
void ListenerWindow::on_pushButtonAddPlaylist_clicked()
{
    bool ok;
    QString text=QInputDialog::getText(this,"New Playlist","Enter playlistName:",QLineEdit::Normal,"",&ok);
    if(text.isEmpty())
    {
        QMessageBox::warning(this,"Error","Please enter the name of new playlist");
        return;
    }
    if(ok&& !text.isEmpty())
    {
         string playlistName=text.toStdString();
        if(playlistName=="Favorites")
        {
            QMessageBox::warning(this,"Error","You cant use this name");
            return;
        }
        PlaylistRepository temp;
        vector<Playlist>allPlaylists=temp.playlists(this->listenerId);
        for(auto&a:allPlaylists)
        {
            if(a.getName()==playlistName)
            {
                QMessageBox::warning(this,"Warning","This name was selected before");
                return;
            }
        }
        Playlist newPlaylist(playlistName,this->listenerId,0);
        temp.save(newPlaylist);
        loadPlaylists();
        QMessageBox::information(this,"information","New Playlist was added successfully");
    }
}
void ListenerWindow::on_pushButtonDelete_clicked()
{
    QListWidgetItem *selectedItem=ui->listWidgetPlaylist->currentItem();
    if(!selectedItem)
    {
        QMessageBox::warning(this,"warning","Please select one of the items");
        return;
    }
    string playlistName=selectedItem->text().toStdString();
    if(playlistName=="Favorites")
    {
        QMessageBox::warning(this,"warning","you cant remove this playlist");
        return;
    }
    PlaylistRepository temp;
    int playlistId=temp.getIdByName(playlistName);
    auto aim=temp.search(playlistId);
    if(playlistId==-1)return;
    temp.removeFromFile(aim.value());
    auto newEnd = std::remove_if(SongRepository::playlistSongs.begin(), SongRepository::playlistSongs.end(),
                                 [playlistId](const pair<int, int>& p) { return p.first == playlistId; });
        SongRepository::playlistSongs.erase(newEnd, SongRepository::playlistSongs.end());
        SongRepository::savePlaylistSongsToFile();
    ui->listWidgetPlaylist->clear();
    loadPlaylists();
    ui->listWidgetMusic->clear();
    QMessageBox::information(this,"Success","Playlist removed successfully");
}
void ListenerWindow::on_listWidgetPlaylist_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetMusic->clear();
    PlaylistRepository tempPlaylist;
    SongRepository tempSong;
    vector<Playlist>all=tempPlaylist.playlists(this->listenerId);
    vector<Song>likedSongs=tempSong.getByLikedSongs(this->listenerId);
    vector<Song>songs;
    for(auto&l:likedSongs)
    {
        songs.push_back(l);
    }
    for(auto&a:all)
    {
        vector<Song>playlistSongs=tempSong.getByPlaylist(a.getPlaylistId());
        for(auto&p:playlistSongs)
        {
            songs.push_back(p);
        }
        playlistSongs.clear();
    }
    ui->listWidgetMusic->clear();
    QString itemName=item->text();
    vector<Song>allSongs;
    if(itemName=="Favorites")
    {
        for(auto&f:SongRepository::favorite)
        {
            allSongs.push_back(f);
        }
        QString sortCriteria = ui->comboSort->currentText();
        if (sortCriteria == "Name")
        {
            std::sort(allSongs.begin(), allSongs.end(), [](const Song& a, const Song& b) {
                return a.getSongName() < b.getSongName();
            });
        }
        else if (sortCriteria == "Year")
        {
            std::sort(allSongs.begin(), allSongs.end(), [](const Song& a, const Song& b) {
                return a.getReleaseYear() > b.getReleaseYear();
            });
        }
        for(auto&a:allSongs)
        {
            QSize iconSize(70,70);
            ui->listWidgetMusic->setIconSize(iconSize);
            QString coverPath=QString::fromStdString(a.getSongImage());
            QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(a.getSongName()));
            ui->listWidgetMusic->addItem(item);
        }
        return;
    }
    PlaylistRepository temp;
    int playlistId=temp.getIdByName(itemName.toStdString());
    if(playlistId==-1)return;
    vector<int>songId;
    for(auto&p:SongRepository::playlistSongs)
    {
        if(p.first==playlistId)songId.push_back(p.second);
    }
    SongRepository temp2;
    for(auto&s:songId)
    {
        auto target=temp2.search(s);
        if(!target.has_value())return;
        allSongs.push_back(target.value());
    }
    QString sortCriteria = ui->comboSort->currentText();
    if (sortCriteria == "Name")
    {
        std::sort(allSongs.begin(), allSongs.end(), [](const Song& a, const Song& b) {
            return a.getSongName() < b.getSongName();
        });
    }
    else if (sortCriteria == "Year")
    {
        std::sort(allSongs.begin(), allSongs.end(), [](const Song& a, const Song& b) {
            return a.getReleaseYear() > b.getReleaseYear();
        });
    }
    for(auto&s:allSongs)
    {
        QSize iconSize(70,70);
        ui->listWidgetMusic->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(s.getSongImage());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(s.getSongName()));
        ui->listWidgetMusic->addItem(item);
    }
}
void ListenerWindow::on_pushButtonDeleteMusic_clicked()
{
    QListWidgetItem *selectedPlaylist=ui->listWidgetPlaylist->currentItem();
    QListWidgetItem *selectedSong=ui->listWidgetMusic->currentItem();
    if(!selectedPlaylist || !selectedSong)
    {
        QMessageBox::warning(this,"warning","Please select an item");
        return;
    }
    QString selectedPlaylistName=selectedPlaylist->text();
    QString selectedSongName=selectedSong->text();
    if(selectedPlaylistName=="Favorites")
    {
        SongRepository temp;
        auto it=find_if(SongRepository::favorite.begin(),SongRepository::favorite.end(),[&selectedSongName](Song&s)
                          {
                              return s.getSongName()==selectedSongName;
                          });
        int songId;
        if(it!=SongRepository::favorite.end())songId=it->getSongId();
        auto newLikedEnd=remove_if(SongRepository::likedSongs.begin(),SongRepository::likedSongs.end(),[this,songId](pair<int,int>&p)
                                     {
                                         return p.first==this->listenerId&&p.second==songId;
                                     });
        SongRepository::likedSongs.erase(newLikedEnd,SongRepository::likedSongs.end());
        auto FavEnd=remove_if(SongRepository::favorite.begin(),SongRepository::favorite.end(),[songId](Song&s)
                                {
                                    return s.getSongId()==songId;
                                });
        SongRepository::favorite.erase(FavEnd,SongRepository::favorite.end());
        temp.saveLikedSongsToFile();
    }
    else
    {
        PlaylistRepository temp;
        int playlistId=temp.getIdByName(selectedPlaylistName.toStdString());
        if(playlistId==-1)return;
        SongRepository tempSong;
        int songId=-1;
        for(auto&p:SongRepository::playlistSongs)
        {
            if(p.first==playlistId)
            {
                auto target=tempSong.search(p.second);
                if(target.has_value()&&target.value().getSongName()==selectedSongName)
                {
                    songId=p.second;
                    break;
                }
            }
        }
        if(songId!=-1)
        {
            auto it=find_if(SongRepository::playlistSongs.begin(),SongRepository::playlistSongs.end(),[playlistId,songId](pair<int,int>&p)
                {
                return p.first==playlistId && p.second==songId;
            });
            if(it!=SongRepository::playlistSongs.end())
            {
                SongRepository::playlistSongs.erase(it);
                SongRepository::savePlaylistSongsToFile();
            }
        }
    }
    on_listWidgetPlaylist_itemClicked(selectedPlaylist);
    QMessageBox::information(this,"Success","Song removed from playlist successfully");
}
void ListenerWindow::on_pushButtonEdit_clicked()
{
    QString name=ui->lineEditName->text();
    QString userName=ui->lineEditUsername->text();
    QString password=ui->lineEditPassword->text();
    QString biography=ui->textEdit->toPlainText();
    QString photo=ui->lineEditPhoto->text();
    string name2=name.toStdString();
    string userName2=userName.toStdString();
    string password2=password.toStdString();
    string biography2=biography.toStdString();
    ListenerRepository temp;
    if(userName.isEmpty() || password.isEmpty() || name.isEmpty() || biography.isEmpty() )
    {
        QMessageBox::warning(this,"Error","Please fill the lines");
        return;
    }
    if(name.contains("&") || userName.contains("&") || password.contains("&") || biography.contains("&"))
    {
        QMessageBox::warning(this,"Invalid character","You cant use character '&' ");
        return;
    }
    if(ui->lblPasswordStrength->text().contains("Weak"))
    {
        QMessageBox::warning(this,"warning","Password must contain at least 12 characters long and including at least one uppercase letter, one lowercase letter,one digit and one special charecter");
        return ;
    }
    if(photo.isEmpty())photo="F:/screan shots/Screenshot 2026-07-21 204414.png";
    auto find=temp.searchByUserName(userName2);
    if(find && find.value().getId()!=listenerId)
    {
        QMessageBox::critical(this,"warning","This username was selected before");
        return;
    }
    temp.updateListener(this->listenerId,name2,userName2,password2,biography2,photo.toStdString());
    auto it=temp.search(this->listenerId);
    temp.saveToFile(it.value());
    QMessageBox::information(this,"Success","Account updated successfully");
}
void ListenerWindow::on_pushButtonDelete_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete Account","Do you want to delete your account ?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::No)return;
    ListenerRepository temp;
    PlaylistRepository tempPlaylist;
    SongRepository tempSong;
    vector<Playlist>playlists=tempPlaylist.playlists(this->listenerId);
    for(auto&p:playlists)
    {
        tempPlaylist.removeFromFile(p);
        tempSong.removePlaylistSongsFromFile(p.getPlaylistId());
    }
    tempSong.removeLikedSongsFromFile(this->listenerId);
    temp.removeFromFile(this->listenerId);
    QMessageBox::information(this,"Delete Account","Your account was deleted successfully");
    if(login)
    {
        login->show();
    }
    this->close();
}

void ListenerWindow::on_pushButtonEditPlaylistName_clicked()
{
    PlaylistRepository tempPlaylist;
    vector<Playlist>allPlaylists=tempPlaylist.playlists(this->listenerId);
    QListWidgetItem *item=ui->listWidgetPlaylist->currentItem();
    if(!item)
    {
        QMessageBox::warning(this,"Warning","Please select one of the playlists");
        return;
    }
    QString oldName=item->text();
    int playlistId=tempPlaylist.getIdByName(oldName.toStdString());
    if(oldName=="Favorites")
    {
        QMessageBox::warning(this,"Error","You cant change the name of this playlist");
        return;
    }
    bool ok;
    QString newName=QInputDialog::getText(this,"Edit Playlist Name","Enter the new name",QLineEdit::Normal,oldName,&ok);
    string newName2=newName.toStdString();
    if(newName=="Favorites")
    {
        QMessageBox::warning(this,"Error","You cant select this name");
        return;
    }
    for(auto&a:allPlaylists)
    {
        if(a.getName()==newName2)
        {
            QMessageBox::warning(this,"Error","This name was selected before");
            return;
        }
    }
    if(ok && !newName.isEmpty())
    {
        tempPlaylist.updateName( playlistId,newName2);
        auto it=tempPlaylist.search(playlistId);
        tempPlaylist.saveToFile(it.value());
        loadPlaylists();
        QMessageBox::information(this,"Success","Playlist was edited successfully");
    }
}
void ListenerWindow::filterSongs()
{
    QString find=ui->lineSearch->text();
    if(find.isEmpty())
    {
        ui->listWidget_3->clear();
        return;
    }
    SongRepository tempSong;
    vector<Song>currentSongs=tempSong.getAllSongs();
    ui->listWidget_3->clear();
    QString searchText = ui->lineSearch->text().trimmed();
    QString selectedGenre = ui->comboGenreFilter->currentText();
    QString selectedYear = ui->comboYearFilter->currentText();
    QString sortCriteria = ui->comboSort->currentText();
    if (sortCriteria == "Name")
    {
        std::sort(currentSongs.begin(), currentSongs.end(), [](const Song& a, const Song& b) {
            return a.getSongName() < b.getSongName();
        });
    }
    else if (sortCriteria == "Year")
    {
        std::sort(currentSongs.begin(), currentSongs.end(), [](const Song& a, const Song& b) {
            return a.getReleaseYear() > b.getReleaseYear();
        });
    }
    for (const auto& song : currentSongs)
    {
        QString songName = QString::fromStdString(song.getSongName());
        QString songGenre = QString::fromStdString(song.getGenre());
        QString songYear = QString::number(song.getReleaseYear());
        bool matchesSearch = songName.contains(searchText, Qt::CaseInsensitive);
        bool matchesGenre = (selectedGenre == "All" || songGenre == selectedGenre);
        bool matchesYear = (selectedYear == "All" || songYear == selectedYear);
        if (matchesSearch && matchesGenre && matchesYear)
        {
            QString coverPath=QString::fromStdString(song.getSongImage());
            QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(song.getSongName()));
            ui->listWidget_3->addItem(item);
        }
    }
}
void ListenerWindow::filterMusics()
{
    QString find=ui->lineSearch->text();
    if(find.isEmpty())
    {
        ui->listWidget_3->clear();
        return;
    }
    PlaylistRepository tempPlaylist;
    SongRepository tempSong;
    vector<Playlist>all=tempPlaylist.playlists(this->listenerId);
    vector<Song>likedSongs=tempSong.getByLikedSongs(this->listenerId);
    vector<Song>currentSongs;
    for(auto&l:likedSongs)
    {
        currentSongs.push_back(l);
    }
    for(auto&a:all)
    {
        vector<Song>playlistSongs=tempSong.getByPlaylist(a.getPlaylistId());
        for(auto&p:playlistSongs)
        {
            bool addedBefore=false;
            for(auto&c:currentSongs)
            {
                if(c.getSongName()==p.getSongName())addedBefore=true;
            }
            if(!addedBefore)currentSongs.push_back(p);
        }
        playlistSongs.clear();
    }
    ui->listWidgetMusic->clear();
    QString searchText = ui->lineSearch->text().trimmed();
    QString selectedGenre = ui->comboGenreFilter->currentText();
    QString selectedYear = ui->comboYearFilter->currentText();
    QString sortCriteria = ui->comboSort->currentText();

    if (sortCriteria == "Name")
    {
        std::sort(currentSongs.begin(), currentSongs.end(), [](const Song& a, const Song& b) {
            return a.getSongName() < b.getSongName();
        });
    }
    else if (sortCriteria == "Year")
    {
        std::sort(currentSongs.begin(), currentSongs.end(), [](const Song& a, const Song& b) {
            return a.getReleaseYear() > b.getReleaseYear();
        });
    }
    for (const auto& song : currentSongs)
    {
        QString songName = QString::fromStdString(song.getSongName());
        QString songGenre = QString::fromStdString(song.getGenre());
        QString songYear = QString::number(song.getReleaseYear());
        bool matchesSearch = songName.contains(searchText, Qt::CaseInsensitive);
        bool matchesGenre = (selectedGenre == "All" || songGenre == selectedGenre);
        bool matchesYear = (selectedYear == "All" || songYear == selectedYear);
        if (matchesSearch && matchesGenre && matchesYear)
        {
            QSize iconSize(70,70);
            ui->listWidgetArtists->setIconSize(iconSize);
            QString coverPath=QString::fromStdString(song.getSongImage());
            QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(song.getSongName()));
            ui->listWidgetMusic->addItem(item);
        }
    }
}
void ListenerWindow::on_pushButtonSearch_clicked()
{

    filterSongs();
}
void ListenerWindow::on_comboSort_currentIndexChanged(int index)
{
    filterSongs();
    if (ui->listWidgetPlaylist->currentItem())
    {
        on_listWidgetPlaylist_itemClicked(ui->listWidgetPlaylist->currentItem());
    }
}
void ListenerWindow::on_pushButtonPlay_clicked()
{
    player->play();
}


void ListenerWindow::on_pushButtonPause_clicked()
{
    player->pause();
}


void ListenerWindow::on_pushButtonNextSong_clicked()
{
    player->next();
}


void ListenerWindow::on_pushButtonPrevious_clicked()
{
    player->previous();
}


void ListenerWindow::on_listWidgetPlaylist_itemDoubleClicked(QListWidgetItem *item)
{
    QString playlistName=item->text();
    PlaylistRepository tempPlaylist;
    int playlistId=tempPlaylist.getIdByName(playlistName.toStdString());
    SongRepository tempSong;
    vector<Song>playlistSongs=tempSong.getByPlaylist(playlistId);
    vector<Song>tempPlaylistSongs;
    if(playlistName!="Favorites")tempPlaylistSongs=playlistSongs;
    else if(playlistName=="Favorites")tempPlaylistSongs=tempSong.getByLikedSongs(this->listenerId);
    if(ui->comboSort->currentText()=="Name")
    {
        sort(tempPlaylistSongs.begin(),tempPlaylistSongs.end(),[](Song&a,Song&b)
             {
                 return a.getSongName()<b.getSongName();
             });
    }
    else if(ui->comboSort->currentText()=="Year")
    {
        sort(tempPlaylistSongs.begin(),tempPlaylistSongs.end(),[](Song&a,Song&b)
             {
                 return a.getReleaseYear()>b.getReleaseYear();
             });
    }
    if(tempPlaylistSongs.empty())return;
    player->loadQueue(tempPlaylistSongs,0);
    player->play();
}
void ListenerWindow::on_listWidgetMusic_itemDoubleClicked(QListWidgetItem *item)
{
    QString songName=item->text();
    SongRepository tempSong;
    auto song=tempSong.getSongByName(songName.toStdString());
    if(!song.has_value())return;
    player->playSong(song.value());
}
void ListenerWindow::on_pushButtonPlay_2_clicked()
{
    player->play();
}
void ListenerWindow::on_pushButtonPause_2_clicked()
{
    player->pause();
}
void ListenerWindow::on_pushButtonNextSong_2_clicked()
{
    player->next();
}
void ListenerWindow::on_pushButtonPrevious_2_clicked()
{
    player->previous();
}


void ListenerWindow::on_listWidgetAlbums_itemDoubleClicked(QListWidgetItem *item)
{
    ArtistRepository tempArtist;
    QListWidgetItem *selectedArtist=ui->listWidgetArtists->currentItem();
    QString artist=selectedArtist->text();
    int artistId=tempArtist.getIdByName(artist.toStdString());
    QString albumName=item->text();
    AlbumRepository tempAlbum;
    int albumId=tempAlbum.getIdByName(albumName.toStdString());
    SongRepository tempSong;
    vector<Song>albumSongs=tempSong.getByAlbum(albumId);
    vector<Song>tempAlbumSongs;
    if(albumName!="Singles")tempAlbumSongs=albumSongs;
    else if(albumName=="Singles")tempAlbumSongs=tempSong.singleSong(artistId);
    if(ui->comboSort->currentText()=="Name")
    {
        sort(tempAlbumSongs.begin(),tempAlbumSongs.end(),[](Song&a,Song&b)
             {
                 return a.getSongName()<b.getSongName();
             });
    }
    else if(ui->comboSort->currentText()=="Year")
    {
        sort(tempAlbumSongs.begin(),tempAlbumSongs.end(),[](Song&a,Song&b)
             {
                 return a.getReleaseYear()>b.getReleaseYear();
             });
    }
    if(tempAlbumSongs.empty())return;
    player->loadQueue(tempAlbumSongs,0);
    player->play();
}


void ListenerWindow::on_listWidget_3_itemDoubleClicked(QListWidgetItem *item)
{
    QString songName=item->text();
    SongRepository tempSong;
    auto song=tempSong.getSongByName(songName.toStdString());
    if(!song.has_value())return;
    player->playSong(song.value());
}


void ListenerWindow::on_lineEditPassword_textChanged(const QString &password)
{
    if(password.isEmpty())
    {
        ui->lblPasswordStrength->setText("");
        return;
    }
    bool hasLower=password.contains(QRegularExpression("[a-z]"));
    bool hasUpper=password.contains(QRegularExpression("[A-Z]"));
    bool hasDigit=password.contains(QRegularExpression("[0-9]"));
    bool hasSpecial=password.contains(QRegularExpression("[^a-zA-Z0-9]"));
    bool hasAll= hasLower && hasUpper && hasDigit && hasSpecial;
    if(password.length()>=12 && hasAll)
    {
        ui->lblPasswordStrength->setText("Strong password");
        ui->lblPasswordStrength->setStyleSheet("color: #2ecc71; font-weight: bold;");
    }
    else if(password.length()>=6 && hasAll)
    {
        ui->lblPasswordStrength->setText("Medium password");
        ui->lblPasswordStrength->setStyleSheet("color: #f39c12; font-weight: bold;");
    }
    else
    {
        ui->lblPasswordStrength->setText("Weak password");
        ui->lblPasswordStrength->setStyleSheet("color: #e74c3c; font-weight: bold;");
    }
}
void ListenerWindow::on_pushButtonBrowse_clicked()
{
    QString fileCover=QFileDialog::getOpenFileName(this,"selectsong","","All Files (*.*)");
    if(!fileCover.isEmpty())
    {
        ui->lineEditPhoto->setText(fileCover);
    }
}