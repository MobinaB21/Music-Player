#include "artistwindow.h"
#include "ui_artistwindow.h"
#include<QMessageBox>
#include"songrepository.h"
#include<vector>
#include"addsongwindow.h"
#include"addalbumwindow.h"
#include"albumrepository.h"
#include"editsongwindow.h"
ArtistWindow::ArtistWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArtistWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->listWidget,&QListWidget::itemClicked,this,&ArtistWindow::on_listWidget_itemClicked);
}
ArtistWindow::~ArtistWindow()
{
    delete ui;
}
void ArtistWindow::setLoginWindow(LoginWindow * lw){ login=lw;}

void ArtistWindow::on_buttonAddalbum_clicked()
{
    AddAlbumWindow *album=new AddAlbumWindow(this->artistId);
    album->show();
    connect(album,&AddAlbumWindow::destroyed,this,&ArtistWindow::refreshAlbumList);
}
void ArtistWindow::on_buttonLogout_clicked()
{
    if(login)
    {
        login->show();
    }
    this->close();
}
void ArtistWindow::on_buttonAddmusic_clicked()
{
    AddSongWindow *song=new AddSongWindow(this->artistId);
    song->show();
    connect(song,&AddSongWindow::destroyed,this,&ArtistWindow::refreshSongList);
}
void ArtistWindow::on_buttonDelete_clicked()
{
    QListWidgetItem *target=ui->listWidget->currentItem();
    if(!target)
    {
        QMessageBox::warning(this,"Please select a song","Warning");
        return;
    }
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete","Do you want to delete ?",QMessageBox::Yes | QMessageBox::No );
    if(reply==QMessageBox::Yes)
    {
        int row=ui->listWidget->row(target);
        SongRepository temp;
        vector<Song>songs=temp.getByArtist(this->artistId);
        Song aim=songs[row];
        temp.removeFromFile(aim);
        delete target;
        refreshSongList();
    }
}
void ArtistWindow::refreshSongList()
{
    ui->listWidget->clear();
    SongRepository temp;
    vector<Song>songs=temp.getByArtist(this->artistId);
    for(const auto& song:songs)
    {
        ui->listWidget->addItem(QString::fromStdString(song.getSongName()));
    }
}
void ArtistWindow::refreshAlbumList()
{
    ui->listWidget->clear();
    AlbumRepository temp;
    vector<Album>albums=temp.albums(this->artistId);
    for(auto&a:albums)
    {
        ui->listWidget->addItem(QString::fromStdString(a.getAlbumName()));
    }
}
void ArtistWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString name=item->text();
    SongRepository song;
    Song target=song.getByName(name.toStdString());
    ui->lblGenre->setText(QString::fromStdString(target.getGenre()));
    ui->lblYear->setText(QString::number(target.getReleaseYear()));
}
void ArtistWindow::on_edit_clicked()
{
    QListWidgetItem *target=ui->listWidget->currentItem();
    if(!target)
    {
        QMessageBox::warning(this,"warning","Please select one of the item");
        return;
    }
    int row=ui->listWidget->row(target);
    SongRepository temp;
    vector<Song>songs=temp.getByArtist(this->artistId);
    Song aim=songs[row];
    EditSongWindow *editWin=new EditSongWindow(aim,this);
    connect(editWin,&EditSongWindow::destroyed,this,&ArtistWindow::refreshSongList);
    editWin->show();
}