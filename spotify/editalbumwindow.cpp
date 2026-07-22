#include "editalbumwindow.h"
#include "ui_editalbumwindow.h"
#include"album.h"
#include<QMessageBox>
#include"albumrepository.h"
#include<QFileDialog>
EditAlbumWindow::EditAlbumWindow(Album target,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditAlbumWindow)
    ,currentAlbum(target)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEdit->setText(QString::fromStdString(currentAlbum.getAlbumName()));
    ui->lineEditCover->setText(QString::fromStdString(currentAlbum.getCover()));
}

EditAlbumWindow::~EditAlbumWindow()
{
    delete ui;
}

void EditAlbumWindow::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    QString albumCover=ui->lineEditCover->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"Please enter the name of album","warning");
        return;
    }
    if(albumCover.isEmpty())albumCover="F:/screan shots/Screenshot 2026-07-21 190410.png";
    AlbumRepository temp;
    temp.removeFromFile(currentAlbum);
    Album updated=currentAlbum;
    updated.setAlbumName(name.toStdString());
    temp.saveToFile(updated);
    updated.setCover(albumCover.toStdString());
    this->close();
}
void EditAlbumWindow::on_pushButtonBrowse_clicked()
{
    QString fileCover=QFileDialog::getOpenFileName(this,"selectCover","","All Files (*.*)");
    if(!fileCover.isEmpty())
    {
        ui->lineEditCover->setText(fileCover);
    }
}

