#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;
private slots:
    void clickLoginButton();
    void clickRegisterButton();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // LOGINWINDOW_H
