#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFocusPolicy(Qt::NoFocus);

    setupMenu();
}


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

