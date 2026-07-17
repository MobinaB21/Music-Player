#include "registerwindow.h"
#include "ui_registerwindow.h"
#include<QMessageBox>
#include"artistrepository.h"
#include"listenerrepository.h"
#include"loginwindow.h"
RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButtonOK_clicked()
{
    QString name=ui->lineEditName->text();
    QString username=ui->lineEditUserName->text();
    QString password=ui->lineEditPassword->text();
    QString biography=ui->textEdit->toPlainText();
    QString role=ui->radioButtonArtist->isChecked()? "Artist":"Listener";
    if(name.isEmpty() || username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"Enter your information","warning");
        return ;
    }
     ArtistRepository artist;
    ListenerRepository listener;
     if(artist.searchByUserName(username.toStdString())||listener.searchByUserName(username.toStdString()))
    {
        QMessageBox::critical(this,"warning","This username was selected before");
        return;
     }
    Account account(name.toStdString(),username.toStdString(),biography.toStdString(),0,role.toStdString(),password.toStdString());
    if(role=="Artist")artist.save(account);
    else listener.save(account);
    QMessageBox::information(this,"Register","Successfully added");
    this->close();
    LoginWindow *lw=new LoginWindow();
    lw->show();
}
void RegisterWindow::on_pushButtonCancle_clicked()
{
    this->close();
    LoginWindow *lw= new LoginWindow();
    lw->show();
}

