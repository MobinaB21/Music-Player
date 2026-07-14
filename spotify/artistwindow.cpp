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
ArtistWindow::ArtistWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArtistWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->listWidgetSongs, &QListWidget::itemClicked, this, &ArtistWindow::on_listWidgetSongs_itemClicked);
    refreshSongList();
    refreshAlbumList();
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
    AddAlbumWindow *album = new AddAlbumWindow(this->artistId, this);
    connect(album, &AddAlbumWindow::destroyed, this, &ArtistWindow::refreshAlbumList);
    album->show();
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
        int row = ui->listWidget_2->row(target);
        AlbumRepository temp;
        vector<Album> albums = temp.albums(this->artistId);
        Album aim = albums[row];
        refreshAlbumList();
    }
}
void ArtistWindow::on_edit_2_clicked()
{
    QMessageBox::information(this, "Edit Album", "Album editing feature can be implemented here!");
    QListWidgetItem *target = ui->listWidget_2->currentItem();
    if(!target)
    {
        QMessageBox::warning(this, "Warning", "Please select an album to edit.");
        return;
    }
    int row = ui->listWidget_2->row(target);
    AlbumRepository temp;
    vector<Album> albums = temp.albums(this->artistId);
    Album aim = albums[row];
    EditAlbumWindow *editWin = new EditAlbumWindow(aim, this);
    connect(editWin, &EditAlbumWindow::destroyed, this, &ArtistWindow::refreshAlbumList);
    editWin->show();
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
        ui->listWidget_2->addItem(QString::fromStdString(a.getAlbumName()));
    }
}
void ArtistWindow::on_buttonAddmusic_clicked()
{
    AddSongWindow *song = new AddSongWindow(this->artistId, this);
    connect(song, &AddSongWindow::destroyed, this, &ArtistWindow::refreshSongList);
    song->show();
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
        int row = ui->listWidgetSongs->row(target);
        SongRepository temp;
        vector<Song> songs = temp.getByArtist(this->artistId);
        Song aim = songs[row];
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
    int row = ui->listWidgetSongs->row(target);
    SongRepository temp;
    vector<Song> songs = temp.getByArtist(this->artistId);
    Song aim = songs[row];
    EditSongWindow *editWin = new EditSongWindow(aim, this);
    connect(editWin, &EditSongWindow::destroyed, this, &ArtistWindow::refreshSongList);
    editWin->show();
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
    vector<Song> songs = temp.getByArtist(this->artistId);
    for(const auto& song : songs)
    {
        ui->listWidgetSongs->addItem(QString::fromStdString(song.getSongName()));
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