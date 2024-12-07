#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void on_pushButtonEncrypt_clicked();
    void on_pushButtonDecrypt_clicked();

private:
    Ui::MainWindow *ui;
    QString generateRandomString(int length) const;
    AES *aes;  // AES instance as a member variable
};
#endif // MAINWINDOW_H
