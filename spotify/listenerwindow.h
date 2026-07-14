#ifndef LISTENERWINDOW_H
#define LISTENERWINDOW_H
#include"loginwindow.h"
#include <QWidget>

namespace Ui {
class ListenerWindow;
}

class ListenerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ListenerWindow(QWidget *parent = nullptr);
    void setLoginWindow(LoginWindow * lw);
    void refreshSongList();
    ~ListenerWindow();

private slots:
    void on_logoutButton_clicked();

    void on_deleteButton_clicked();

    void on_editButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ListenerWindow *ui;
    LoginWindow *login;
    int artistId;
};

#endif // LISTENERWINDOW_H
