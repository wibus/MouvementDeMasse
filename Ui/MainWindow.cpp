#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFileDialog>

#include <Plays/AbstractPlay.h>
#include <Acts/AbstractAct.h>


MainWindow::MainWindow(scaena::QGLStage *stage) :
    _stage(stage),
    _menuBar(new QMenuBar()),
    _character(0x0)
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
    if(_character == 0x0)
        cacheCharacter();

    QString fileName = QFileDialog::getSaveFileName(
        this, "Save City", "./", "XML files (*.xml)");

    if(fileName.isEmpty())
        return;
    if(!fileName.endsWith(".xml"))
        fileName.append(".xml");

    _character->saveCity(fileName.toStdString());
}

void MainWindow::loadCity()
{
    if(_character == 0x0)
        cacheCharacter();

    QString fileName = QFileDialog::getOpenFileName(
        this, "Load City", "./", "XML files (*.xml)");

    if(fileName.isEmpty())
        return;

    _character->loadCity(fileName.toStdString());
}

void MainWindow::cacheCharacter()
{
    _character =  static_cast<MdMCharacter*>(&(
        _stage->play().currentAct().getCharacter("MdMCharacter")
    ));
}
