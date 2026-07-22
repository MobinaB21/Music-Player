#include "addsongwindow.h"
#include "ui_addsongwindow.h"
#include<QMessageBox>
#include"song.h"
#include"songrepository.h"
#include"albumrepository.h"
#include<QFileDialog>
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
    QString filePath=ui->linePath->text();
    QString songCover=ui->lineSongCover->text();

    if(name.isEmpty()||audioFileName.isEmpty()||genre.isEmpty()||releaseYear.isEmpty() || filePath.isEmpty())
    {
        QMessageBox::warning(this,"warning","Please fill all the lines");
        return;
    }
    int albumId=0;
    if(ui->comboBox->currentIndex()!=0)
    {
        QString album=ui->comboBox->currentText();
        albumId=tempAlbum.getIdByName(album.toStdString());
    }
    if(songCover.isEmpty())songCover="F:/screan shots/Screenshot 2026-07-21 185330.png";
    Song newSong(name.toStdString(),releaseYear.toInt(),genre.toStdString(),audioFileName.toStdString(),0,this->artistId,albumId,filePath.toStdString(),songCover.toStdString());
    SongRepository song;
    song.save(newSong);
    this->close();
}
void AddSongWindow::on_pushButtonBrowse_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,"selectsong","","All Files (*.*)");
    if(!filePath.isEmpty())
    {
        ui->linePath->setText(filePath);
    }
}
void AddSongWindow::on_pushButtonBrowse_2_clicked()
{
    QString fileCover=QFileDialog::getOpenFileName(this,"selectCover","","All Files (*.*)");
    if(!fileCover.isEmpty())
    {
        ui->lineSongCover->setText(fileCover);
    }
}

