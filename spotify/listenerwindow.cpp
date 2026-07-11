#include "listenerwindow.h"
#include "ui_listenerwindow.h"

ListenerWindow::ListenerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

ListenerWindow::~ListenerWindow()
{
    delete ui;
}
