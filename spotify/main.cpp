#include"loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow *loginPage=new LoginWindow();
    loginPage->show();
    return QApplication::exec();
}
