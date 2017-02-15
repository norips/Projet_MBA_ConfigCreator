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
//    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/003-022.jpg"),"003-022.jpg","../img/003-022.jpg"));
//    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/e102.jpg"),"e102.jpg","../img/e102.jpg"));
//    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/pinball.jpg"),"pinball.jpg","../img/pinball.jpg"));
//    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/tournesol.jpg"),"tournesol.jpg","../img/tournesol.jpg"));
//    ui->lvTableaux->addItem(new canvaItem(QPixmap("../img/tournesol2.jpg"),"tournesol2.jpg","../img/tournesol2.jpg"));
//    ui->lvTableaux->addItem(new QListWidgetItem(QIcon(QPixmap("../img/plus.png")),"Nouveau"));
    ConfigHolder& hold = ConfigHolder::Instance();
    hold.addEmpty();
    createUIFromConfig(hold);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openCanvas(QListWidgetItem *item)
{
    canvaItem *itemC = (canvaItem*)item;
    if(itemC->getText() == "Nouveau"){
        ConfigHolder::Instance().addEmpty();
        createUIFromConfig(ConfigHolder::Instance());
    } else {
        formTableau *tab = new formTableau(0,itemC);
        tab->exec();
    }
    reDraw();
}

void MainWindow::reDraw(){
    qDebug() << "redraw" << endl;
    ConfigHolder& hold = ConfigHolder::Instance();
    createUIFromConfig(hold);
}

void MainWindow::on_actionOuvrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Configuration"), QDir::currentPath(), tr("Configuration Files (*.json)"));
    ConfigHolder& hold = ConfigHolder::Instance();
    hold.LoadFromJSONFile(fileName);
    createUIFromConfig(hold);
}

void MainWindow::createUIFromConfig(const ConfigHolder &conf){
    QVector<Canva*> canvas = conf.getCanvas();
    ui->lvTableaux->clear();
    foreach (Canva *c, canvas) {
        ui->lvTableaux->addItem(c->toItem());
    }
    ui->lvTableaux->repaint();
}
