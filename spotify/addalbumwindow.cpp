#include "addalbumwindow.h"
#include "ui_addalbumwindow.h"
#include<QMessageBox>
#include"album.h"
#include"albumrepository.h"
AddAlbumWindow::AddAlbumWindow(int id ,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddAlbumWindow)
{
    ui->setupUi(this);
    this->artistId=id;
    setAttribute(Qt::WA_DeleteOnClose);
}

AddAlbumWindow::~AddAlbumWindow()
{
    delete ui;
}

void AddAlbumWindow::on_pushButton_clicked()
{
    QString name=ui->albumName->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"warning","You shoud enter name of album");
        return;
    }
    Album newAlbum(name.toStdString(),0,artistId);
    AlbumRepository album;
    album.save(newAlbum);
    this->close();

}

