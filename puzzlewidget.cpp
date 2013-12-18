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

//鼠标点击方法
void PuzzleWidget::mousePressEvent(QMouseEvent *event){
    QPoint p = event->pos();
    int index = findIndex(p);
    if(-1 == index){
        return;
    }
    QPixmap pixmap = piecePixmaps[index];
    QRect rect = pieceRects[index];

    //拖动开始时需要去除
    piecePixmaps.removeAt(index);
    pieceRects.removeAt(index);
    update(rect);

    QByteArray itemData;
    QDataStream dataStream(&itemData,QIODevice::WriteOnly);
    dataStream << pixmap;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("image/x-puzzle-piece",itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - rect.topLeft());
    drag->setPixmap(pixmap);

    if(!(drag->exec(Qt::MoveAction) == Qt::MoveAction)){
        //如果拖动不成功重新赋值回去
        piecePixmaps.insert(index,pixmap);
        pieceRects.insert(index,rect);
        update(rect);
    }
}

//根据QPoint查找索引位置
int PuzzleWidget::findIndex(QPoint point){
    for(int i = 0; i < pieceRects.size(); i++){
        QRect rect = pieceRects.at(i);
        if(rect.contains(point)){
            return i;
        }
    }
    return -1;
}

