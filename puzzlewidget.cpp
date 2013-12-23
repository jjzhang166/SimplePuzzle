#include "puzzlewidget.h"

#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QTime>

PuzzleWidget::PuzzleWidget(QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);
}

void PuzzleWidget::splitImageToPieces(QPixmap &sourcePixmap,int &rows,int &columns){
    int size = qMin(sourcePixmap.width(),sourcePixmap.height());

    //调整大小
    currentPixmap = sourcePixmap.copy(sourcePixmap.width() - size,size,
                                              sourcePixmap.height() - size,size);

    //计算宽度跟高度
    width = currentPixmap.width()/columns;
    height = currentPixmap.height()/rows;

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            QPixmap currentPiecePixmap = currentPixmap.copy(j * width,i * height,
                                                            width,height);
            piecePixmaps.append(currentPiecePixmap);
            QRect rect = QRect(j * width,i * height,width,height);
            pieceLocations.append(QPoint(j * width, i * height));
            pieceRects.append(rect);
        }
    }

    shuffled();

}

//重新绘图
void PuzzleWidget::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
    QPainter painter(this);

    for(int i = 0; i < piecePixmaps.size(); ++i){
        QRect rect = pieceRects.at(i);
        painter.drawPixmap(rect.adjusted(0,0,-1,-1),piecePixmaps.at(i));
    }

    if(highlightedRect.isValid()){
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        //adjusted 在原来的QRect的基础上附加值，并返回新的QRect
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    qDebug() << size();

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
    QRect rect = targetSquare(event->pos());
    int index = findIndex(rect);
    if(-1 == index){
        return;
    }
    QPixmap pixmap = piecePixmaps[index];

    //拖动开始时需要去除
    update(rect);

    QByteArray itemData;
    QDataStream dataStream(&itemData,QIODevice::WriteOnly);
    dataStream << index ;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("image/x-puzzle-piece",itemData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - rect.topLeft());
    drag->setPixmap(pixmap);

    if(!(drag->exec(Qt::MoveAction) == Qt::MoveAction)){
        //如果拖动不成功重新赋值回去
        update(rect);
    }
}

//拖动时
void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event){
    //找到当前处于哪一块上
    QRect rect = targetSquare(event->pos());
    int index = findIndex(rect);
    if(event->mimeData()->hasFormat("image/x-puzzle-piece") && index != -1){
        //将该块绘制成高亮
//        rect = pieceRects[index];
//        highlightedRect = rect;
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }else{
        event->ignore();
    }
    update(rect);
}

//取消拖动时 与dragMoveEvent相反
void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event){
    QRect rect = highlightedRect;
    highlightedRect = QRect();
    update(rect);
    event->accept();
}

//释放
void PuzzleWidget::dropEvent(QDropEvent *event){
    QRect rect = targetSquare(event->pos());
    int currentIndex = findIndex(rect);

    if(event->mimeData()->hasFormat("image/x-puzzle-piece") && currentIndex != -1){
        QByteArray puzzleData = event->mimeData()->data("image/x-puzzle-piece");
        int index;
        QDataStream dataStream(&puzzleData,QIODevice::ReadOnly);
        dataStream >> index;
        if(currentIndex == index){
            event->ignore();
            return;
        }
        //交换图片
        QPixmap currentPixmap = piecePixmaps[currentIndex];
        piecePixmaps[currentIndex] = piecePixmaps[index];
        piecePixmaps[index] = currentPixmap;
        //交换图片位置
        QPoint currentLocation = pieceLocations[currentIndex];
        pieceLocations[currentIndex] = pieceLocations[index];
        pieceLocations[index] = currentLocation;
        //判断位置是否处于正确的位置,如果全部处于正确的位置则完成
        if(rightPlace()){
            emit puzzleCompleted();
        }
        //接受后更新
        event->accept();
        update(pieceRects[currentIndex]);
        update(pieceRects[index]);
    }else{
        event->ignore();
    }
}

//将坐标转换成Rect
QRect PuzzleWidget::targetSquare(const QPoint &position) const
{
    return QRect(position.x()/width * width, position.y()/height * height, width, height);
}

//查找索引
int PuzzleWidget::findIndex(QRect rect){
    for(int i = 0; i < pieceRects.size(); i++){
        if(rect == pieceRects[i]){
            return i;
        }
    }
    return -1;
}

//计算成功的数量
bool PuzzleWidget::rightPlace(){
    int inPlace = 0;
    //根据QRect 是否包含有QPoint判断改图片是否处于正确的位置
    for(int i = 0; i < pieceRects.size(); i++){
        if(pieceRects[i].contains(pieceLocations[i])){
             ++inPlace;
        }
    }
    qDebug() << "inPlace:" << inPlace;
    if(inPlace == pieceSize()){
        return true;
    }
    return false;
}

void PuzzleWidget::shuffled(){
    //初始化
    int initArray[pieceSize()] ;
    for(int i = 0; i < pieceSize(); i++){
        initArray[i] = i;
    }

    //乱序
    QTime time;
    for(int i = 0; i < pieceSize(); i++){
        time = QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        int r = i + qrand()%(pieceSize() - i);
        int temp = initArray[r];
        initArray[r] = initArray[i];
        initArray[i] = temp;
    }

    for(int i = 0; i < pieceSize(); i++){
        QPixmap temp = piecePixmaps[i];
        int change = initArray[i];
        piecePixmaps[i] = piecePixmaps[change];
        piecePixmaps[change] = temp;
        QPoint tempPoint = pieceLocations[i];
        pieceLocations[i] = pieceLocations[change];
        pieceLocations[change] = tempPoint;
    }
}
