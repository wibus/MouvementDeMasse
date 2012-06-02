#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenuBar>

#include <Stages/QGLStage.h>


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(scaena::QGLStage* stage);

public slots:
signals:

private:
    scaena::QGLStage* _stage;
    QMenuBar* _menuBar;
};

#endif // MAINWINDOW_H
