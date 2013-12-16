#include "mainwindow.h"
#include "puzzlewidget.h"

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
    setupMenu();

    puzzleWidget = new PuzzleWidget(this);
    this->setCentralWidget(puzzleWidget);
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

}

//打开图片
void MainWindow::openImage(QString path){
    QString filePath = path;
    if(filePath.isNull()){
        filePath = QFileDialog::getOpenFileName(this,tr("Open File"),"","Image Files (*.png *.jpg *.bmp)");
    }
    if(filePath.isEmpty()){
        QMessageBox::warning(this,tr("Open File Error"),"Can't open file",QMessageBox::Cancel);
        return;
    }
    qDebug() << "File Path:" << filePath;
    //实用QPixmap读取图片
    QPixmap pixmap;
    pixmap.load(filePath);
    int rows = 5;
    int columns = 5;
    if(!pixmap.isNull()){
        puzzleWidget->splitImageToPieces(pixmap,rows,columns);
    }else{
        QMessageBox::warning(this,tr("Read File Error"),"Can't read file:"+path,QMessageBox::Cancel);
        return;
    }

}

