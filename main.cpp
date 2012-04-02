#include <ScaenaApplication/Application.h>
#include <Stages/AbstractStage.h>
#include <Stages/QGLStage.h>
#include "ScaenaBindings/MdMPlay.h"

#include <exception>
#include <iostream>

using namespace std;
using namespace scaena;


int main(int argc, char** argv) try
{
    getApplication().init(argc, argv, new MdMPlay());

    QGLStage* stage = new QGLStage();
    getApplication().addCustomStage(stage);
    getApplication().setupStage("QGLStage");

    getApplication().stage().setSize(600, 400);
    getApplication().stage().setUpdateInterval(16);

    return getApplication().execute();
}
catch(exception& e)
{
    cerr << "Exception caught : " << e.what() << endl;
}
catch(...)
{
    cerr << "Exception passed threw main..." << endl;
    throw;
}
