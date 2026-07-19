#include "addsongwindow.h"
#include "ui_addsongwindow.h"
#include<QMessageBox>
#include"song.h"
#include"songrepository.h"
#include"albumrepository.h"
AddSongWindow::AddSongWindow(int id,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddSongWindow)
{
    ui->setupUi(this);
    this->artistId=id;
    setAttribute(Qt::WA_DeleteOnClose);
     AlbumRepository tempAlbum;
    vector<Album>albums=tempAlbum.albums(this->artistId);
    ui->comboBox->addItem("default");
    for(auto&a:albums)
    {
        ui->comboBox->addItem(QString::fromStdString(a.getAlbumName()));
    }
}
AddSongWindow::~AddSongWindow()
{
    delete ui;
}
void AddSongWindow::on_pushButton_3_clicked()
{
    AlbumRepository tempAlbum;
    QString name=ui->lineName->text();
    QString audioFileName=ui->lineAudio->text();
    QString genre=ui->lineGenre->text();
    QString releaseYear=ui->lineReleaseYear->text();

    if(name.isEmpty()||audioFileName.isEmpty()||genre.isEmpty()||releaseYear.isEmpty())
    {
        QMessageBox::warning(this,"Please fill all the lines","warning");
        return;
    }
    int albumId=0;
    if(ui->comboBox->currentIndex()!=0)
    {
        QString album=ui->comboBox->currentText();
        albumId=tempAlbum.getIdByName(album.toStdString());
    }
    Song newSong(name.toStdString(),releaseYear.toInt(),genre.toStdString(),audioFileName.toStdString(),0,this->artistId,albumId);
    SongRepository song;
    song.save(newSong);
    this->close();
}

