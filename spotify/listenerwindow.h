#ifndef LISTENERWINDOW_H
#define LISTENERWINDOW_H
#include"loginwindow.h"
#include <QWidget>
#include<QListWidget>
#include<vector>
#include"song.h"
namespace Ui {
class ListenerWindow;
}

class ListenerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ListenerWindow(int listenerId,QWidget *parent = nullptr);
    void setLoginWindow(LoginWindow * lw);
    void refreshSongList();
    void loadArtists();
    void loadPlaylists();
    ~ListenerWindow();

private slots:
    void on_pushButtonLike_clicked();

    void on_pushButtonLogout_clicked();

    void on_pushButtonAddSong_clicked();

    void on_listWidgetArtists_itemClicked(QListWidgetItem *item);

    void on_listWidgetAlbums_itemClicked(QListWidgetItem *item);

    void on_pushButtonAddPlaylist_clicked();

    void on_pushButtonDelete_clicked();

    void on_listWidgetPlaylist_itemClicked(QListWidgetItem *item);

    void on_pushButtonDeleteMusic_clicked();

    void on_pushButtonEdit_clicked();

    void on_pushButtonDelete_2_clicked();

    void on_pushButtonEditPlaylistName_clicked();

    void filterSongs();

    void on_pushButtonSearch_clicked();

    void on_comboSort_currentIndexChanged(int Index);

    void filterMusics();

private:
    Ui::ListenerWindow *ui;
    LoginWindow *login=nullptr;
    int listenerId;
};

#endif // LISTENERWINDOW_H
