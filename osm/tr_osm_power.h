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


#ifndef TR_OSM_POWER_H
#define TR_OSM_POWER_H

#include "tr_point.h"

#include <QString>
#include <QXmlStreamReader>

/* 
    <node id="28000000" version="1" lon="111.0005" lat="48.0005">
    <tag k='generator:method' v='photovoltaic' />
    <tag k='generator:output:electricity' v='yes' />
    <tag k='generator:source' v='solar' />
    <tag k='generator:type' v='solar_photovoltaic_panel' />
    <tag k='generator:place' v='roof' />
    <tag k='power' v='generator' />
    </node>
*/

class tr_osm_power
{
public:
    tr_osm_power();
    virtual ~tr_osm_power();

    TrPoint getPoint() const;
    void setPoint(const TrPoint & pt);

    virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);
    virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);

private:
    static QString ms_power;
    static QString ms_generator;

    TrPoint m_pt;

    void writeXmlOsmTag(QXmlStreamWriter &xml_out, const QString & k, const QString & v);
};

#endif // TR_OSM_POWER_H
