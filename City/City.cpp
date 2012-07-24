#include "City.h"

#include <cmath>
using namespace std;

#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>

using namespace cellar;


City::City(int width, int height):
    _size(  width, height),
    _sky(128, 128),
    _sun(Vec4f(-1, -1, 2, 0), Vec3f(-1.0, -1.0, -0.5)),
    _ground(   width+1, height+1),    
    _lands(    width,   height),
    _junctions(width+1, height+1),
    _bridges(),
    _citizens(),
    _calendar(),
    _description()
{
}

void City::reset()
{
    resetJunctions();
    resetLands();
    _bridges.clear();
    _citizens.clear();
}

bool City::load(const string& fileName)
{
    return false;
}

bool City::save(const string& fileName)
{
    QString xmlDoc;
    QXmlStreamWriter xmlWriter(&xmlDoc);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("city");
    xmlWriter.writeStartElement("size");
    xmlWriter.writeAttribute("width", QString::number(_size.x()));
    xmlWriter.writeAttribute("height", QString::number(_size.y()));
    xmlWriter.writeEndElement(); // size
    xmlWriter.writeEndElement(); // city

    xmlWriter.writeEndDocument();


    QFile file(fileName.c_str());
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << xmlDoc;
    file.close();

    return true;
}

void City::update()
{
    _calendar.tic();
    _sun.setTime(_calendar.date().hour,
                 _calendar.date().minute);
    _sky.update();
}

void City::resetJunctions()
{
    for(int j=0; j<_junctions.height(); ++j)
    {
        for(int i=0; i<_junctions.width(); ++i)
        {
            Junction* junc = _junctions.get(i, j);
            if(junc != 0x0)
                delete junc;
            _junctions.set(i, j, new Junction());
        }
    }
}

void City::resetLands()
{
    for(int j=0; j<_lands.height(); ++j)
    {
        for(int i=0; i<_lands.width(); ++i)
        {
            Land* land = _lands.get(i, j);
            if(land != 0x0)
                delete land;
            _lands.set(i, j, new Land());
        }
    }
}
