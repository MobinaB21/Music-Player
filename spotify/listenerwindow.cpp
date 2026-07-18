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
ListenerWindow::ListenerWindow(int listenerId,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenerWindow)

{
    ui->setupUi(this);
    ui->comboGenreFilter->addItem("All");
     ui->comboGenreFilter->addItems({"Pop", "Rock", "Rap", "Jazz", "Classical"});
     ui->comboYearFilter->addItem("All");
    for(int year=2026;year>=0;year--)
    {
         ui->comboYearFilter->addItem(QString::number(year));
    }
    connect(ui->lineSearch, &QLineEdit::textChanged, this, &ListenerWindow::filterSongs);
    connect(ui->comboGenreFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterSongs);
    connect(ui->comboYearFilter, &QComboBox::currentTextChanged, this, &ListenerWindow::filterSongs);
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
    ui->lblName->setText("Welcom back "+QString::fromStdString(listener.value().getFullName()));
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
    sort(allArtists.begin(),allArtists.end(),[](const Account& a,const Account&b)
        {
        return a.getFullName()<b.getFullName();
    });
    for(auto&a:allArtists)
    {
        QListWidgetItem *item=new QListWidgetItem(QString::fromStdString(a.getFullName()));
        ui->listWidgetArtists->addItem(item);
    }
}
void ListenerWindow::loadPlaylists()
{
    ui->listWidgetPlaylist->clear();
    PlaylistRepository playlistRepo;
    vector<Playlist>allPlaylists=playlistRepo.playlists(this->listenerId);
    ui->listWidgetPlaylist->addItem("Favorites");
    sort(allPlaylists.begin(), allPlaylists.end(), [](const Playlist& a, const Playlist& b) {
        string name1 = a.getName();
        string name2 = b.getName();
        transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
        return name1 < name2;
    });
    for(auto&a:allPlaylists)
    {
        ui->listWidgetPlaylist->addItem(QString::fromStdString(a.getName()));
    }
}
void ListenerWindow::on_pushButtonLike_clicked()
{
    QListWidgetItem *selectedSong=ui->listWidget_3->currentItem();
    if(!selectedSong)
    {
        QMessageBox::warning(this,"warning","Please select a song ");
        return;
    }
    QListWidgetItem *selectedArtist=ui->listWidgetArtists->currentItem();
    QListWidgetItem *selectedAlbum=ui->listWidgetAlbums->currentItem();
    int row=ui->listWidget_3->row(selectedSong);
    ArtistRepository temp1;
    QString artistName=selectedArtist->text();
    int artistId=temp1.getIdByName(artistName.toStdString());
    AlbumRepository temp2;
    vector<Album>albumList=temp2.albums(artistId);
    QString albumName=selectedAlbum->text();
    int albumId=temp2.getIdByName(albumName.toStdString());
    SongRepository temp3;
    vector<Song>songs=temp3.getByAlbum(albumId);
    sort(songs.begin(),songs.end(),[]( Song&a,Song&b)
         {
             return a.getSongName()<b.getSongName();
         });
    Song aim=songs[row];
    int foundIndex = -1;
    for (size_t i = 0; i < SongRepository::likedSongs.size(); i++) {
        if (SongRepository::likedSongs[i].first == this->listenerId && SongRepository::likedSongs[i].second == aim.getSongId()) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex != -1)QMessageBox::information(this, "Favorites","You liked this song before");
    else
    {
        SongRepository::likedSongs.push_back({this->listenerId, aim.getSongId()});
        SongRepository::favorite.push_back(aim);
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
    QListWidgetItem *selectedSong=ui->listWidget_3->currentItem();
    if(!selectedSong)
    {
        QMessageBox::warning(this,"warning","Please select one of the songs");
        return;
    }
    QListWidgetItem *selectedArtist=ui->listWidgetArtists->currentItem();
    QListWidgetItem *selectedAlbum=ui->listWidgetAlbums->currentItem();
    int row=ui->listWidget_3->row(selectedSong);
    ArtistRepository temp1;
    QString artistName=selectedArtist->text();
    int artistId=temp1.getIdByName(artistName.toStdString());
    AlbumRepository temp2;
    vector<Album>albumList=temp2.albums(artistId);
    QString albumName=selectedAlbum->text();
    int albumId=temp2.getIdByName(albumName.toStdString());
    SongRepository temp3;
    vector<Song>songs=temp3.getByAlbum(albumId);
    sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        string str1 = a.getSongName();
        string str2 = b.getSongName();
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
        return str1 < str2;
    });
    Song aim=songs[row];
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
        if(p.getName()!="Favorit Songs")
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
        SongRepository::playlistSongs.push_back({id,aim.getSongId()});
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
    if(!singleSongs.empty())albumNames.push_back("Singles");
    sort(albumNames.begin(), albumNames.end(), [](const string& a, const string& b) {
        string str1 = a, str2 = b;
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
        return str1 < str2;
    });
    for(auto& a:albumNames)
    {
        ui->listWidgetAlbums->addItem(QString::fromStdString(a));
    }
}
void ListenerWindow::on_listWidgetAlbums_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_3->clear();
    string albumName=item->text().toStdString();
    AlbumRepository album;
    int albumId=album.getIdByName(albumName);
    SongRepository temp;
    vector<Song> songs=temp.getByAlbum(albumId);
    sort(songs.begin(), songs.end(), [](const Song& a, const Song& b) {
        string str1 = a.getSongName();
        string str2 = b.getSongName();
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
        return str1 < str2;
    });
    for(auto&s:songs)
    {
        ui->listWidget_3->addItem(QString::fromStdString(s.getSongName()));
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
    QString itemName=item->text();
    vector<string>favoriteSongNames;
    if(itemName=="Favorites")
    {
        for(auto&f:SongRepository::favorite)
        {
            favoriteSongNames.push_back(f.getSongName());
        }
        sort(favoriteSongNames.begin(), favoriteSongNames.end(), [](const string& a, const string& b) {
            string str1 = a, str2 = b;
            transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
            transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
            return str1 < str2;
        });
        for(auto&f:favoriteSongNames)
        {
            ui->listWidgetMusic->addItem(QString::fromStdString(f));
        }
        return;
    }
     vector<string>songNames;
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
        songNames.push_back(target.value().getSongName());
    }
    sort(songNames.begin(), songNames.end(), [](const string& a, const string& b) {
        string str1 = a, str2 = b;
        transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
        transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
        return str1 < str2;
    });
    for(auto&s:songNames)
    {
        ui->listWidgetMusic->addItem(QString::fromStdString(s));
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
    auto find=temp.searchByUserName(userName2);
    if(find)
    {
        QMessageBox::critical(this,"warning","This username was selected before");
        return;
    }
    temp.updateListener(this->listenerId,name2,userName2,password2,biography2);
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
            ui->listWidget_3->addItem(songName);
        }
    }
}
void ListenerWindow::on_pushButtonSearch_clicked()
{

    filterSongs();
}

