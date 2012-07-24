#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(scaena::QGLStage *stage) :
    _stage(stage),
    _menuBar(new QMenuBar()),
    _city(0x0)
{
    resize(800, 624);

    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    layout->setMargin(0);

    // Build menu bar
    QMenu* menuFile  = _menuBar->addMenu("File");

    QAction* actionSave = menuFile->addAction("Save");
    QObject::connect(actionSave, SIGNAL(triggered()),
                     this,       SLOT(saveCity()));

    QAction* actionLoad = menuFile->addAction("Load");
    QObject::connect(actionLoad, SIGNAL(triggered()),
                     this,       SLOT(loadCity()));

    QAction* actionRestart = menuFile->addAction("Restart");
    QObject::connect(actionRestart, SIGNAL(triggered()),
                     _stage,        SLOT(restartPlay()));

    QAction* actionExit = menuFile->addAction("Exit");
    QObject::connect(actionExit, SIGNAL(triggered()),
                     this,        SLOT(close()));


    // Tweak layout
    _menuBar->setMinimumHeight(_menuBar->sizeHint().height());
    _menuBar->setMaximumHeight(_menuBar->sizeHint().height());
    layout->addWidget(_menuBar);
    layout->addWidget(_stage);
}

void MainWindow::saveCity()
{
    if(_city == 0x0)
        cacheCity();
}

void MainWindow::loadCity()
{
    if(_city == 0x0)
        cacheCity();
}

void MainWindow::cacheCity()
{
}
