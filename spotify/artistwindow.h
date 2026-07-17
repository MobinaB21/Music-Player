#ifndef ARTISTWINDOW_H
#define ARTISTWINDOW_H
#include"loginwindow.h"
#include <QWidget>
#include<QListWidget>
namespace Ui {
class ArtistWindow;
}

class ArtistWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistWindow(int artistId,QWidget *parent = nullptr);
    void setLoginWindow(LoginWindow *lw);
    void refreshSongList();
    void refreshAlbumList();
    void refreshSongsOfAlbum(int albumId);
    ~ArtistWindow();

private slots:
    void on_buttonAddalbum_clicked();

    void on_buttonLogout_clicked();

    void on_buttonAddmusic_clicked();

    void on_buttonDelete_clicked();
    void on_listWidgetSongs_itemClicked(QListWidgetItem *item);

    void on_edit_clicked();

    void on_buttonDelete_2_clicked();

    void on_edit_2_clicked();

    void on_buttonLogout_2_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ArtistWindow *ui;
    LoginWindow *login;
    int artistId;
};

#endif // ARTISTWINDOW_H
