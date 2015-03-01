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
    QMenu *optionMenu;
    QMenu *helpMenu;

    QToolBar *editToolBar;
    QToolBar *viewToolBar;
    QToolBar *playToolBar;

    QWidget * homeView;
    QWidget * editionView;
    QWidget * loadingView;

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
    QAction *saveAsProjectAct;
    QAction *playVideoAct;
    QAction *pauseVideoAct;
    QAction *stopVideoAct;
    QAction *clearPageAct;

    QActionGroup *changeOgnionFrequencyActGrp;
    QAction *pelure1Act;
    QAction *pelure2Act;
    QAction *pelure3Act;
    QAction *pelure4Act;

    QActionGroup *drawingFrequencyActGrp;
    QAction *drawingFrequency1Act;
    QAction *drawingFrequency2Act;
    QAction *drawingFrequency3Act;

    QActionGroup *toolsActGrp;

    QAction *selectPencilAct;
    QAction *selectEraserAct;
    QAction *toggleBackgroundAct;
    QAction *toggleOnionAct;
    QAction *undoAct;

    QLabel * loadingLabel;
    QLabel * loadingLabelText;
    QMovie * loadingGif;

public slots:
    void newProject();
    void createdProject();
    void openProject();
    void saveProject();
    void saveAsProject();
    void undo();
    void toggleOnion(bool);
    void toggleBackground(bool);
    void selectPencil();
    void selectEraser();
    void changeColor(QColor const &color);
    void changeToolSize(int);
    void loadPicture(int);
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void clearPage();
    void changeOgnionFrequency1();
    void changeOgnionFrequency2();
    void changeOgnionFrequency3();
    void changeOgnionFrequency4();


    void drawingFrequency1();
    void drawingFrequency2();
    void drawingFrequency3();

public:
    explicit MainWindow(QWidget *parent = 0);
    void showHomeView();
    void showEditionView();
    void showLoadingView(QString);

    void createViews();
    void createHomeView();
    void createEditionView();
    void createLoadingView();

    void createToolbars();
    void createMenus();
    void createActions();
    void createDrawArea();
    void createTimeline();

    void selectPicture(int i);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void enableEditionActions(bool);
};

#endif // MAINWINDOW_H
