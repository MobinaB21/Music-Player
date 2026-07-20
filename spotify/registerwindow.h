#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancle_clicked();

    void on_lineEditPassword_textChanged(const QString &arg1);

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
