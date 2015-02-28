#include "newprojectdialog.h"
#include <QWidget>
#include <QTemporaryDir>

NewProjectDialog::NewProjectDialog(QWidget * parent) : QDialog(parent){
    QHBoxLayout *simplelay = new QHBoxLayout;
    //this->working_directory = new QDir("/home/wolkom/Downloads/test_video/test");
    this->working_directory = new QTemporaryDir();

    QFont font = this->font();
    font.setPointSize(16);
    this->setFont(font);

    QVBoxLayout *panelfinal = new QVBoxLayout(this);
    this->nom = new QLineEdit;
    this->nom->setMaximumWidth(200);
    this->fps = new QComboBox;
    this->fps->addItem("6 Img/Sec" , QVariant(6));
    this->fps->addItem("8 Img/Sec", QVariant(8));
    this->fps->addItem("12 Img/Sec", QVariant(12));
    this->fps->addItem("24 Img/Sec", QVariant(24));
    this->fps->setMaximumWidth(200);
    QPushButton *parcourirButt = new QPushButton("Parcourir");
    parcourirButt->setMaximumWidth(100);


    QHBoxLayout *panelbutt = new QHBoxLayout;
    QPushButton *creerButt = new QPushButton("Creer");
    creerButt->setMaximumWidth(150);
    QPushButton *annulerButt = new QPushButton("Annuler");
    annulerButt->setMaximumWidth(150);
    panelbutt->addWidget(creerButt);
    panelbutt->addWidget(annulerButt);
    panelbutt->setAlignment(Qt::AlignRight);
    panelbutt->setMargin(20);

    this->nomvideo = new QLineEdit;
    this->nomvideo->setMaximumWidth(250);
    simplelay->addWidget(this->nomvideo);
    simplelay->addWidget(parcourirButt);
    //creation de la fenentre
    QFormLayout *panelform = new QFormLayout;
    panelform->setHorizontalSpacing(40);
    panelform->setVerticalSpacing(20);
    panelform->setLabelAlignment(Qt::AlignRight);
    panelform->addRow("Nom du projet", nom);
    panelform->addRow("Frequence d'image", this->fps);
    panelform->addRow("Video à charger", simplelay);
    panelform->setFormAlignment(Qt::AlignCenter);
    panelfinal->addLayout(panelform);
    panelfinal->addLayout(panelbutt);
    panelfinal->setAlignment(Qt::AlignCenter);

    QObject::connect(parcourirButt, SIGNAL(pressed()),this,SLOT(loadVideo()));
    QObject::connect(creerButt, SIGNAL(pressed()),this,SLOT(createProject()));
    QObject::connect(annulerButt, SIGNAL(pressed()),this, SLOT(hide()));
}

/*
 *
 * Slots
 *
 */
void NewProjectDialog::loadVideo(){
    QString nomfichier = QFileDialog::getOpenFileName(this,tr("Ouvrir video"),"",tr("Video Files (*.mp4)"));
    this->nomvideo->setText(nomfichier);
}

void NewProjectDialog::createProject(){
    this->hide();
    emit createdProject();
}
