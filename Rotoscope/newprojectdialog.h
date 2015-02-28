#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QFont>
#include <QFileDialog>
#include <QObject>
#include <QDir>
#include <QTemporaryDir>

class NewProjectDialog : public QDialog{
    Q_OBJECT
private:
    QAction *create;
    QAction *cancel;
public slots:
    void loadVideo();
    void createProject();
signals:
    void createdProject();
public:
    QLineEdit *nomvideo;
    QLineEdit *nom;
    QComboBox *fps;
    QTemporaryDir * working_directory;
    NewProjectDialog(QWidget * parent = 0);
    ~NewProjectDialog(){
    }
};

#endif // NEWPROJECTDIALOG_H

