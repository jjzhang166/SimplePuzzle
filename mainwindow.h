#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "puzzlewidget.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
public slots:

    void openImage(QString path = QString());
    void isCompleted();
    void restart();

private:
    void setupMenu();

    PuzzleWidget *puzzleWidget;
    QString currentPath;
};

#endif // MAINWINDOW_H
