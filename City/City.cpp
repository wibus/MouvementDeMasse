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
    if( !fileInfo.exists() )
        cout << fileInfo.fileName().toStdString() << endl;
    QString filePath   = fileInfo.path() + '/';
    QString filePrefix = fileInfo.fileName();

    int dotPos = filePrefix.indexOf('.');
    if(dotPos != -1)
        filePrefix.truncate( dotPos );

    QString xmlFileName       = filePrefix + ".xml";
    QString descriptFileName  = "Visual_description.xml";
    QString heightMapFileName = filePrefix + "_heightMap.bmp";
    QString skyMapFileName    = filePrefix + "_skyMap.bmp";


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
                    xml.writeAttribute("id",    QString::number(ctz.cid));
                    xml.writeAttribute("state", Citizen::STATE_STRINGS[ctz.state].c_str());
                    xml.writeAttribute("walkingSpeed", QString::number(ctz.walkingSpeed));
                    xml.writeAttribute("position", toString(ctz.position).c_str());
                    xml.writeAttribute("direction", toString(ctz.direction).c_str());
                    xml.writeAttribute("homePos", toString(ctz.homePos).c_str());
                    xml.writeAttribute("homeRoom", toString(ctz.homeRoom).c_str());
                    xml.writeAttribute("workPos", toString(ctz.workPos).c_str());
                    xml.writeAttribute("workRoom", toString(ctz.workRoom).c_str());

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


    if( !saveDescription((filePath + descriptFileName).toStdString()))
        return false;
    if( !saveHeightMap((filePath + heightMapFileName).toStdString()))
        return false;
    if( !saveSkyMap((filePath + skyMapFileName).toStdString()))
        return false;

    return true;
}

bool City::saveDescription(const std::string& fileName)
{
    QString xmlDoc;
    QXmlStreamWriter xml(&xmlDoc);

    xml.writeStartDocument();
        xml.writeStartElement("visual_description");
            xml.writeStartElement("sun");
                xml.writeAttribute("radius",   toString(_description.sunRadius).c_str());
                xml.writeAttribute("color",    toString(_description.sunColor).c_str());
                xml.writeAttribute("ambient",  toString(_description.sunLight.ambient).c_str());
                xml.writeAttribute("diffuse",  toString(_description.sunLight.diffuse).c_str());
                xml.writeAttribute("specular", toString(_description.sunLight.specular).c_str());
                xml.writeAttribute("direction",toString(_description.sunLight.direction).c_str());
            xml.writeEndElement(); //sun

            xml.writeStartElement("sky");
                xml.writeAttribute("nightColor",   toString(_description.nightSkyColor).c_str());
                xml.writeAttribute("dayColor",     toString(_description.daySkyColor).c_str());
                xml.writeAttribute("skyLineColor", toString(_description.skylineColor).c_str());
                xml.writeAttribute("curColor",     toString(_description.curSkyColor).c_str());
            xml.writeEndElement(); //sky

            xml.writeStartElement("ground");
                xml.writeAttribute("grassColor", toString(_description.grassColor).c_str());
                xml.writeAttribute("mudColor",   toString(_description.mudColor).c_str());
                xml.writeAttribute("waterColor", toString(_description.waterColor).c_str());
                xml.writeAttribute("waterShininess", toString(_description.waterShininess).c_str());
            xml.writeEndElement(); //ground

            xml.writeStartElement("size");
                xml.writeAttribute("unitPerMeter",   QString::number(_description.unitPerMeter));
                xml.writeAttribute("meterPerUnit",   QString::number(_description.meterPerUnit));
                xml.writeAttribute("citizensHeight", QString::number(_description.citizensHeight));
                xml.writeAttribute("roadWidth",      QString::number(_description.roadWidth));
                xml.writeAttribute("bridgeWidth",    QString::number(_description.bridgeWidth));
                xml.writeAttribute("bridgeHeight",   QString::number(_description.bridgeHeight));
                xml.writeAttribute("storyHeight",    QString::number(_description.storyHeight));
            xml.writeEndElement(); //size

            xml.writeStartElement("speed");
                xml.writeAttribute("normalWalking", QString::number(_description.normalWalkingSpeed));
            xml.writeEndElement(); //speed
        xml.writeEndElement(); //visual_description
    xml.writeEndDocument();

    // Files writings
    QFile file( fileName.c_str() );
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
    Image heightMap = Image(new unsigned char[_ground.width()*_ground.height()*3],
                            _ground.width(), _ground.height(), Image::RGB);

    for(unsigned int j=0; j<heightMap.height(); ++j)
    {
        for(unsigned int i=0; i<heightMap.width(); ++i)
        {
            float intensity = _ground.heightAt((int)i, (int)j) * 256 / amplitude;
            unsigned char absIntensity = absolute(intensity);
            bool underWater = intensity < 0.0f;
            heightMap.setColorAt(i, j, RGBAColor(
                absIntensity,
                ( underWater ? 0  : 32 + absIntensity*0.875f),
                (!underWater ? 0  : 32 + absIntensity*0.875f)
            ));
        }
    }

    return heightMap.saveBmp( fileName );
}

bool City::saveSkyMap(const std::string& fileName)
{
    Image skyMap = Image(new unsigned char[_sky.cloudsGrid().width()*_sky.cloudsGrid().height()*3],
                         _sky.cloudsGrid().width(), _sky.cloudsGrid().height(), Image::RGB);

    for(unsigned int j=0; j<skyMap.height(); ++j)
    {
        for(unsigned int i=0; i<skyMap.width(); ++i)
        {
            float intensity = (_sky.cloudsGrid().get(i, j) + 1.0f) * 0.5f * 256;
            skyMap.setColorAt(i, j, RGBAColor(
                intensity,
                intensity,
                128 + intensity /2.0f
            ));
        }
    }

    return skyMap.saveBmp( fileName );
}

void City::update()
{
    _calendar.tic();
    _sun.setTime(_calendar.date().hour,
                 _calendar.date().minute,
                 _calendar.date().second);
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
