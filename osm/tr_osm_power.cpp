/******************************************************************
 * project:     OSM Traffic
 *
 * (C)          Schmid Hubert 2025
 ******************************************************************/

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "tr_osm_power.h"

#include <QVariant>

QString tr_osm_power::ms_power = "power";
QString tr_osm_power::ms_generator = "generator";

tr_osm_power::tr_osm_power()
    : m_pt{0.0,0.0}
{
}

tr_osm_power::~tr_osm_power()
{
}

TrPoint tr_osm_power::getPoint() const
{
    return m_pt;
}

void tr_osm_power::setPoint(const TrPoint &pt)
{
    m_pt = pt;
}

uint64_t tr_osm_power::readXmlDescription(QXmlStreamReader &xml_in)
{
    return 0;
}

/*<tag k='generator:output:electricity' v='yes' />
<tag k='generator:source' v='solar' />
<tag k='generator:type' v='solar_photovoltaic_panel' />
<tag k='generator:place' v='roof' />
<tag k='power' v='generator' />*/


void tr_osm_power::writeXmlDescription(QXmlStreamWriter &xml_out, uint64_t id)
{
    //id="28000000" version="1" lon="111.0005" lat="48.0005">
    xml_out.writeStartElement("node");
    xml_out.writeAttribute("id", QVariant(static_cast<qulonglong>(id)).toString());
    xml_out.writeAttribute("lon", QVariant(m_pt.x/TR_COOR_FACTOR).toString());
    xml_out.writeAttribute("lat", QVariant(m_pt.y/TR_COOR_FACTOR).toString());

    writeXmlOsmTag(xml_out, ms_generator + ":method", "photovoltaic");
    writeXmlOsmTag(xml_out, ms_generator + ":output:electricity", "yes");
    writeXmlOsmTag(xml_out, ms_generator + ":source", "solar");
    writeXmlOsmTag(xml_out, ms_generator + ":type", "solar_photovoltaic_panel");
    writeXmlOsmTag(xml_out, ms_generator + ":place", "roof");
    writeXmlOsmTag(xml_out, ms_power, ms_generator);

    xml_out.writeEndElement();

}

void tr_osm_power::writeXmlOsmTag(QXmlStreamWriter &xml_out, const QString &k, const QString &v)
{
    xml_out.writeStartElement("tag");
    xml_out.writeAttribute("k", k);
    xml_out.writeAttribute("v", v);
    xml_out.writeEndElement();
}
