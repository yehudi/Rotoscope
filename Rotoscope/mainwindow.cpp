#include "mainwindow.h"
#include "newprojectdialog.h"
#include "ui_mainwindow.h"
#include "picturesview.h"
#include "clickablethumb.h"

#include <QList>
#include <QColorDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, QApplication * app) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    this->app = app;
    this->project_is_temporary = true;
    ui->setupUi(this);
    this->createViews();
    this->enableEditionActions(false);
    this->showHomeView();
    //this->showLoadingView("Loading");
    //this->showEditionView();
    //this->showHomeView();
    //this->showLoadingView("Loading");
    this->showMaximized();

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
    this->createLoadingView();
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
    this->playToolBar = new QToolBar;
    this->playToolBar->addAction(this->playVideoAct);
    this->playToolBar->addAction(this->pauseVideoAct);
    this->playToolBar->addAction(this->stopVideoAct);
    this->playToolBar->setIconSize(QSize(15,15));
    this->playToolBar->setFixedHeight(20);
    splitter->addWidget(this->playToolBar);
    this->playToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    splitter->addWidget(this->picturesArea);
    this->picturesArea->setSizePolicy(QSizePolicy::Preferred ,QSizePolicy::Fixed);
    /*QList<int> sizes;
    sizes.push_back(10);
    sizes.push_back(300);
    splitter->setSizes(sizes);*/
    splitter->setStretchFactor(0,8);
    splitter->setStretchFactor(1,1);
    splitter->setStretchFactor(2,2);
    panel->setMargin(40);
    QPushButton * left = new QPushButton(QIcon("../Resources/MyIcons/png/arrowhead7.png"),"");
    left->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left->setMaximumWidth(50);
    left->setMinimumWidth(20);
    connect(left, SIGNAL(pressed()), this->picturesArea, SLOT(prevPicture()));
    QPushButton * right = new QPushButton(QIcon("../Resources/MyIcons/png/arrow487.png"), "");
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

void MainWindow::createLoadingView(){
    this->loadingView = new QWidget(this);
    this->loadingView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout * layout = new QVBoxLayout();
    this->loadingView->setLayout(layout);
    layout->setAlignment(Qt::AlignCenter);
    this->loadingLabel = new QLabel();
    this->loadingLabel->setAlignment(Qt::AlignCenter);
    this->loadingLabelText = new QLabel;
    this->loadingGif = new QMovie("../Resources/loading.gif");
    loadingLabel->setMovie(this->loadingGif);
    layout->addWidget(this->loadingLabel);
    layout->addWidget(this->loadingLabelText);

    QFont font = this->font();
    font.setPointSize(16);
    this->loadingLabelText->setMargin(20);
    this->loadingLabelText->setFont(font);
    this->loadingLabelText->setAlignment(Qt::AlignCenter);
}

void MainWindow::showHomeView(){
    //this->loadingGif->stop();
    this->homeView->show();
    if (this->centralWidget())
        this->centralWidget()->setParent(0);
    this->setCentralWidget(this->homeView);
    this->ui->mainToolBar->hide();
    this->editionView->hide();
    this->loadingView->hide();
    QList<QToolBar *> toolbars = this->findChildren<QToolBar *>();
    foreach(QToolBar * tl, toolbars){
        tl->hide();
    }
}

void MainWindow::showLoadingView(QString message){
    qDebug()<<"Showing loading view";

    this->loadingGif->start();
    this->loadingLabelText->setText(message);
    this->loadingView->show();
    if (this->centralWidget())
        this->centralWidget()->setParent(0);
    this->setCentralWidget(this->loadingView);

    this->ui->mainToolBar->hide();
    this->editionView->hide();
    this->homeView->hide();
    QList<QToolBar *> toolbars = this->findChildren<QToolBar *>();
    foreach(QToolBar * tl, toolbars){
        tl->hide();
    }
    this->loadingGif->start();
}

void MainWindow::showEditionView(){
    this->loadingGif->stop();

    this->editionView->show();

    if (this->centralWidget())
        this->centralWidget()->setParent(0);
    this->setCentralWidget(this->editionView);

    this->ui->mainToolBar->show();
    this->homeView->hide();
    this->loadingView->hide();
    QList<QToolBar *> toolbars = this->findChildren<QToolBar *>();
    foreach(QToolBar * tl, toolbars){
        tl->show();
    }
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
    this->fileMenu->addAction(this->openProjectAct);
    this->fileMenu->addAction(this->closeProjectAct);
    this->fileMenu->addAction(this->saveProjectAct);
    this->fileMenu->addAction(this->saveAsProjectAct);
    this->fileMenu->addAction(this->exportAct);
    this->fileMenu->addAction(this->exitAct);

    this->editMenu = this->menuBar()->addMenu(tr("&Edition"));
    this->editMenu->addAction(this->selectPencilAct);
    this->editMenu->addAction(this->selectEraserAct);
    this->editMenu->addSeparator();
    this->editMenu->addAction(this->toggleOnionAct);
    this->editMenu->addAction(this->toggleBackgroundAct);
    this->editMenu->addAction(this->clearPageAct);

    this->viewMenu = this->menuBar()->addMenu(tr("&Previsualisation"));
    this->viewMenu->addAction(this->playVideoAct);
    this->viewMenu->addAction(this->pauseVideoAct);
    this->viewMenu->addAction(this->stopVideoAct);

    this->optionMenu = this->menuBar()->addMenu(tr("&Options"));
    QMenu * menuPelures = this->optionMenu->addMenu(tr("&nombre de pelures"));
    menuPelures->addAction(this->pelure1Act);
    menuPelures->addAction(this->pelure2Act);
    menuPelures->addAction(this->pelure3Act);
    menuPelures->addAction(this->pelure4Act);

    QMenu * menuDrawings = this->optionMenu->addMenu(tr("&Frequence dessin"));
    menuDrawings->addAction(this->drawingFrequency1Act);
    menuDrawings->addAction(this->drawingFrequency2Act);
    menuDrawings->addAction(this->drawingFrequency3Act);
}
void MainWindow::createActions(){

    /*
     * General actions
     */
    this->newProjectAct = new QAction(tr("&Nouveau Projet"),this);
    this->newProjectAct->setStatusTip("Creation d'un nouveau projet");
    this->newProjectAct->setShortcut(QKeySequence(tr("Ctrl+N")));
    connect(this->newProjectAct, SIGNAL(triggered()),this,SLOT(newProject()));

    this->openProjectAct = new QAction(tr("&Ouvrir Projet"),this);
    this->openProjectAct->setStatusTip("Ouvrir projet existant");
    connect(this->openProjectAct, SIGNAL(triggered()),this,SLOT(openProject()));

    this->closeProjectAct = new QAction(tr("&Fermer projet"),this);
    this->closeProjectAct->setStatusTip("Fermeture du projet");
    connect(this->closeProjectAct, SIGNAL(triggered()),this,SLOT(closeProject()));

    this->saveProjectAct = new QAction(tr("&Sauvegarder"),this);
    this->saveProjectAct->setStatusTip("Sauvegarde du projet en cours");
    this->saveProjectAct->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(this->saveProjectAct, SIGNAL(triggered()),this,SLOT(saveProject()));

    this->saveAsProjectAct = new QAction(tr("&Sauvegarder sous..."),this);
    this->saveAsProjectAct->setStatusTip("Sauvegarde du projet en cours dans un autre dossier");
    this->saveAsProjectAct->setShortcut(QKeySequence(tr("Ctrl+Maj+S")));
    connect(this->saveAsProjectAct, SIGNAL(triggered()),this,SLOT(saveAsProject()));

    this->toggleOnionAct = new QAction(QIcon("../Resources/MyIcons/png/three117.png"), tr("&Afficher pelures"), this);
    this->toggleOnionAct->setCheckable(true);
    this->toggleOnionAct->setChecked(false);
    connect(this->toggleOnionAct, SIGNAL(toggled(bool)), this, SLOT(toggleOnion(bool)));

    this->toggleBackgroundAct = new QAction(QIcon("../Resources/art-studio/artstudiopng/landscape14.png"),tr("&Afficher fond"), this);
    this->toggleBackgroundAct->setCheckable(true);
    this->toggleBackgroundAct->setChecked(true);
    connect(this->toggleBackgroundAct, SIGNAL(toggled(bool)), this, SLOT(toggleBackground(bool)));
    /*
     * Tools
     */
    toolsActGrp = new QActionGroup(this);
    this->selectPencilAct = new QAction(QIcon("../Resources/art-studio/artstudiopng/marker.png"), tr("&Pinceau"), toolsActGrp);
    this->selectPencilAct->setStatusTip("Selectionner outil pinceau");
    this->selectPencilAct->setCheckable(true);
    this->selectPencilAct->setChecked(true);
    connect(this->selectPencilAct, SIGNAL(triggered()), this, SLOT(selectPencil()));

    this->selectEraserAct = new QAction(QIcon("../Resources/art-studio/artstudiopng/bricks9.png"), tr("&Gomme"), toolsActGrp);
    this->selectEraserAct->setStatusTip("Selectionner outil gomme");
    this->selectEraserAct->setCheckable(true);
    connect(this->selectEraserAct, SIGNAL(triggered()), this, SLOT(selectEraser()));

    this->playVideoAct = new QAction(QIcon("../Resources/MyIcons/png/arrow16.png"),tr("&play"), this);
    this->playVideoAct->setStatusTip("Lancer la video");
    connect(this->playVideoAct, SIGNAL(triggered()), this, SLOT(playVideo()));

    this->pauseVideoAct = new QAction(QIcon("../Resources/MyIcons/png/pause52.png"),tr("&pause"), this);
    this->pauseVideoAct->setStatusTip("Mettre la video en pause");
    connect(this->pauseVideoAct, SIGNAL(triggered()), this, SLOT(pauseVideo()));

    this->exportAct = new QAction(tr("&exporter"), this);
    this->exportAct->setStatusTip("exporter la video");
    connect(this->exportAct, SIGNAL(triggered()), this, SLOT(exportVideo()));

    this->exitAct = new QAction(tr("&quitter"), this);
    this->exitAct->setStatusTip("quitter l'application");
    connect(this->exitAct, SIGNAL(triggered()), this, SLOT(close()));


    this->stopVideoAct = new QAction(QIcon("../Resources/MyIcons/png/stop4.png"),tr("&stop"), this);
    this->stopVideoAct->setStatusTip("Arreter la video");
    connect(this->stopVideoAct, SIGNAL(triggered()), this, SLOT(stopVideo()));

    this->undoAct = new QAction(tr("&annuler"), this);
    this->undoAct->setStatusTip("annuler dernière opération");
    connect(this->undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    this->clearPageAct = new QAction(tr("&vider la page"), this);
    this->clearPageAct->setStatusTip("Vider la page courrante");
    connect(this->clearPageAct, SIGNAL(triggered()), this, SLOT(clearPage()));

    this->changeOgnionFrequencyActGrp = new QActionGroup(this);
    this->pelure1Act = new QAction(tr("1 pelure"), this->changeOgnionFrequencyActGrp);
    this->pelure1Act->setCheckable(true);
    this->pelure1Act->setStatusTip("Changer nombre de pelures");
    connect(this->pelure1Act, SIGNAL(triggered()), this, SLOT(changeOgnionFrequency1()));
    this->pelure2Act = new QAction(tr("3 pelure"), this->changeOgnionFrequencyActGrp);
    this->pelure2Act->setCheckable(true);
    this->pelure2Act->setStatusTip("Changer nombre de pelures");
    connect(this->pelure2Act, SIGNAL(triggered()), this, SLOT(changeOgnionFrequency2()));
    this->pelure3Act = new QAction(tr("5 pelure"), this->changeOgnionFrequencyActGrp);
    this->pelure3Act->setCheckable(true);
    this->pelure3Act->setChecked(true);
    this->pelure3Act->setStatusTip("Changer nombre de pelures");
    connect(this->pelure3Act, SIGNAL(triggered()), this, SLOT(changeOgnionFrequency3()));
    this->pelure4Act = new QAction(tr("7 pelure"), this->changeOgnionFrequencyActGrp);
    this->pelure4Act->setCheckable(true);
    this->pelure4Act->setStatusTip("Changer nombre de pelures");
    connect(this->pelure4Act, SIGNAL(triggered()), this, SLOT(changeOgnionFrequency4()));

    this->drawingFrequencyActGrp = new QActionGroup(this);
    this->drawingFrequency1Act = new QAction(tr("1 image sur 1"), this->drawingFrequencyActGrp);
    this->drawingFrequency1Act->setCheckable(true);
    this->drawingFrequency1Act->setChecked(true);
    this->drawingFrequency1Act->setStatusTip("Changer la frequence d'image pour les pelures d'ognions");
    connect(this->drawingFrequency1Act, SIGNAL(triggered()), this, SLOT(drawingFrequency1()));
    this->drawingFrequency2Act = new QAction(tr("1 image sur 2"), this->drawingFrequencyActGrp);
    this->drawingFrequency2Act->setCheckable(true);
    this->drawingFrequency2Act->setStatusTip("Changer la frequence d'image pour les pelures d'ognions");
    connect(this->drawingFrequency2Act, SIGNAL(triggered()), this, SLOT(drawingFrequency2()));
    this->drawingFrequency3Act = new QAction(tr("1 image sur 3"), this->drawingFrequencyActGrp);
    this->drawingFrequency3Act->setCheckable(true);
    this->drawingFrequency3Act->setStatusTip("Changer la frequence d'image pour les pelures d'ognions");
    connect(this->drawingFrequency3Act, SIGNAL(triggered()), this, SLOT(drawingFrequency3()));
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

void MainWindow::exportVideo(){
    QString path = QFileDialog::getSaveFileName(this, tr("Exporter projet"), "/home/wolkom", tr("Video(*.mp4, *.avi)"));
    if(path.size()>0){
        QStringList list = path.split(".");
        if(list.size() == 0 || (list[list.size()-1] != QString("avi") && list[list.size()-1] != QString("mp4"))){
            path+=".mp4";
        }
        this->showLoadingView("Exportation vers "+path);
        this->app->processEvents();
        QStringList command;
        command <<"-r"<<QString::number(this->fps)<<"-i"<<this->projectDirectory+"/video_%4d.jpeg.png"<<path;
        QProcess process;
        qDebug()<<command.join(" ");
        process.setWorkingDirectory(this->projectDirectory);
        process.start("avconv", command);
        process.waitForFinished();
        this->showEditionView();
    }
}

void MainWindow::drawingFrequency1(){
    this->drawingArea->frequenceDessin = 1;
    this->drawingArea->update();
}


void MainWindow::drawingFrequency2(){
    this->drawingArea->frequenceDessin = 2;
    this->drawingArea->update();
}


void MainWindow::drawingFrequency3(){
    this->drawingArea->frequenceDessin = 3;
    this->drawingArea->update();
}

void MainWindow::changeOgnionFrequency1(){
    this->drawingArea->nombrePelures = 2;
    this->drawingArea->update();
}


void MainWindow::changeOgnionFrequency2(){
    this->drawingArea->nombrePelures = 4;
    this->drawingArea->update();
}


void MainWindow::changeOgnionFrequency3(){
    this->drawingArea->nombrePelures = 6;
    this->drawingArea->update();
}


void MainWindow::changeOgnionFrequency4(){
    this->drawingArea->nombrePelures = 8;
    this->drawingArea->update();
}

void MainWindow::clearPage(){
    this->drawingArea->clearDessin();
}

void MainWindow::pauseVideo(){
    this->picturesArea->pause();
    this->playVideoAct->setEnabled(true);
    this->pauseVideoAct->setDisabled(true);
}

void MainWindow::stopVideo(){
    this->playVideoAct->setEnabled(true);
    this->pauseVideoAct->setDisabled(true);
    this->picturesArea->stop();
}

void MainWindow::playVideo(){
    this->picturesArea->play();
    this->playVideoAct->setDisabled(true);
    this->pauseVideoAct->setEnabled(true);
}

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
                this->projectDirectory = filename;
                this->saveProjectInfo();
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

void MainWindow::saveAsProject(){
    this->picturesArea->selectPicture(this->drawingArea->current_picture);
    QString filename = QFileDialog::getSaveFileName(this, tr("Sauvegarder projet"), "/home/wolkom", tr("Dossiers"));
    QDir dir;
    if(filename.size()>0){
        this->project_is_temporary = false;
        if(dir.mkpath(filename)){
           this->projectDirectory = filename;
           this->saveProjectInfo();
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
    this->picturesArea->selectPicture(this->drawingArea->current_picture);
}

void MainWindow::newProject(){
    this->newProjectDialog->show();
}

void MainWindow::createdProject(){
    this->fps = this->newProjectDialog->fps->currentData().toInt();
    this->projectName = this->newProjectDialog->nom->text();

    this->showLoadingView("Chargement de la video...");
    this->app->processEvents();
    this->loadingGif->start();
    QStringList command;
    command <<"-i"<<this->newProjectDialog->nomvideo->text()<<"-r"<< QString::number(this->fps)<<this->newProjectDialog->working_directory->path()+"/video_%4d.jpeg";
    qDebug()<<command;
    QProcess process;
    process.setWorkingDirectory(this->newProjectDialog->working_directory->path());
    process.start("ffmpeg", command);
    process.waitForFinished();
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp"<<"*.jpeg";
    QFileInfoList fileInfoList = QDir(this->newProjectDialog->working_directory->path()).entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
    ClickableThumb * label;
    this->showLoadingView("Creation de l'interface...");
    this->app->processEvents();
    this->loadingGif->start();
    foreach(QFileInfo fi, fileInfoList){
        QImage temp(fi.absoluteFilePath());
        QImage pic(temp.scaled(640,480,Qt::KeepAspectRatio).size(),QImage::Format_ARGB32);
        pic.fill(Qt::transparent);
        pic.save(fi.absoluteFilePath()+".png");
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
    this->enableEditionActions(true);
}

void MainWindow::undo(){
    //TODO
}

void MainWindow::toggleOnion(bool b){
    if(b)
        this->drawingArea->nombrePelures = 5;
    else
        this->drawingArea->nombrePelures = 1;
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

void MainWindow::closeProject(){
    this->project_is_temporary = true;
    this->projectName = "";
    this->projectDirectory = "";
    this->enableEditionActions(false);
    this->showHomeView();
    this->app->processEvents();
    this->drawingArea->paths = std::vector<QString>();
    while(!this->picturesArea->thumbs.empty()){
        ClickableThumb * th = this->picturesArea->thumbs[0];
        this->picturesArea->thumbs.erase(this->picturesArea->thumbs.begin());
        delete th;
    }

}

void MainWindow::openProject(){
    QString filename = QFileDialog::getExistingDirectory();
    if(filename.size()> 0){
        this->loadProjectInfo(filename);
        this->showLoadingView("Chargement de "+this->projectName+"...");
        this->app->processEvents();
        this->loadingGif->start();
        this->project_is_temporary = false;
        QStringList filters;
        filters <<"*.jpg" << "*.jpeg";
        QFileInfoList fileInfoList = QDir(filename).entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
        ClickableThumb * label;
        foreach(QFileInfo fi, fileInfoList){
            QImage temp(fi.absoluteFilePath());
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
        this->enableEditionActions(true);
    }else{
        this->showHomeView();
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


void MainWindow::enableEditionActions(bool b){
    if(b){
        this->exportAct->setDisabled(false);
        this->closeProjectAct->setDisabled(false);
        this->openProjectAct->setEnabled(false);
        this->saveProjectAct->setEnabled(true);
        this->saveAsProjectAct->setEnabled(true);
        this->playVideoAct->setEnabled(true);
        this->pauseVideoAct->setDisabled(true);
        this->stopVideoAct->setEnabled(true);
        this->toolsActGrp->setEnabled(true);
        this->changeOgnionFrequencyActGrp->setEnabled(true);
        this->toggleBackgroundAct->setEnabled(true);
        this->toggleOnionAct->setEnabled(true);
        this->undoAct->setEnabled(true);
        this->clearPageAct->setEnabled(true);
    }else{
        this->closeProjectAct->setDisabled(true);
        this->openProjectAct->setDisabled(false);
        this->exportAct->setEnabled(false);
        this->saveProjectAct->setDisabled(true);
        this->saveAsProjectAct->setDisabled(true);
        this->playVideoAct->setDisabled(true);
        this->pauseVideoAct->setDisabled(true);
        this->stopVideoAct->setDisabled(true);
        this->toolsActGrp->setDisabled(true);
        this->changeOgnionFrequencyActGrp->setDisabled(true);
        this->toggleBackgroundAct->setDisabled(true);
        this->toggleOnionAct->setDisabled(true);
        this->undoAct->setDisabled(true);
        this->clearPageAct->setDisabled(true);
    }

}


void MainWindow::loadProjectInfo(QString directory){
    QFile f(directory+"/.project.txt");
    if (f.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&f);
        QString info = in.readAll();
        qDebug()<<info;
        QStringList list = info.split("\t\n");
        this->projectName = list[0];
        this->projectDirectory = list[1];
        this->fps = list[2].toInt();
        f.close();
    }
}

void MainWindow::saveProjectInfo(){
    QStringList list;
    list<<this->projectName<<this->projectDirectory<<QString::number(this->fps);
    QFile f(this->projectDirectory+"/.project.txt");
    if(f.open(QFile::WriteOnly)){
        f.write(list.join("\t\n").toStdString().c_str());
        f.close();
    }
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
