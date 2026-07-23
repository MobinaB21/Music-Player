#include "artistwindow.h"
#include "ui_artistwindow.h"
#include <QMessageBox>
#include "songrepository.h"
#include <vector>
#include "addsongwindow.h"
#include "addalbumwindow.h"
#include "albumrepository.h"
#include "editsongwindow.h"
#include<QListWidgetItem>
#include"editalbumwindow.h"
#include"artistrepository.h"
#include<QFileDialog>
ArtistWindow::ArtistWindow(int artistId,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArtistWindow)
{
    this->artistId=artistId;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->listWidgetSongs, &QListWidget::itemClicked, this, &ArtistWindow::on_listWidgetSongs_itemClicked);
    refreshSongList();
    refreshAlbumList();
    ui->comboGenreFilter->addItem("All");
    ui->comboGenreFilter->addItems({"Pop", "Rock", "Rap", "Jazz", "Classical","Hip Hop"});
    ui->comboYearFilter->addItem("All");
    for(int year=2026;year>=0;year--)
    {
        ui->comboYearFilter->addItem(QString::number(year));
    }
    connect(ui->lineSearch, &QLineEdit::textChanged, this, &ArtistWindow::filterSongs);
    connect(ui->comboGenreFilter, &QComboBox::currentTextChanged, this, &ArtistWindow::filterSongs);
    connect(ui->comboYearFilter, &QComboBox::currentTextChanged, this, &ArtistWindow::filterSongs);
    ArtistRepository tempArtist;
    auto artist=tempArtist.search(artistId);
    ui->lblImage->setFixedSize(30,30);
    QString profilePhoto=QString::fromStdString(artist.value().getProfilePhoto());
    QPixmap pixmap(profilePhoto);
    ui->lblImage->setPixmap(pixmap.scaled(ui->lblImage->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->lblName->setText("Welcom back "+QString::fromStdString(artist.value().getFullName()));
    player=new PlaybackList(this);
    ArtistRepository temp;
    auto currentUser=temp.search(this->artistId);
    ui->lineEditName->setText(QString::fromStdString(currentUser.value().getFullName()));
    ui->lineEditUsername->setText(QString::fromStdString(currentUser.value().getUserName()));
    ui->lineEditPassword->setText(QString::fromStdString(currentUser.value().getPassword()));
    ui->textEdit->setText(QString::fromStdString(currentUser.value().getBiography()));
    ui->lineEditPhoto->setText(QString::fromStdString(currentUser.value().getProfilePhoto()));
}
ArtistWindow::~ArtistWindow()
{
    delete ui;
}
void ArtistWindow::setLoginWindow(LoginWindow * lw) {
    login = lw;
}
void ArtistWindow::on_buttonAddalbum_clicked()
{
    AddAlbumWindow *album = new AddAlbumWindow(this->artistId, nullptr);
    connect(album, &AddAlbumWindow::destroyed, this, [this]() {
        this->refreshAlbumList();
        this->show();
    });
    album->show();
    this->hide();
}
void ArtistWindow::on_buttonDelete_clicked()
{
    QListWidgetItem *target = ui->listWidget_2->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select an album to delete.");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Album", "Do you want to delete this album?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QString targetName=target->text();
        AlbumRepository temp;
        vector<Album> albums = temp.albums(this->artistId);
        SongRepository tmp;
        int id=temp.getIdByName(targetName.toStdString());
        auto aim=temp.search(id);
        vector<Song>albumSongs=tmp.getByAlbum(aim.value().getAlbumId());
        temp.removeFromFile(aim.value());
        for(auto&a:albumSongs)
        {
            tmp.removeFromFile(a);
        }
        refreshSongsOfAlbum(aim.value().getAlbumId());
        refreshAlbumList();
    }
}
void ArtistWindow::on_edit_2_clicked()
{
    QListWidgetItem *target = ui->listWidget_2->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select an album to edit.");
        return;
    }
    QString albumName=target->text();
    AlbumRepository temp;
    vector<Album> albums = temp.albums(this->artistId);
    int albumId=temp.getIdByName(albumName.toStdString());
    auto aim=temp.search(albumId);
    if(!aim.has_value())return;
    EditAlbumWindow *editWin = new EditAlbumWindow(aim.value(), nullptr);
    connect(editWin, &EditAlbumWindow::destroyed, this,[this](){
    this->refreshAlbumList();
    this->show();
    });
    editWin->show();
    this->hide();
}
void ArtistWindow::on_buttonLogout_2_clicked()
{
    on_buttonLogout_clicked();
}
void ArtistWindow::refreshAlbumList()
{
    ui->listWidget_2->clear();
    AlbumRepository temp;
    vector<Album> albums = temp.albums(this->artistId);
    for(auto& a : albums)
    {
        QSize iconSize(70,70);
        ui->listWidget_2->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(a.getCover());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(a.getAlbumName()));
        ui->listWidget_2->addItem(item);
    }
    ui->listWidget_2->sortItems(Qt::AscendingOrder);
}
void ArtistWindow::refreshSongsOfAlbum(int albumId)
{
    ui->listWidget->clear();
    SongRepository temp;
    vector<Song>songs=temp.getByAlbum(albumId);
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
        ui->listWidget->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(s.getSongImage());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(s.getSongName()));
        ui->listWidget->addItem(item);
    }
}
void ArtistWindow::on_buttonAddmusic_clicked()
{
    AddSongWindow *song = new AddSongWindow(this->artistId, nullptr);
    connect(song, &AddSongWindow::destroyed, this, [this]() {
        this->refreshSongList();
        this->show();
    });
    song->show();
    this->hide();
}
void ArtistWindow::on_buttonDelete_2_clicked()
{
    QListWidgetItem *target = ui->listWidgetSongs->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select a song to delete.");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Song", "Do you want to delete this song?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        SongRepository temp;
        QString targetName=target->text();
        Song aim=temp.getByName(targetName.toStdString());
        vector<Song> songs = temp.singleSong(this->artistId);
        temp.removeFromFile(aim);
        refreshSongList();
    }
}
void ArtistWindow::on_edit_clicked()
{
    QListWidgetItem *target = ui->listWidgetSongs->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select a song to edit.");
        return;
    }
    SongRepository temp;
    QString targetName=target->text();
    Song aim=temp.getByName(targetName.toStdString());
    vector<Song> songs = temp.singleSong(this->artistId);
    EditSongWindow *editWin = new EditSongWindow(this->artistId,aim, nullptr);
    connect(editWin, &EditSongWindow::destroyed, this,[this](){
        this->refreshSongList();
        this->show();
    });
    editWin->show();
    this->hide();
}
void ArtistWindow::on_listWidgetSongs_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    SongRepository song;
    Song target = song.getByName(name.toStdString());
    ui->lblGenre->setText(QString::fromStdString(target.getGenre()));
    ui->lblYear->setText(QString::number(target.getReleaseYear()));
}
void ArtistWindow::refreshSongList()
{
    ui->listWidgetSongs->clear();
    SongRepository temp;
    vector<Song> songs = temp.singleSong(this->artistId);
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
    for(const auto& song : songs)
    {
        QSize iconSize(70,70);
        ui->listWidgetSongs->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(song.getSongImage());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(song.getSongName()));
        ui->listWidgetSongs->addItem(item);
    }
}
void ArtistWindow::on_buttonLogout_clicked()
{
    if(login)
    {
        login->show();
    }
    this->close();
}
void ArtistWindow::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    string clickedAlbumName = item->text().toStdString();
    AlbumRepository tempAlbum;
    int albumId = tempAlbum.getIdByName(clickedAlbumName);
    ui->listWidget->clear();
    SongRepository tempSong;
    vector<Song> songs = tempSong.getByAlbum(albumId);
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
    for(const auto& song : songs)
    {
        QSize iconSize(70,70);
        ui->listWidget->setIconSize(iconSize);
        QString coverPath=QString::fromStdString(song.getSongImage());
        QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(song.getSongName()));
        ui->listWidget->addItem(item);
    }
}
void ArtistWindow::on_pushButton_clicked()
{
    QListWidgetItem *target = ui->listWidget->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select a song to delete.");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Song", "Do you want to delete this song?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        AlbumRepository tempAlbum;
        QListWidgetItem * selectedAlbum=ui->listWidget_2->currentItem();
        QString selectedAlbumName=selectedAlbum->text();
        int albumId=tempAlbum.getIdByName(selectedAlbumName.toStdString());
        vector<Album>albums=tempAlbum.albums(this->artistId);
        auto selected=tempAlbum.search(albumId);
        if(!selected.has_value())return;
        SongRepository tempSong;
        QString songName=target->text();
        Song aim=tempSong.getByName(songName.toStdString());
        vector<Song> songs = tempSong.getByAlbum(selected.value().getAlbumId());
        tempSong.removeFromFile(aim);
        refreshSongsOfAlbum(selected.value().getAlbumId());
    }
}
void ArtistWindow::on_pushButton_2_clicked()
{
    QListWidgetItem *target = ui->listWidget->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select a song to edit.");
        return;
    }
     AlbumRepository album;
    QListWidgetItem *selectAlbum=ui->listWidget_2->currentItem();
    QString selectedAlbumName=selectAlbum->text();
    int albumId=album.getIdByName(selectedAlbumName.toStdString());
    vector<Album>albums=album.albums(this->artistId);
    auto selected=album.search(albumId);
    SongRepository temp;
    QString targetName=target->text();
    Song aim=temp.getByName(targetName.toStdString());
    vector<Song> songs = temp.getByAlbum(albumId);
    EditSongWindow *editWin = new EditSongWindow(this->artistId,aim, nullptr);
    connect(editWin, &EditSongWindow::destroyed, this, [this, albumId]() {
        this->refreshSongsOfAlbum(albumId);
        refreshSongList();
        this->show();
    });
    editWin->show();
    this->hide();
}
void ArtistWindow::on_pushButtonEdit_clicked()
{
    ArtistRepository temp;
    QString name=ui->lineEditName->text();
    QString userName=ui->lineEditUsername->text();
    QString password=ui->lineEditPassword->text();
    QString biography=ui->textEdit->toPlainText();
    QString profilePhoto=ui->lineEditPhoto->text();
    string name2=name.toStdString();
    string userName2=userName.toStdString();
    string password2=password.toStdString();
    string biography2=biography.toStdString();
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
    auto find=temp.searchByUserName(userName2);
    if(find && find.value().getId()!=artistId)
    {
        QMessageBox::critical(this,"warning","This username was selected before");
        return;
    }
    if(profilePhoto.isEmpty())profilePhoto="F:/screan shots/Screenshot 2026-07-21 204414.png";
    temp.updateArtist(this->artistId,name2,userName2,password2,biography2,profilePhoto.toStdString());
    auto it=temp.search(this->artistId);
    temp.saveToFile(it.value());
    ArtistRepository tempArtist;
    auto artist=tempArtist.search(artistId);
    ui->lblImage->setFixedSize(30,30);
    QString photo=QString::fromStdString(artist.value().getProfilePhoto());
    QPixmap pixmap(photo);
    ui->lblImage->setPixmap(pixmap.scaled(ui->lblImage->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QMessageBox::information(this,"Success","Account updated successfully");
}
void ArtistWindow::on_pushButtonDelete_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete Account","Do you want to delete your account ?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::No)return;
    ArtistRepository temp;
    AlbumRepository tempAlbum;
    SongRepository tempSong;
    vector<Song>songs=tempSong.getByArtist(this->artistId);
    vector<Album>albums=tempAlbum.albums(this->artistId);
    for(auto&s:songs)
    {
        tempSong.removeFromFile(s);
    }
    for(auto&a:albums)
    {
        tempAlbum.removeFromFile(a);
    }
    temp.removeFromFile(this->artistId);
    QMessageBox::information(this,"Delete Account","Your account was deleted successfully");
    if(login)
    {
        login->show();
    }
    this->close();
}
void ArtistWindow::filterSongs()
{
    QString find=ui->lineSearch->text();
    if(find.isEmpty())
    {
        ui->listWidgetSearch->clear();
        return;
    }
    SongRepository tempSong;
    vector<Song>currentSongs=tempSong.getByArtist(this->artistId);
    ui->listWidgetSearch->clear();
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
            ui->listWidgetSearch->setIconSize(iconSize);
            QString coverPath=QString::fromStdString(song.getSongImage());
            QListWidgetItem *item=new QListWidgetItem(QIcon(coverPath),QString::fromStdString(song.getSongName()));
            ui->listWidgetSearch->addItem(item);
        }
    }
}
void ArtistWindow::on_comboSort_currentIndexChanged(int index)
{
    refreshSongList();
    filterSongs();
}
void ArtistWindow::on_pushButtonPlay_clicked()
{
    player->play();
}
void ArtistWindow::on_pushButtonPause_clicked()
{
    player->pause();
}
void ArtistWindow::on_pushButtonNextSong_clicked()
{
    player->next();
}
void ArtistWindow::on_pushButtonPrevious_clicked()
{
    player->previous();
}
void ArtistWindow::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    QString albumName=item->text();
    AlbumRepository tempAlbum;
    int albumId=tempAlbum.getIdByName(albumName.toStdString());
    SongRepository tempSong;
    vector<Song>albumSongs=tempSong.getByAlbum(albumId);
    vector<Song>tempAlbumSongs=albumSongs;
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
void ArtistWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString songName=item->text();
    SongRepository tempSong;
    auto song=tempSong.getSongByName(songName.toStdString());
    if(!song.has_value())return;
    player->playSong(song.value());
}
void ArtistWindow::on_pushButtonPlay_2_clicked()
{
    player->play();
}
void ArtistWindow::on_pushButtonPause_2_clicked()
{
    player->pause();
}
void ArtistWindow::on_listWidgetSongs_itemDoubleClicked(QListWidgetItem *item)
{
    QString songName=item->text();
    SongRepository tempSong;
    auto song=tempSong.getSongByName(songName.toStdString());
    if(!song.has_value())return;
    player->playSong(song.value());
}
void ArtistWindow::on_lineEditPassword_textChanged(const QString &password)
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
void ArtistWindow::on_pushButtonBrowse_clicked()
{
    QString fileCover=QFileDialog::getOpenFileName(this,"selectsong","","All Files (*.*)");
    if(!fileCover.isEmpty())
    {
        ui->lineEditPhoto->setText(fileCover);
    }
}