#ifndef LISTENERWINDOW_H
#define LISTENERWINDOW_H
#include"loginwindow.h"
#include <QWidget>
#include<QListWidget>
#include"playbacklist.h"
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

    void on_pushButtonPlay_clicked();

    void on_pushButtonPause_clicked();

    void on_pushButtonNextSong_clicked();

    void on_pushButtonPrevious_clicked();

    void on_listWidgetPlaylist_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidgetMusic_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButtonPlay_2_clicked();

    void on_pushButtonPause_2_clicked();

    void on_pushButtonNextSong_2_clicked();

    void on_pushButtonPrevious_2_clicked();

    void on_listWidgetAlbums_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_3_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::ListenerWindow *ui;
    LoginWindow *login=nullptr;
    int listenerId;
    PlaybackList *player;
};

#endif // LISTENERWINDOW_H
