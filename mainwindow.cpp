#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include "formtableau.h"
#include "canvaitem.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lvTableaux->setIconSize(QSize(150,150));
    ui->lvTableaux->setResizeMode(QListWidget::Adjust);
    ui->lvTableaux->setDragEnabled(false);
    connect(ui->lvTableaux, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openCanvas(QListWidgetItem*)));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/003-022.jpg"),"003-022.jpg","../img/003-022.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/e102.jpg"),"e102.jpg","../img/e102.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/pinball.jpg"),"pinball.jpg","../img/pinball.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/tournesol.jpg"),"tournesol.jpg","../img/tournesol.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/tournesol2.jpg"),"tournesol2.jpg","../img/tournesol2.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QIcon("../img/plus.png"),"Nouveau","../img/plus.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openCanvas(QListWidgetItem *item)
{
    canvaItem *itemC = (canvaItem*)item;
    qDebug() << itemC->getText();
    formTableau* tab = new formTableau(0,itemC);
    tab->show();
}
