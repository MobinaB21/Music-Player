#include "listenerwindow.h"
#include "ui_listenerwindow.h"
#include<QMessageBox>
#include"songrepository.h"
#include"addsongwindow.h"
#include"editsongwindow.h"
ListenerWindow::ListenerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}
ListenerWindow::~ListenerWindow()
{
    delete ui;
}
void ListenerWindow::setLoginWindow(LoginWindow * lw){ login=lw;}
void ListenerWindow::on_logoutButton_clicked()
{
    if(login)
    {
        login->show();
    }
    this->close();
}
void ListenerWindow::on_deleteButton_clicked()
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
        //refreshSongList();
    }
}
void ListenerWindow::refreshSongList()
{
    ui->listWidget->clear();
    SongRepository temp;
    vector<Song>songs=temp.getByArtist(this->artistId);
    for(const auto& song:songs)
    {
        ui->listWidget->addItem(QString::fromStdString(song.getSongName()));
    }
}
void ListenerWindow::on_pushButton_4_clicked()
{
    AddSongWindow *addWin=new AddSongWindow(this->artistId,this);
    connect(addWin,&AddSongWindow::destroyed,this,&ListenerWindow::refreshSongList);
    addWin->show();
}
void ListenerWindow::on_editButton_clicked()
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
    connect(editWin,&EditSongWindow::destroyed,this,&ListenerWindow::refreshSongList);
    editWin->show();
}
