#ifndef ADDSONGWINDOW_H
#define ADDSONGWINDOW_H

#include <QWidget>

namespace Ui {
class AddSongWindow;
}

class AddSongWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AddSongWindow(int id,QWidget *parent = nullptr);
    ~AddSongWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButtonBrowse_clicked();

    void on_pushButtonBrowse_2_clicked();

private:
    Ui::AddSongWindow *ui;
    int artistId;
};

#endif // ADDSONGWINDOW_H
