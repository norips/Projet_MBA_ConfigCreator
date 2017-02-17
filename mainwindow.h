#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "Config/configholder.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openCanvas(QListWidgetItem *);
    void on_actionOuvrir_triggered();
    void reDraw();
    void on_actionEnregistrer_triggered();

private:
    Ui::MainWindow *ui;

    void createUIFromConfig(const ConfigHolder& conf);
};

#endif // MAINWINDOW_H
