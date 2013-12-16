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
    int pieceSize() const {return piecePixmaps.size();}

protected:
    void paintEvent(QPaintEvent *event);

//    void dragEnterEvent(QDragEnterEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

private:
    QList<QPixmap> piecePixmaps;
    QList<QPoint> pieceLocations;
    QList<QRect> pieceRects;
    int width;
    int height;

};

#endif // PUZZLEWIDGET_H
