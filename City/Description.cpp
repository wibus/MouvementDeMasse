#include "Description.h"

#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <Misc/CellarUtils.h>
#include <Misc/Log.h>

using namespace cellar;


Description::Description() :
    sunRadius(0.99994f),
    sunLight(),
    sunColor(     1.00, 0.90, 0.20, 1.00),
    nightSkyColor(0.02, 0.01, 0.05, 1.00),
    daySkyColor(  0.40, 0.60, 0.80, 1.00),
    skylineColor( 0.12, 0.12, 0.12, 1.00),
    curSkyColor(  nightSkyColor         ),
    grassColor(   0.32, 0.90, 0.08, 1.00),
    mudColor(     0.20, 0.14, 0.01, 1.00),
    waterColor(   0.03, 0.03, 0.30, 0.42),
    waterShininess( 200.0f),
    unitPerMeter(1.0f/40.0f),
    meterPerUnit(1.0f/unitPerMeter),
    citizensHeight(1.8f * unitPerMeter),
    roadWidth(     8.00f * unitPerMeter),
    bridgeWidth(   2.0f*roadWidth),
    bridgeHeight(  bridgeWidth),
    storyHeight(   8.0f * unitPerMeter),
    normalWalkingSpeed( 1.4f * unitPerMeter) // real life = 1.4 m/s
{
    sunLight.direction(1.0f, 1.0f, 0.0f, 0.0f).normalize();
    sunLight.ambient = curSkyColor;
    sunLight.diffuse( 0.76, 0.74, 0.72);
    sunLight.specular(0.65, 0.58, 0.46);
}

bool Description::load(const std::string& fileName)
{
    QFile xmlFile(fileName.c_str());
    xmlFile.open(QIODevice::ReadOnly);
    QTextStream xmlStream(&xmlFile);
    QXmlStreamReader xml(xmlStream.readAll());

    while (!xml.atEnd())
    {
        xml.readNext();
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            QXmlStreamAttributes att = xml.attributes();

            if(xml.name() == "sun")
            {
                fromString(att.value("radius").toString().toStdString(),    sunRadius);
                fromString(att.value("color").toString().toStdString(),     sunColor);
                fromString(att.value("ambient").toString().toStdString(),   sunLight.ambient);
                fromString(att.value("diffuse").toString().toStdString(),   sunLight.diffuse);
                fromString(att.value("specular").toString().toStdString(),  sunLight.specular);
                fromString(att.value("direction").toString().toStdString(), sunLight.direction);
            }
            else if(xml.name() == "sky")
            {
                fromString(att.value("nightColor").toString().toStdString(),   nightSkyColor);
                fromString(att.value("dayColor").toString().toStdString(),     daySkyColor);
                fromString(att.value("skyLineColor").toString().toStdString(), skylineColor);
                fromString(att.value("curColor").toString().toStdString(),     curSkyColor);
            }
            else if(xml.name() == "ground")
            {
                fromString(att.value("grassColor").toString().toStdString(),     grassColor);
                fromString(att.value("mudColor").toString().toStdString(),       mudColor);
                fromString(att.value("waterColor").toString().toStdString(),     waterColor);
                fromString(att.value("waterShininess").toString().toStdString(), waterShininess);
            }
            else if(xml.name() == "size")
            {
                fromString(att.value("unitPerMeter").toString().toStdString(),   unitPerMeter);
                fromString(att.value("meterPerUnit").toString().toStdString(),   meterPerUnit);
                fromString(att.value("citizensHeight").toString().toStdString(), citizensHeight);
                fromString(att.value("roadWidth").toString().toStdString(),      roadWidth);
                fromString(att.value("bridgeWidth").toString().toStdString(),    bridgeWidth);
                fromString(att.value("bridgeHeight").toString().toStdString(),   bridgeHeight);
                fromString(att.value("storyHeight").toString().toStdString(),    storyHeight);
            }
            else if(xml.name() == "speed")
            {
                fromString(att.value("normalWalking").toString().toStdString(), normalWalkingSpeed);
            }
        }
    }
    if (xml.hasError())
    {
        getLog().postMessage(new Message(
            'E',
            false,
            "City's description could not be loaded from XML file: " + fileName
            + "\terror: " + xml.errorString().toStdString(),
            "Description")
        );

        return false;
    }
    return true;
}

bool Description::save(const std::string& fileName)
{
    QString xmlDoc;
    QXmlStreamWriter xml(&xmlDoc);

    xml.writeStartDocument();
        xml.writeStartElement("visual_description");
            xml.writeStartElement("sun");
                xml.writeAttribute("radius",   toString(sunRadius).c_str());
                xml.writeAttribute("color",    toString(sunColor).c_str());
                xml.writeAttribute("ambient",  toString(sunLight.ambient).c_str());
                xml.writeAttribute("diffuse",  toString(sunLight.diffuse).c_str());
                xml.writeAttribute("specular", toString(sunLight.specular).c_str());
                xml.writeAttribute("direction",toString(sunLight.direction).c_str());
            xml.writeEndElement(); //sun

            xml.writeStartElement("sky");
                xml.writeAttribute("nightColor",   toString(nightSkyColor).c_str());
                xml.writeAttribute("dayColor",     toString(daySkyColor).c_str());
                xml.writeAttribute("skyLineColor", toString(skylineColor).c_str());
                xml.writeAttribute("curColor",     toString(curSkyColor).c_str());
            xml.writeEndElement(); //sky

            xml.writeStartElement("ground");
                xml.writeAttribute("grassColor",     toString(grassColor).c_str());
                xml.writeAttribute("mudColor",       toString(mudColor).c_str());
                xml.writeAttribute("waterColor",     toString(waterColor).c_str());
                xml.writeAttribute("waterShininess", toString(waterShininess).c_str());
            xml.writeEndElement(); //ground

            xml.writeStartElement("size");
                xml.writeAttribute("unitPerMeter",   QString::number(unitPerMeter));
                xml.writeAttribute("meterPerUnit",   QString::number(meterPerUnit));
                xml.writeAttribute("citizensHeight", QString::number(citizensHeight));
                xml.writeAttribute("roadWidth",      QString::number(roadWidth));
                xml.writeAttribute("bridgeWidth",    QString::number(bridgeWidth));
                xml.writeAttribute("bridgeHeight",   QString::number(bridgeHeight));
                xml.writeAttribute("storyHeight",    QString::number(storyHeight));
            xml.writeEndElement(); //size

            xml.writeStartElement("speed");
                xml.writeAttribute("normalWalking", QString::number(normalWalkingSpeed));
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
