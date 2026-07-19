#ifndef EDITSONGWINDOW_H
#define EDITSONGWINDOW_H
#include"song.h"
#include <QWidget>
#include"song.h"
namespace Ui {
class EditSongWindow;
}

class EditSongWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditSongWindow(int id,Song target ,QWidget *parent = nullptr);
    ~EditSongWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EditSongWindow *ui;
    Song currentSong;
    int artistId;
};

#endif // EDITSONGWINDOW_H
