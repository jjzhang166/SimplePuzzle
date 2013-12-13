#include "mainwindow.h"
#include "puzzlewidget.h"
#include <QApplication>
#include <QPixmap>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    //w.show();

    PuzzleWidget p;
    QPixmap pixmap;
    pixmap.load(":/images/example.jpg");
    if(!pixmap.isNull()){
        int rows = 5;
        int columns = 5;
        p.splitImageToPieces(pixmap,rows,columns);
    }
    p.show();

    return a.exec();
}
