#include "editsongwindow.h"
#include "ui_editsongwindow.h"
#include<QMessageBox>
#include"songrepository.h"
EditSongWindow::EditSongWindow(Song target,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditSongWindow)
    , currentSong(target)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEditName->setText(QString::fromStdString(currentSong.getSongName()));
    ui->lineEditYear->setText(QString::number(currentSong.getReleaseYear()));
    //ui->lineEditAlbumName->setText(QString::fromStdString(currentSong.getAlbumName()))
    ui->lineEditAudio->setText(QString::fromStdString(currentSong.getAudioFile()));
    ui->lineEditGenre->setText(QString::fromStdString(currentSong.getGenre()));
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
    QString albumName=ui->lineEditAlbumName->text();
    if(name.isEmpty() || year.isEmpty() || genre.isEmpty() || audio.isEmpty())
    {
        QMessageBox::warning(this,"Please fill al the lines","warning");
        return;
    }
    SongRepository temp;
    temp.removeFromFile(currentSong);
    Song updated=currentSong;
    updated.setSongName(name.toStdString());
    updated.setReleaseYear(year.toInt());
    updated.setGenre(genre.toStdString());
    updated.setAudioFileName(audio.toStdString());
    temp.saveToFile(updated);
    this->close();
}
