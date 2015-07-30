#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qprogressdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool ExecDosCmd(QString cmd);
    void disPlay();
    void getTopic();
private slots:
    void on_zkStart_clicked();

    void on_pushButton_13_clicked();

    void on_consumer_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::MainWindow *ui;
    QStringList topics;
    QProgressDialog  *progressDialog;
};

#endif // MAINWINDOW_H
