#include "registerwindow.h"
#include "ui_registerwindow.h"
#include<QMessageBox>
#include"artistrepository.h"
#include"listenerrepository.h"
#include"loginwindow.h"
#include<QRegularExpression>
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
        QMessageBox::warning(this,"warning","Enter your information");
        return ;
    }
    if(name.contains("&") || username.contains("&") || password.contains("&") || biography.contains("&"))
    {
        QMessageBox::warning(this,"Invalid character","You cant use character '&' ");
        return;
    }
    if(ui->lblPasswordStrength->text().contains("Weak"))
    {
        QMessageBox::warning(this,"warning","Password must contain at least 12 characters long and including at least one uppercase letter, one lowercase letter,one digit and one special charecter");
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


void RegisterWindow::on_lineEditPassword_textChanged(const QString &password)
{
    if(password.isEmpty())
    {
        ui->lblPasswordStrength->setText("");
        return;
    }
    bool hasLower=password.contains(QRegularExpression("[a-z]"));
    bool hasUpper=password.contains(QRegularExpression("[A-Z]"));
    bool hasDigit=password.contains(QRegularExpression("[0-9]"));
    bool hasSpecial=password.contains(QRegularExpression("[^a-zA-Z0-9]"));
    bool hasAll= hasLower && hasUpper && hasDigit && hasSpecial;
    if(password.length()>=12 && hasAll)
    {
        ui->lblPasswordStrength->setText("Strong password");
        ui->lblPasswordStrength->setStyleSheet("color: #2ecc71; font-weight: bold;");
    }
    else if(password.length()>=6 && hasAll)
    {
        ui->lblPasswordStrength->setText("Medium password");
        ui->lblPasswordStrength->setStyleSheet("color: #f39c12; font-weight: bold;");
    }
    else
    {
        ui->lblPasswordStrength->setText("Weak password");
        ui->lblPasswordStrength->setStyleSheet("color: #e74c3c; font-weight: bold;");
    }
}