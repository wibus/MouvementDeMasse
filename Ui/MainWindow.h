#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenuBar>

#include <Stages/QGLStage.h>
#include <MdMCharacter.h>


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(scaena::QGLStage* stage);

public slots:
    void saveCity();
    void loadCity();

signals:

private:
    void cacheCharacter();

    scaena::QGLStage* _stage;
    QMenuBar* _menuBar;
    MdMCharacter* _character;
};

#endif // MAINWINDOW_H
