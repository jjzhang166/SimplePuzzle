#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMouseEvent>
#include <QDropEvent>

class PuzzleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PuzzleWidget(QWidget *parent = 0);
    void splitImageToPieces(QPixmap &sourcePixmap,int &rows,int &columns);
    int pieceSize() const {return piecePixmaps.length();}
    void clear();

signals:
    void puzzleCompleted(); //完成

protected:
    void paintEvent(QPaintEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:

    QRect targetSquare(const QPoint &position) const;
    int findIndex(const QRect rect);
    bool rightPlace();     //根据所处位置的QRect与QPoint进行比较
    void shuffled();

    QList<QPixmap> piecePixmaps;
    QList<QRect> pieceRects;
    QList<QPoint> pieceLocations; //用于记录每个方块的正确位置

    int width; //宽度
    int height; //高度
    QPixmap currentPixmap;//当前图片大小

};

#endif // PUZZLEWIDGET_H
