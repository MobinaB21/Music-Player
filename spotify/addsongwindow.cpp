#include "addsongwindow.h"
#include "ui_addsongwindow.h"
#include<QMessageBox>
#include"song.h"
#include"songrepository.h"
AddSongWindow::AddSongWindow(int id,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddSongWindow)
{
    ui->setupUi(this);
    this->artistId=id;
    setAttribute(Qt::WA_DeleteOnClose);
}
AddSongWindow::~AddSongWindow()
{
    delete ui;
}
void AddSongWindow::on_pushButton_3_clicked()
{
    QString name=ui->lineName->text();
    QString album=ui->lineAlbum->text();
    QString audioFileName=ui->lineAudio->text();
    QString genre=ui->lineGenre->text();
    QString releaseYear=ui->lineReleaseYear->text();
    int albumId=0;
    if(name.isEmpty()||album.isEmpty()||audioFileName.isEmpty()||genre.isEmpty()||releaseYear.isEmpty())
    {
        QMessageBox::warning(this,"Please fill all the lines","warning");
        return;
    }
    Song newSong(name.toStdString(),releaseYear.toInt(),genre.toStdString(),audioFileName.toStdString(),0,this->artistId,albumId);
    SongRepository song;
    song.save(newSong);
    this->close();
}
