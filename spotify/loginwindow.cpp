#include "loginwindow.h"
#include "ui_loginwindow.h"
#include"artistrepository.h"
#include"listenerrepository.h"
#include<QMessageBox>
#include"artistwindow.h"
#include"listenerwindow.h"
#include"registerwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

}
void LoginWindow::clickLoginButton()
{
    QString username=ui->lineEdit_3->text();
    QString password=ui->lineEdit_2->text();
    ArtistRepository artist;
    optional<Account>account;
    account=artist.searchByUserName(username.toStdString());
    if(!account.has_value())
    {
        ListenerRepository listener;
        account=listener.searchByUserName(username.toStdString());
    }
    if(account.has_value())
    {
        if(account->getPassword()==password.toStdString())
        {
            if(account->getRole()=="Artist")
            {
                ArtistWindow *artWindow=new ArtistWindow();
                artWindow->show();
            }
            else if(account->getRole()=="Listener")
            {
                ListenerWindow *listenWindow=new ListenerWindow();
                listenWindow->show();
            }
            this->hide();
        }
        else
        {
            QMessageBox::warning(this,"Password is incorrect","warning");
            ui->lineEdit_2->clear();
        }
    }
    else QMessageBox::warning(this,"User not found","warning");
}
void LoginWindow::clickRegisterButton()
{
    RegisterWindow *registerWindow=new RegisterWindow();
    registerWindow->show();
    this->hide();
}
LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_2_clicked()
{
    this->clickLoginButton();
}


void LoginWindow::on_pushButton_clicked()
{
    this->clickRegisterButton();
}

