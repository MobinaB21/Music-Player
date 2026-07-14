#include "editalbumwindow.h"
#include "ui_editalbumwindow.h"
#include"album.h"
#include<QMessageBox>
#include"albumrepository.h"
EditAlbumWindow::EditAlbumWindow(Album target,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditAlbumWindow)
    ,currentAlbum(target)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QString::fromStdString(currentAlbum.getAlbumName()));
}

EditAlbumWindow::~EditAlbumWindow()
{
    delete ui;
}

void EditAlbumWindow::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"Please enter the name of album","warning");
        return;
    }
    AlbumRepository temp;
    temp.removeFromFile(currentAlbum);
    Album updated=currentAlbum;
    updated.setAlbumName(name.toStdString());
    temp.saveToFile(updated);
    this->close();
}

