#include "addalbumwindow.h"
#include "ui_addalbumwindow.h"
#include<QMessageBox>
#include"album.h"
#include"albumrepository.h"
#include<QFileDialog>
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
    QString albumCover=ui->lineEditCover->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"warning","You shoud enter name of album");
        return;
    }
    if(albumCover.isEmpty())albumCover="F:/screan shots/Screenshot 2026-07-21 190410.png";
    Album newAlbum(name.toStdString(),0,artistId,albumCover.toStdString());
    AlbumRepository album;
    album.save(newAlbum);
    this->close();
}
void AddAlbumWindow::on_pushButtonBrowse_clicked()
{
    QString fileCover=QFileDialog::getOpenFileName(this,"selectsong","","All Files (*.*)");
    if(!fileCover.isEmpty())
    {
        ui->lineEditCover->setText(fileCover);
    }
}

