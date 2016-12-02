#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFileDialog>
#include <QDir>
#include "formtableau.h"
#include "canvaitem.h"
#include "Config/configholder.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lvTableaux->setWordWrap(true);
    ui->lvTableaux->setIconSize(QSize(150,150));
    ui->lvTableaux->setResizeMode(QListWidget::Adjust);
    ui->lvTableaux->setDragEnabled(false);
    connect(ui->lvTableaux, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openCanvas(QListWidgetItem*)));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/003-022.jpg"),"003-022.jpg","../img/003-022.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/e102.jpg"),"e102.jpg","../img/e102.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/pinball.jpg"),"pinball.jpg","../img/pinball.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/tournesol.jpg"),"tournesol.jpg","../img/tournesol.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/tournesol2.jpg"),"tournesol2.jpg","../img/tournesol2.jpg"));
    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/plus.png"),"Nouveau","../img/plus.png"));
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

void MainWindow::on_actionOuvrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Configuration"), QDir::currentPath(), tr("Configuration Files (*.json)"));
    ConfigHolder hold;
    hold.LoadFromJSONFile(fileName);
    createUIFromConfig(hold);
}

void MainWindow::createUIFromConfig(const ConfigHolder &conf){
    QVector<canvaItem*> canvas = conf.getCanvas();
    ui->lvTableaux->clear();
    foreach (canvaItem *c, canvas) {
        ui->lvTableaux->addItem(c);
    }
}
