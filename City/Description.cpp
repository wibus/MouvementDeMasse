#include "Description.h"

#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


#include <CellarWorkbench/Misc/Log.h>

using namespace cellar;


Description::Description() :
    sunRadius(0.99994f),
    sunLight(),
    sunColor(     1.00f, 0.90f, 0.20f, 1.00f),
    nightSkyColor(0.02f, 0.01f, 0.05f, 1.00f),
    daySkyColor(  0.40f, 0.60f, 0.80f, 1.00f),
    sunSetColor(  0.70f, 0.55f, 0.10f, 1.00f),
    skylineColor( 0.12f, 0.12f, 0.12f, 1.00f),
    grassColor(   0.32f, 0.90f, 0.08f, 1.00f),
    mudColor(     0.20f, 0.14f, 0.01f, 1.00f),
    waterColor(   0.03f, 0.03f, 0.30f, 0.42f),
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
    sunLight.direction = glm::normalize(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
    sunLight.diffuse = glm::vec4( 0.76f, 0.74f, 0.72f , 1.0f);
    sunLight.specular = glm::vec4(0.65f, 0.58f, 0.46f, 1.0f);
}

bool Description::load(const std::string& fileName)
{
    throw std::exception();
/*
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
                fromString(att.value("sunSetColor").toString().toStdString(),  sunSetColor);
                fromString(att.value("skyLineColor").toString().toStdString(), skylineColor);
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
    */
    return true;
}

bool Description::save(const std::string& fileName)
{
    throw std::exception();
/*
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
                xml.writeAttribute("sunSetColor",  toString(sunSetColor).c_str());
                xml.writeAttribute("skyLineColor", toString(skylineColor).c_str());
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
    */
    return true;
}
