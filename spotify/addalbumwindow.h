#ifndef ADDALBUMWINDOW_H
#define ADDALBUMWINDOW_H

#include <QWidget>

namespace Ui {
class AddAlbumWindow;
}

class AddAlbumWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddAlbumWindow(int id,QWidget *parent = nullptr);
    ~AddAlbumWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddAlbumWindow *ui;
    int artistId;
};

#endif // ADDALBUMWINDOW_H
