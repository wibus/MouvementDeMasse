#include <exception>
#include <iostream>
using namespace std;

#include <ScaenaApplication/Application.h>
#include <Stages/QGLStage.h>
using namespace scaena;

#include "MdMPlay.h"
#include "Ui/MainWindow.h"


int main(int argc, char** argv) try
{
    getApplication().init(argc, argv, new MdMPlay());

    QGLStage* stage = new QGLStage();
    getApplication().addCustomStage(stage);
    getApplication().chooseStage(stage->id());

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
