#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(scaena::QGLStage *stage) :
    _stage(stage),
    _menuBar(new QMenuBar())
{
    resize(800, 624);

    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    layout->setMargin(0);

    // Build menu bar
    QMenu* menuFile  = _menuBar->addMenu("File");
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
