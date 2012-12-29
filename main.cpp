#include <exception>
#include <iostream>
using namespace std;

#include <QApplication>
#include <QCleanlooksStyle>

#include <DataStructure/PGrid3D.h>

#include <ScaenaApplication/Application.h>
#include <Stages/QGLStage.h>
using namespace scaena;

#include "MdMPlay.h"
#include "Ui/MainWindow.h"


int main(int argc, char** argv) try
{
    getApplication().init(argc, argv);
    getApplication().setPlay(shared_ptr<AbstractPlay>(new MdMPlay()));

    QGLStage* stage = new QGLStage();
    getApplication().addCustomStage(stage);
    getApplication().chooseStage(stage->id());

    stage->setDrawSynch( false );
    stage->setDrawInterval( 0 );
    stage->setUpdateInterval( 0 );

    MainWindow window(stage);
    window.show();

    return getApplication().execute();    
}
catch(exception& e)
{
    cerr << "Exception caught : " << e.what() << endl;
}
catch(...)
{
    cerr << "Exception passed threw.." << endl;
    throw;
}
