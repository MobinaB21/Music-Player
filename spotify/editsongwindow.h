#ifndef EDITSONGWINDOW_H
#define EDITSONGWINDOW_H
#include"song.h"
#include <QWidget>

namespace Ui {
class EditSongWindow;
}

class EditSongWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditSongWindow(Song target ,QWidget *parent = nullptr);
    ~EditSongWindow();

private:
    Ui::EditSongWindow *ui;
};

#endif // EDITSONGWINDOW_H
