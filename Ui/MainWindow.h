#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenuBar>

#include <Stages/QGLStage.h>
#include <City/City.h>


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
    void cacheCity();

    scaena::QGLStage* _stage;
    QMenuBar* _menuBar;
    City* _city;
};

#endif // MAINWINDOW_H
