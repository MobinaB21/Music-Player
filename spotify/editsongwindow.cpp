#include "editsongwindow.h"
#include "ui_editsongwindow.h"

EditSongWindow::EditSongWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditSongWindow)
{
    ui->setupUi(this);
}

EditSongWindow::~EditSongWindow()
{
    delete ui;
}
