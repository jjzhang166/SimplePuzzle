#include "mainwindow.h"
#include "puzzlewidget.h"
#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.openImage(":/images/example.jpg");
    w.show();

//    PuzzleWidget widget;
//    int rows = 5;
//    int column = 5;
//    QPixmap pixmap;
//    pixmap.load(":/images/example.jpg");
//    if(!pixmap.isNull()){
//        widget.splitImageToPieces(pixmap,rows,column);
//    }
//    widget.show();

    return a.exec();
}
