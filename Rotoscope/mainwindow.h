#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QColorDialog>
#include <QBrush>

#include "newprojectdialog.h"
#include "qzonedessin.h"
#include "picturesview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int fps;
    bool project_is_temporary;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *editToolBar;
    QToolBar *viewToolBar;

    QWidget * homeView;
    QWidget * editionView;

    QBrush * myBruch;
    QPushButton * colorPickerButt;

    NewProjectDialog * newProjectDialog;
    QZoneDessin * drawingArea;
    PicturesView * picturesArea;
    QColorDialog * colorDialog;

    QColor toolColor;
    int toolSize;

    QAction *newProjectAct;
    QAction *saveProjectAct;

    QActionGroup *toolsActGrp;

    QAction *selectPencilAct;
    QAction *selectEraserAct;
    QAction *toggleBackgroundAct;
    QAction *toggleOnionAct;
    QAction *undoAct;

public slots:
    void newProject();
    void createdProject();
    void openProject();
    void saveProject();
    void undo();
    void toggleOnion(bool);
    void toggleBackground(bool);
    void selectPencil();
    void selectEraser();
    void changeColor(QColor const &color);
    void changeToolSize(int);
    void loadPicture(int);

public:
    explicit MainWindow(QWidget *parent = 0);
    void showHomeView();
    void showEditionView();

    void createViews();
    void createHomeView();
    void createEditionView();

    void createToolbars();
    void createMenus();
    void createActions();
    void createDrawArea();
    void createTimeline();

    void selectPicture(int i);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
