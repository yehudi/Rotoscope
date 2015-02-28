#include "mainwindow.h"
#include "newprojectdialog.h"
#include "ui_mainwindow.h"
#include "picturesview.h"
#include "clickablethumb.h"

#include <QList>
#include <QColorDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    this->project_is_temporary = true;
    ui->setupUi(this);
    this->showMaximized();
    this->createViews();
    this->showHomeView();

}

/*
 *
 * Views
 *
 */

void MainWindow::createViews(){
    this->newProjectDialog = new NewProjectDialog(this);
    this->createActions();
    this->createToolbars();
    this->createMenus();
    this->createHomeView();
    this->createEditionView();
}

void MainWindow::createHomeView(){
    this->homeView = new QWidget(this);

    QVBoxLayout *panel = new QVBoxLayout();
    QPushButton * newButt = new QPushButton("Nouveau Projet");
    newButt->setMinimumSize(350,100);
    connect(newButt, SIGNAL(pressed()),this,SLOT(newProject()));

    QPushButton * openButt = new QPushButton("Ouvrir Projet");
    openButt->setMinimumSize(350,100);
    connect(openButt, SIGNAL(pressed()),this,SLOT(openProject()));
    panel->addWidget(newButt);
    panel->addWidget(openButt);
    panel->setAlignment(Qt::AlignCenter);
    this->homeView->setLayout(panel);
    connect(this->newProjectDialog, SIGNAL(createdProject()), this, SLOT(createdProject()));
}

void MainWindow::createEditionView(){

    QVBoxLayout * containerLayout= new QVBoxLayout();

    QSplitter * splitter = new QSplitter(Qt::Vertical ,this);

    containerLayout->addWidget(splitter);

    this->editionView = splitter;
    //this->editionView->setLayout(containerLayout);

    QHBoxLayout *panel = new QHBoxLayout();

    QWidget * upArea = new QWidget();

    this->createTimeline();
    this->createDrawArea();


    splitter->setChildrenCollapsible(true);
    splitter->addWidget(upArea);
    splitter->addWidget(this->picturesArea);
    this->picturesArea->setSizePolicy(QSizePolicy::Preferred ,QSizePolicy::Fixed);
    /*QList<int> sizes;
    sizes.push_back(10);
    sizes.push_back(300);
    splitter->setSizes(sizes);*/
    splitter->setStretchFactor(0,8);
    splitter->setStretchFactor(1,2);
    panel->setMargin(40);
    QPushButton * left = new QPushButton("<");
    left->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left->setMaximumWidth(50);
    left->setMinimumWidth(20);
    connect(left, SIGNAL(pressed()), this->picturesArea, SLOT(prevPicture()));
    QPushButton * right = new QPushButton(">");
    right->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    right->setMaximumWidth(50);
    right->setMinimumWidth(20);
    connect(right, SIGNAL(pressed()), this->picturesArea, SLOT(nextPicture()));
    panel->addWidget(left);
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(this->drawingArea);
    panel->addLayout(layout);
    panel->addWidget(right);
    upArea->setLayout(panel);
}

void MainWindow::showHomeView(){
    this->ui->mainToolBar->hide();
    this->editionView->hide();
    QList<QToolBar *> toolbars = this->findChildren<QToolBar *>();
    foreach(QToolBar * tl, toolbars){
        tl->hide();
    }

    this->setCentralWidget(this->homeView);
}

void MainWindow::showEditionView(){
    this->ui->mainToolBar->show();
    this->homeView->hide();
    QList<QToolBar *> toolbars = this->findChildren<QToolBar *>();
    foreach(QToolBar * tl, toolbars){
        tl->show();
    }
    this->setCentralWidget(this->editionView);
    this->editionView->show();
}

/*
 *
 * Initialisations
 *
 */

void MainWindow::createToolbars(){
    this->myBruch = new QBrush();
    this->myBruch->setStyle(Qt::SolidPattern);
    this->myBruch->setColor(Qt::black);
    this->toolColor = QColor(Qt::black);
    this->toolSize = 2;
    this->editToolBar = this->ui->mainToolBar;
    this->editToolBar->setIconSize(QSize(50,50));
    this->editToolBar->setMovable(false);
    this->addToolBar(Qt::LeftToolBarArea, this->editToolBar);
    this->editToolBar->setFloatable(false);
    this->editToolBar->addAction(this->selectPencilAct);
    this->editToolBar->addAction(this->selectEraserAct);

    this->editToolBar->addSeparator();

    QComboBox *toolSize = new QComboBox;
    for(int i=2; i<=20;i+=2){
        QString s = "";
        s.append(QString("%1").arg(i));
        s.append(QString(" pt"));
        toolSize->addItem(s);
    }

    connect(toolSize, SIGNAL(activated(int)), this, SLOT(changeToolSize(int)));

    this->editToolBar->addWidget(toolSize);

    this->colorPickerButt = new QPushButton;
    this->colorPickerButt->setFixedHeight(50);
    this->colorPickerButt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QColor color(Qt::black);
    QString s("background: #"+ QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                             + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                             + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
    this->colorPickerButt->setStyleSheet(s);
    this->colorPickerButt->update();
    this->colorDialog = new QColorDialog(this);
    connect(this->colorPickerButt, SIGNAL(clicked()), this->colorDialog, SLOT(show()));
    connect(this->colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(changeColor(QColor)));
    this->editToolBar->addWidget(this->colorPickerButt);
    this->viewToolBar = this->addToolBar(tr("&Vue"));
    this->addToolBar(Qt::LeftToolBarArea, this->viewToolBar);
    this->viewToolBar->setFloatable(false);
    this->viewToolBar->setMovable(false);
    this->viewToolBar->setIconSize(QSize(50,50));
    this->viewToolBar->addAction(this->toggleOnionAct);
    this->viewToolBar->addAction(this->toggleBackgroundAct);
}

void MainWindow::createMenus(){
    this->fileMenu = this->menuBar()->addMenu(tr("&Fichier"));
    this->fileMenu->addAction(this->newProjectAct);
    this->fileMenu->addAction(this->saveProjectAct);

    this->editMenu = this->menuBar()->addMenu(tr("&Edition"));
    this->editMenu->addAction(this->selectPencilAct);
    this->editMenu->addAction(this->selectEraserAct);
    this->editMenu->addSeparator();
    this->editMenu->addAction(this->toggleOnionAct);
    this->editMenu->addAction(this->toggleBackgroundAct);

    this->viewMenu = this->menuBar()->addMenu(tr("&Previsualisation"));
}

void MainWindow::createActions(){

    /*
     * General actions
     */
    this->newProjectAct = new QAction(tr("&Nouveau Projet"),this);
    this->newProjectAct->setStatusTip("Creation d'un nouveau projet");
    this->newProjectAct->setShortcut(QKeySequence(tr("Ctrl+N")));
    connect(this->newProjectAct, SIGNAL(triggered()),this,SLOT(newProject()));

    this->saveProjectAct = new QAction(tr("&Sauvegarder"),this);
    this->saveProjectAct->setStatusTip("Sauvegarde du projet en cours");
    this->saveProjectAct->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(this->saveProjectAct, SIGNAL(triggered()),this,SLOT(saveProject()));


    QIcon *pelicon = new QIcon("../Resources/art-studio/artstudiopng/sun1.png");
    this->toggleOnionAct = new QAction(*pelicon, tr("&Afficher pelures"), this);
    this->toggleOnionAct->setCheckable(true);
    this->toggleOnionAct->setChecked(false);
    connect(this->toggleOnionAct, SIGNAL(toggled(bool)), this, SLOT(toggleOnion(bool)));

    QIcon *backicon = new QIcon("../Resources/art-studio/artstudiopng/landscape14.png");
    this->toggleBackgroundAct = new QAction(*backicon,tr("&Afficher fond"), this);
    this->toggleBackgroundAct->setCheckable(true);
    this->toggleBackgroundAct->setChecked(true);
    connect(this->toggleBackgroundAct, SIGNAL(toggled(bool)), this, SLOT(toggleBackground(bool)));
    /*
     * Tools
     */
    toolsActGrp = new QActionGroup(this);
    QIcon *icon = new QIcon("../Resources/art-studio/artstudiopng/marker.png");
    QIcon *gumicon = new QIcon("../Resources/art-studio/artstudiopng/bricks9.png");
    this->selectPencilAct = new QAction(*icon, tr("&Pinceau"), toolsActGrp);
    this->selectPencilAct->setStatusTip("Selectionner outil pinceau");
    this->selectPencilAct->setCheckable(true);
    this->selectPencilAct->setChecked(true);
    connect(this->selectPencilAct, SIGNAL(triggered()), this, SLOT(selectPencil()));

    this->selectEraserAct = new QAction(*gumicon, tr("&Gomme"), toolsActGrp);
    this->selectEraserAct->setStatusTip("Selectionner outil gomme");
    this->selectEraserAct->setCheckable(true);
    connect(this->selectEraserAct, SIGNAL(triggered()), this, SLOT(selectEraser()));

}

void MainWindow::createDrawArea(){
    this->drawingArea = new QZoneDessin(this->editionView);
}

void MainWindow::createTimeline(){
    this->picturesArea = new PicturesView(this->editionView);
    connect(this->picturesArea, SIGNAL(askedPicture(int)), this, SLOT(loadPicture(int)));
}

/*
 *
 * Slots
 *
 */


void MainWindow::loadPicture(int i){
    this->drawingArea->loadImage(i);
}

void MainWindow::changeToolSize(int i){
    this->toolSize = i*2 + 2;
    QPen pen;
    pen.setBrush(*this->myBruch);
    pen.setWidth(this->toolSize);
    pen.setColor(this->toolColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    this->drawingArea->setPen(pen);
}

void MainWindow::changeColor(QColor const &color){
    this->myBruch->setColor(color);
    QPen pen;
    this->toolColor = color;
    pen.setBrush(*this->myBruch);
    pen.setWidth(this->toolSize);
    pen.setColor(color);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    this->drawingArea->setPen(pen);
    QString s("background: #"+ QString(color.red() < 16? "0" : "") + QString::number(color.red(),16)
                             + QString(color.green() < 16? "0" : "") + QString::number(color.green(),16)
                             + QString(color.blue() < 16? "0" : "") + QString::number(color.blue(),16) + ";");
    this->colorPickerButt->setStyleSheet(s);
    this->colorPickerButt->update();
}

void MainWindow::saveProject(){
    this->picturesArea->selectPicture(this->drawingArea->current_picture);
    if(this->project_is_temporary){
        QString filename = QFileDialog::getSaveFileName(this, tr("Sauvegarder projet"), "/home/wolkom", tr("Dossiers"));
        QDir dir;
        if(filename.size()>0){
            this->project_is_temporary = false;
            if(dir.mkpath(filename)){
               std::vector<QString> paths;
               foreach(QString name,this->drawingArea->paths){
                   QFile f(name);
                   QFileInfo fileInfo(f.fileName());
                   QString filen(fileInfo.fileName());
                   QFile::copy(name, filename+"/"+filen);
                   paths.push_back(filename+"/"+filen);
                   QFile::copy(name+".png", filename+"/"+filen+".png");
               }
               this->drawingArea->paths = paths;
               this->newProjectDialog->working_directory->remove();
            }
        }
    }
    this->picturesArea->selectPicture(this->drawingArea->current_picture);
}

void MainWindow::newProject(){
    this->newProjectDialog->show();
}

void MainWindow::createdProject(){
    this->fps = this->newProjectDialog->fps->currentData().toInt();
    QStringList command;
    command <<"-i"<<this->newProjectDialog->nomvideo->text()<<"-r"<< /*QString::number(this->fps)*/ "0.1"<<this->newProjectDialog->working_directory->path()+"/video_%4d.jpeg";
    qDebug()<<command;
    QProcess process;
    process.setWorkingDirectory(this->newProjectDialog->working_directory->path());
    process.start("ffmpeg", command);
    process.waitForFinished();
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp"<<"*.jpeg";
    QFileInfoList fileInfoList = QDir(this->newProjectDialog->working_directory->path()).entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
    ClickableThumb * label;
    foreach(QFileInfo fi, fileInfoList){
        QImage temp(fi.absoluteFilePath());
        QImage pic(temp.scaled(640,480,Qt::KeepAspectRatio).size(),QImage::Format_ARGB32);
        pic.fill(Qt::transparent);
        pic.save(fi.absoluteFilePath()+".png");
        //this->drawingArea->drawed_pictures.push_back(pic);
        this->drawingArea->paths.push_back(fi.absoluteFilePath());
        label = new ClickableThumb();
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        label->setPixmap(QPixmap::fromImage(temp.scaled(50,50)));
        this->picturesArea->thumbs.push_back(label);
    }
    this->picturesArea->zone_dessin = this->drawingArea;
    this->picturesArea->draw();
    this->drawingArea->loadImage(0);
    this->showEditionView();
}

void MainWindow::undo(){
    //TODO
}

void MainWindow::toggleOnion(bool b){
    if(b)
        this->drawingArea->nombrePelures = 5;
    else
        this->drawingArea->nombrePelures = 1;
    qDebug()<<"Toggle ognons";
    this->drawingArea->repaint();
}

void MainWindow::toggleBackground(bool b){
    this->drawingArea->afficherDecor = b;
    this->drawingArea->repaint();
}

void MainWindow::selectPencil(){
    this->drawingArea->erase = false;
}

void MainWindow::selectEraser(){
    this->drawingArea->erase = true;
}

void MainWindow::openProject(){
    QString filename = QFileDialog::getExistingDirectory();
    if(filename.size()> 0){
        this->project_is_temporary = false;
        QStringList filters;
        filters <<"*.jpg" << "*.jpeg";
        QFileInfoList fileInfoList = QDir(filename).entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
        ClickableThumb * label;
        foreach(QFileInfo fi, fileInfoList){
            QImage temp(fi.absoluteFilePath());
            //this->drawingArea->drawed_pictures.push_back(pic);
            this->drawingArea->paths.push_back(fi.absoluteFilePath());
            label = new ClickableThumb();
            label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            label->setPixmap(QPixmap::fromImage(temp.scaled(50,50)));
            this->picturesArea->thumbs.push_back(label);
        }
        this->picturesArea->zone_dessin = this->drawingArea;
        this->picturesArea->draw();
        this->drawingArea->loadImage(0);
        this->showEditionView();
    }
}
/*
 *
 *
 *
 *
 */

void MainWindow::selectPicture(int i){
    this->picturesArea->selectPicture(i);
}

/*
 *
 * Destructeur
 *
 */

MainWindow::~MainWindow()
{
    delete ui;
}
