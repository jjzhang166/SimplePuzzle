#include "mainwindow.h"
#include "puzzlewidget.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFocusPolicy(Qt::NoFocus);
    resize(400,400);

    puzzleWidget = new PuzzleWidget(this);
    this->setCentralWidget(puzzleWidget);

    setupMenu();
}

//初始化菜单
void MainWindow::setupMenu(){

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAction = fileMenu->addAction(tr("&Open"));
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(tr("&Exit"));
    exitAction->setShortcut(QKeySequence::Quit);

    QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
    QAction *restartAction = gameMenu->addAction(tr("&Restart"));

    connect(exitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(puzzleWidget,SIGNAL(puzzleCompleted()),this,SLOT(isCompleted()));
    connect(restartAction,SIGNAL(triggered()),this,SLOT(restart()));
}

//打开图片
void MainWindow::openImage(QString path){
    currentPath = path;
    if(currentPath.isNull()){
        currentPath = QFileDialog::getOpenFileName(this,tr("Open File"),"","Image Files (*.png *.jpg *.bmp)");
    }
    if(currentPath.isEmpty()){
        QMessageBox::warning(this,tr("Open File Error"),"Can't open file",QMessageBox::Cancel);
        return;
    }
    qDebug() << "File Path:" << currentPath;
    //实用QPixmap读取图片
    QPixmap pixmap;
    pixmap.load(currentPath);
    int rows = 5;
    int columns = 5;
    if(!pixmap.isNull()){
        puzzleWidget->splitImageToPieces(pixmap,rows,columns);
    }else{
        QMessageBox::warning(this,tr("Read File Error"),"Can't read file:"+path,QMessageBox::Cancel);
        return;
    }
}

//完成
void MainWindow::isCompleted(){
    QMessageBox::information(this,tr("Puzzle Complted"),tr("Congratulations! You have completed the puzzle!\n"
                                                           "Click OK to start again."),QMessageBox::Ok);
    openImage(currentPath);
}

//重置游戏
void MainWindow::restart(){
    puzzleWidget = new PuzzleWidget(this);
    this->setCentralWidget(puzzleWidget);
    openImage(currentPath);
}

