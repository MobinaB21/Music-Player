#ifndef EDITALBUMWINDOW_H
#define EDITALBUMWINDOW_H
#include"album.h"
#include <QWidget>

namespace Ui {
class EditAlbumWindow;
}

class EditAlbumWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditAlbumWindow(Album target,QWidget *parent = nullptr);
    ~EditAlbumWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EditAlbumWindow *ui;
    Album currentAlbum;
};

#endif // EDITALBUMWINDOW_H
