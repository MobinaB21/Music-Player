#include "editsongwindow.h"
#include "ui_editsongwindow.h"
#include<QMessageBox>
#include"songrepository.h"
#include"albumrepository.h"
EditSongWindow::EditSongWindow(int id,Song target,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditSongWindow)
    , currentSong(target)
{
    ui->setupUi(this);
    this->artistId=id;
    setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEditName->setText(QString::fromStdString(currentSong.getSongName()));
    ui->lineEditYear->setText(QString::number(currentSong.getReleaseYear()));
    ui->lineEditAudio->setText(QString::fromStdString(currentSong.getAudioFile()));
    ui->lineEditGenre->setText(QString::fromStdString(currentSong.getGenre()));
    AlbumRepository tempAlbum;
    vector<Album>albums=tempAlbum.albums(this->artistId);
    ui->comboBox->addItem("default");
    for(auto&a:albums)
    {
        ui->comboBox->addItem(QString::fromStdString(a.getAlbumName()));
    }
}

EditSongWindow::~EditSongWindow()
{
    delete ui;
}

void EditSongWindow::on_pushButton_clicked()
{
    QString name =ui->lineEditName->text();
    QString year=ui->lineEditYear->text();
    QString genre=ui->lineEditGenre->text();
    QString audio=ui->lineEditAudio->text();
    if(name.isEmpty() || year.isEmpty() || genre.isEmpty() || audio.isEmpty())
    {
        QMessageBox::warning(this,"Please fill al the lines","warning");
        return;
    }
    AlbumRepository tempAlbum;
    int albumId=0;
    if(ui->comboBox->currentIndex()!=0)
    {
        QString album=ui->comboBox->currentText();
        albumId=tempAlbum.getIdByName(album.toStdString());
    }
    SongRepository temp;
    temp.removeFromFile(currentSong);
    Song updated=currentSong;
    updated.setSongName(name.toStdString());
    updated.setReleaseYear(year.toInt());
    updated.setGenre(genre.toStdString());
    updated.setAudioFileName(audio.toStdString());
    updated.setAlbumId(albumId);
    temp.saveToFile(updated);
    this->close();
}
