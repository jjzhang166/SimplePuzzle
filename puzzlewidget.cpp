#include "puzzlewidget.h"

#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>

PuzzleWidget::PuzzleWidget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
}

void PuzzleWidget::splitImageToPieces(QPixmap &sourcePixmap,int &rows,int &columns){
    int size = qMin(sourcePixmap.width(),sourcePixmap.height());

    //调整大小
    QPixmap currentPixmap = sourcePixmap.copy(sourcePixmap.width() - size,size,
                                              sourcePixmap.height() - size,size);

    //计算宽度跟高度
    width = currentPixmap.width()/columns;
    height = currentPixmap.height()/rows;

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            QPixmap currentPiecePixmap = currentPixmap.copy(j * width,i * height,
                                                            width,height);
            piecePixmaps.append(currentPiecePixmap);
            pieceLocations.append(QPoint(j * width,i * height));
            QRect rect = QRect(j * width,i * height,width,height);
            pieceRects.append(rect);
        }
    }
}

//重新绘图
void PuzzleWidget::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter painter(this);

    if(highlightedRect.isValid()){
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        //adjusted 在原来的QRect的基础上附加值，并返回新的QRect
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for(int i = 0; i < piecePixmaps.size(); ++i){
        //painter.drawPixmap(pieceLocations.at(i),piecePixmaps.at(i));
        QRect rect = pieceRects.at(i);
        painter.drawPixmap(rect.adjusted(0,0,-1,-1),piecePixmaps.at(i));
    }

}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasFormat("image/x-puzzle-piece")){
        event->accept();
    }else{
        event->ignore();
    }
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event){
    QPoint p = event->pos();
    QRect currentRect = findPiece(p);
    QPixmap pixmap = findPixmap(currentRect);

    QByteArray itemData;
    QDataStream dataStream(&itemData,QIODevice::WriteOnly);
    dataStream << pixmap;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("image/x-puzzle-piece",itemData);
    QDrag *drag = new QDrag(this);
    drag->setPixmap(pixmap);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - currentRect.topLeft());

    //highlightedRect = currentRect;
    update(currentRect);

    if(drag->exec(Qt::MoveAction) == Qt::MoveAction){

    }else{

    }


}

//根据QPoint查找QRect主要的含义是找出拖拽的对象
QRect PuzzleWidget::findPiece(QPoint point){
    foreach (QRect rect, pieceRects) {
        if(rect.contains(point)){
            return rect;
        }
    }
    return QRect();
}

QPixmap PuzzleWidget::findPixmap(QRect rect){
    for(int i = 0; i < pieceRects.size(); i++){
        if(rect == pieceRects[i]){
            return piecePixmaps[i];
        }
    }
}
