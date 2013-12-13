#include "puzzlewidget.h"

#include <QPixmap>
#include <QPainter>

PuzzleWidget::PuzzleWidget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
}

void PuzzleWidget::splitImageToPieces(QPixmap &sourcePixmap,int &rows,int &columns){
    int size = qMin(sourcePixmap.width(),sourcePixmap.height());

    QPixmap currentPixmap = sourcePixmap.copy(sourcePixmap.width() - size,size,
                                              sourcePixmap.height() - size,size);

    //计算宽度跟高度
    width = currentPixmap.width()/columns;
    height = currentPixmap.height()/rows;

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            QPixmap currentPiecePixmap = currentPixmap.copy(i * width,j*height,
                                                            width,height);
            piecePixmaps.append(currentPiecePixmap);
            pieceLocations.append(QPoint(i*width,j*height));
        }
    }
}

void PuzzleWidget::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter painter(this);
    for(int i = 0; i < piecePixmaps.size(); ++i){
        painter.drawPixmap(pieceLocations.at(i),piecePixmaps.at(i));
    }
}
