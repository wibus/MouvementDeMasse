#include "City.h"

#include <cmath>
using namespace std;

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamWriter>

using namespace cellar;


City::City(int width, int height):
    _size(  width, height),
    _calendar(),
    _sky(128, 128),
    _sun(Vec4f(-1, -1, 2, 0), Vec3f(-1.0, -1.0, -0.5)),
    _ground(   width+1, height+1),    
    _lands(    width,   height),
    _junctions(width+1, height+1),
    _bridges(),
    _citizens(),    
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

bool City::load(const string&)
{
    return false;
}

bool City::save(const string& fileName)
{
    // File names
    QFileInfo fileInfo( fileName.c_str() );
    QString filePath   = fileInfo.path() + '/';
    QString filePrefix = fileInfo.fileName();
    filePrefix.truncate(filePrefix.indexOf('.'));

    QString xmlFileName = filePrefix + ".xml";
    QString heightMapFileName = filePrefix + "_heightMap.bmp";
    QString landsMapFileName = filePrefix + "_landsMap.bmp";
    QString junctionsMapFileName = filePrefix + "_junctions.bmp";


    // The XML configuration file
    QString xmlDoc;
    QXmlStreamWriter xml(&xmlDoc);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();

    xml.writeStartElement("city");
        xml.writeStartElement("size");
            xml.writeAttribute("width", QString::number(_size.x()));
            xml.writeAttribute("height", QString::number(_size.y()));
        xml.writeEndElement(); // size

        xml.writeStartElement("calendar");
            xml.writeAttribute("year",   QString::number(_calendar.date().year));
            xml.writeAttribute("month",  QString::number(_calendar.date().month));
            xml.writeAttribute("day",    QString::number(_calendar.date().day));
            xml.writeAttribute("hour",   QString::number(_calendar.date().hour));
            xml.writeAttribute("minute", QString::number(_calendar.date().minute));
            xml.writeAttribute("second", QString::number(_calendar.date().second));
        xml.writeEndElement(); //size

        xml.writeStartElement("sky");
            xml.writeAttribute("wres", QString::number(_sky.cloudsGrid().width()));
            xml.writeAttribute("hres", QString::number(_sky.cloudsGrid().height()));
            xml.writeAttribute("cloudyness",  QString::number(_sky.cloudyness()));
            xml.writeAttribute("compactness", QString::number(_sky.compactness()));
        xml.writeEndElement(); //sky

        xml.writeStartElement("sun");
            xml.writeAttribute("midhightDirection", toString(_sun.midnightDirection()).c_str());
            xml.writeAttribute("rotationAxis",      toString(_sun.rotationAxis()).c_str());
        xml.writeEndElement(); //sun

        xml.writeStartElement("ground");
            xml.writeAttribute("width",  QString::number(_ground.width()));
            xml.writeAttribute("height", QString::number(_ground.height()));
            xml.writeAttribute("minHeight", QString::number(_ground.minHeight()));
            xml.writeAttribute("maxHeight", QString::number(_ground.maxHeight()));
            xml.writeAttribute("waterHeight", QString::number(_ground.waterHeight()));
            xml.writeAttribute("mapUrl", heightMapFileName);
        xml.writeEndElement(); //ground

        xml.writeStartElement("lands");
            xml.writeAttribute("width", QString::number(_lands.width()));
            xml.writeAttribute("height", QString::number(_lands.height()));
            xml.writeAttribute("mapUrl", landsMapFileName);
        xml.writeEndElement(); //lands

        xml.writeStartElement("junctions");
            xml.writeAttribute("width", QString::number(_junctions.width()));
            xml.writeAttribute("height", QString::number(_junctions.height()));
            xml.writeAttribute("mapUrl", junctionsMapFileName);
        xml.writeEndElement(); //junctions

        xml.writeStartElement("bridges");
            xml.writeAttribute("nb", QString::number(_bridges.size()));
            for(size_t b=0; b<_bridges.size(); ++b)
            {
                xml.writeStartElement("bridge");
                    xml.writeAttribute("endA", toString(_bridges[b].endA()).c_str());
                    xml.writeAttribute("endB", toString(_bridges[b].endB()).c_str());
                xml.writeEndElement();
            }
        xml.writeEndElement(); //bridges
    xml.writeEndElement(); // city

    xml.writeEndDocument();


    // Files writings
    QFile file( filePath + xmlFileName );
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
