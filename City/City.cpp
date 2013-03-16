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
    _dateAndTime(2000, JANUARY, 0, 18, 0, 0),
    _timeJump(0, 0, 0, 0, 0, 1),
    _sky(256, 256),
    _sun(Vec4f(-1, -1, 2, 0).normalize(), Vec3f(1.0, 1.0, 1.0).normalize()),
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

bool City::load(const string& fileName)
{
    QString filePath;
    QString xmlFileName;
    QString descriptFileName;
    QString heightMapFileName;
    QString skyMapFileName;

    constructFileNames(
        fileName,
        filePath,
        xmlFileName,
        descriptFileName ,
        heightMapFileName,
        skyMapFileName);

    if( !_description.load((filePath + descriptFileName).toStdString()))
        return false;

    return false;
}

bool City::save(const string& fileName)
{
    QString filePath;
    QString xmlFileName;
    QString descriptFileName;
    QString heightMapFileName;
    QString skyMapFileName;

    constructFileNames(
        fileName,
        filePath,
        xmlFileName,
        descriptFileName ,
        heightMapFileName,
        skyMapFileName);


    // Other files
    if( !_description.save((filePath + descriptFileName).toStdString()))
        return false;
    if( !saveHeightMap((filePath + heightMapFileName).toStdString()))
        return false;
    if( !saveSkyMap((filePath + skyMapFileName).toStdString()))
        return false;


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
            xml.writeAttribute("year",   QString::number(_dateAndTime.year));
            xml.writeAttribute("month",  QString::number(_dateAndTime.month));
            xml.writeAttribute("day",    QString::number(_dateAndTime.day));
            xml.writeAttribute("hour",   QString::number(_dateAndTime.hour));
            xml.writeAttribute("minute", QString::number(_dateAndTime.minute));
            xml.writeAttribute("second", QString::number(_dateAndTime.second));
        xml.writeEndElement(); //size

        xml.writeStartElement("visual_description");
            xml.writeAttribute("fileUrl", descriptFileName);
        xml.writeEndElement(); //visual_description

        xml.writeStartElement("sky");
            xml.writeAttribute("wres", QString::number(_sky.cloudsGrid().width()));
            xml.writeAttribute("hres", QString::number(_sky.cloudsGrid().height()));
            xml.writeAttribute("cloudyness",  QString::number(_sky.cloudyness()));
            xml.writeAttribute("compactness", QString::number(_sky.compactness()));
            xml.writeAttribute("mapUrl", skyMapFileName);
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
            xml.writeAttribute("maxNbStories", QString::number(Land::maxNbStories()));

            for(int j=0; j<_lands.height(); ++j)
            {
                xml.writeStartElement("line");
                xml.writeAttribute("y", QString::number(j));
                for(int i=0; i<_lands.width(); ++i)
                {
                    xml.writeStartElement("land");
                        xml.writeAttribute("pos", toString(Vec2i(i, j)).c_str());
                        xml.writeAttribute("type", Land::TYPE_STRINGS[_lands.get(i, j)->type()].c_str());
                        xml.writeAttribute("nbStories",   QString::number(_lands.get(i, j)->nbStories()));
                        xml.writeAttribute("capacity",    QString::number(_lands.get(i, j)->capacity()));
                        xml.writeAttribute("nbResidents", QString::number(_lands.get(i, j)->nbResidents()));
                    xml.writeEndElement(); //land
                }
                xml.writeEndElement(); //line
            }
        xml.writeEndElement(); //lands

        xml.writeStartElement("junctions");
            xml.writeAttribute("width", QString::number(_junctions.width()));
            xml.writeAttribute("height", QString::number(_junctions.height()));

            for(int j=0; j<_junctions.height(); ++j)
            {
                xml.writeStartElement("line");
                xml.writeAttribute("y", QString::number(j));
                for(int i=0; i<_junctions.width(); ++i)
                {
                    xml.writeStartElement("junction");
                        xml.writeAttribute("pos", toString(Vec2i(i, j)).c_str());
                        xml.writeAttribute("type", Junction::TYPE_STRINGS[_junctions.get(i, j)->type()].c_str());
                        for(int d=0; d<NB_DIRECTIONS; ++d)
                        {
                            if(_junctions.get(i, j)->getStreet( INT_TO_CARDINAL_DIRECTION[d] ) != 0x0)
                            {
                                xml.writeStartElement("street");
                                xml.writeAttribute("direction", CARDINAL_DIRECTION_STRINGS[d].c_str());
                                xml.writeEndElement(); //street
                            }
                        }
                    xml.writeEndElement(); //land
                }
                xml.writeEndElement(); //line
            }
        xml.writeEndElement(); //junctions

        xml.writeStartElement("bridges");
            xml.writeAttribute("nb", QString::number(_bridges.size()));
            for(BridgeIterator b=_bridges.begin(); b!=_bridges.end(); ++b)
            {
                xml.writeStartElement("bridge");
                    xml.writeAttribute("endA", toString( b->endA() ).c_str());
                    xml.writeAttribute("endB", toString( b->endB() ).c_str());
                xml.writeEndElement();
            }
        xml.writeEndElement(); //bridges


        xml.writeStartElement("citizens");
            xml.writeAttribute("nb", QString::number(_citizens.size()));
            for(CitizenIterator c=_citizens.begin(); c!=_citizens.end(); ++c)
            {
                Citizen& ctz = c->second;

                xml.writeStartElement("citizen");
                    xml.writeAttribute("id",    QString::number(ctz.id()));
                    xml.writeAttribute("state", CITIZEN_STATE_STRINGS[ctz.curState].c_str());
                    xml.writeAttribute("walkingSpeed", QString::number(ctz.walkingSpeed));
                    xml.writeAttribute("position", toString(ctz.position).c_str());
                    xml.writeAttribute("direction", toString(ctz.direction).c_str());
                    xml.writeAttribute("homePos", toString(ctz.homePos).c_str());
                    xml.writeAttribute("workPos", toString(ctz.workPos).c_str());

                    xml.writeStartElement("path");
                        xml.writeAttribute("name", "Home to Work path");
                        xml.writeAttribute("src", toString(ctz.homeToWorkPath.source).c_str());
                        xml.writeAttribute("dst", toString(ctz.homeToWorkPath.destination).c_str());
                        xml.writeAttribute("nbNodes", QString::number(ctz.homeToWorkPath.nodes.size()));
                        for(Path::NodeIterator n=ctz.homeToWorkPath.nodes.begin();
                            n != ctz.homeToWorkPath.nodes.end(); ++n)
                        {
                            xml.writeStartElement("node");
                                xml.writeAttribute("type", Path::NODE_TYPE_STRINGS[n->type].c_str());
                                xml.writeAttribute("position", toString(n->pos).c_str());
                            xml.writeEndElement(); //node
                        }
                    xml.writeEndElement(); //path
                xml.writeEndElement(); //citizen
            }
        xml.writeEndElement(); //citizens
    xml.writeEndElement(); // city

    xml.writeEndDocument();


    // Files writings
    QFile file( filePath + xmlFileName );
    if( !file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << xmlDoc;
    file.close();

    return true;
}

bool City::saveHeightMap(const std::string& fileName)
{
    float amplitude = maxVal(absolute(_ground.minHeight()),
                             absolute(_ground.maxHeight()));
    Image heightMap = Image(new unsigned char[_ground.width()*_ground.height()*4],
                            _ground.width(),
                            _ground.height());

    for(int j=0; j<heightMap.height(); ++j)
    {
        for(int i=0; i<heightMap.width(); ++i)
        {
            float intensity = _ground.heightAt((int)i, (int)j) * 256 / amplitude;
            unsigned char absIntensity = absolute(intensity);
            bool underWater = intensity < 0.0f;
            heightMap.setColor(i, j,
                               absIntensity,
                               underWater  ? 0  : 32 + absIntensity*0.875f,
                               !underWater ? 0  : 32 + absIntensity*0.875f,
                               0xff);
        }
    }

    return heightMap.save( fileName );
}

bool City::saveSkyMap(const std::string& fileName)
{
    Image skyMap = Image(new unsigned char[_sky.cloudsGrid().width()*_sky.cloudsGrid().height()*3],
                         _sky.cloudsGrid().width(), _sky.cloudsGrid().height());

    for(int j=0; j<skyMap.height(); ++j)
    {
        for(int i=0; i<skyMap.width(); ++i)
        {
            float intensity = (_sky.cloudsGrid().get(i, j) + 1.0f) * 0.5f * 256;
            skyMap.setColor(i, j,
                            intensity,
                            intensity,
                            128 + intensity /2.0f,
                            0xff);
        }
    }

    return skyMap.save( fileName );
}

void City::constructFileNames(
        const std::string& fileName,
        QString& filePath,
        QString& xmlFileName,
        QString& descriptFileName ,
        QString& heightMapFileName,
        QString& skyMapFileName)
{
    QFileInfo fileInfo( fileName.c_str() );
    if( !fileInfo.exists() )
        cout << fileInfo.fileName().toStdString() << endl;

    QString filePrefix = fileInfo.fileName();
    int dotPos = filePrefix.indexOf('.');
    if(dotPos != -1)
        filePrefix.truncate( dotPos );

    filePath          = fileInfo.path() + '/';
    xmlFileName       = filePrefix + ".xml";
    descriptFileName  = filePrefix + "_visualDescription.xml";
    heightMapFileName = filePrefix + "_heightMap.bmp";
    skyMapFileName    = filePrefix + "_skyMap.bmp";
}

void City::update()
{
    _dateAndTime += _timeJump;
    _sun.setTime(_dateAndTime);
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
