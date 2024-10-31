/******************************************************************
 *
 * @short	point of interest class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoi
 * superclass:	TrGeoPoint
 * modul:	tr_geo_poi.h	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2023-2024
 *
 * beginning:	03.2023
 *
 * history:
 */
/******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software foundation; either version 2, or (at your
 * option) any later version.
 *
 * The GNU trafalgar package is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with the GNU plotutils package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef TR_MAP_POI_H
#define TR_MAP_POI_H

#include "tr_geo_point.h"
#include "tr_map_list.h"

#define SELECT_SIZE 6
#define DRAW_SIZE 4

// TODO: this is a copy of the osm filter (osm_load.h)!
// used 'act_world->way_flags' or 'act_world->node_flags'
#define TYPE_ROAD           0x0000000000010000
#define TYPE_RAIL           0x0000000000020000
#define TYPE_STREAM         0x0000000000040000
// https://wiki.openstreetmap.org/wiki/DE:Key:aerialway
// aerialway -> drag_lift/cable_car/gondola etc. / goods
#define TYPE_LIFT           0x0000000000080000
#define TYPE_BUILDING       0x0000000000100000
#define TYPE_LANDUSE        0x0000000000200000
#define TYPE_NATURAL        0x0000000000400000
// was 'amenity': replace 'place_of_worship'/'shop'/'cafe' etc.
#define TYPE_PUBLIC         0x0000000000800000
// 'leisure -> public'
//#define TYPE_LEISURE      0x0000000001000000
// gate etc.
#define TYPE_RESTRICT       0x0000000001000000
#define TYPE_ADMIN          0x0000000002000000

#define TYPE_POWER          0x0000000004000000

// defines for POI
// natural
#define TYPE_POI_N_PEAK     0x0000000100000000
#define TYPE_POI_N_TREE     0x0000000200000000
#define TYPE_POI_N_CAVE     0x0000000400000000

// public
#define TYPE_POI_P_PARKING  0x0000001000000000
#define TYPE_POI_P_CHURCH   0x0000002000000000
// anything to eat or drink
#define TYPE_POI_P_FOOD     0x0000004000000000
// ATM or bank
#define TYPE_POI_P_MONEY    0x0000008000000000
// doctor or phrarm.
#define TYPE_POI_P_MED      0x0000010000000000
#define TYPE_POI_P_SHOP     0x0000020000000000
#define TYPE_POI_P_SPORT    0x0000040000000000
// hotel, app. etc.
#define TYPE_POI_P_GUEST    0x0000080000000000
#define TYPE_POI_P_FUEL     0x0000100000000000
// info, souvenir, etc
#define TYPE_POI_P_TOUR     0x0000200000000000
// bench, viewpoint, etc
#define TYPE_POI_P_NICE     0x0000400000000000

// TODO: check: move to 'data' value
#define TYPE_POI_P_ALPINE   0x0000800000000000

// TODO: TMP! use only village (greek version)
#define TYPE_POI_A_VILL     0x0001000000000000


class TrMapPoi : public TrGeoPoint
{
private:
	QString m_name;
	uint64_t m_poi_flags;
	uint64_t m_poi_data;

protected:

public:
	TrMapPoi();

	virtual ~TrMapPoi();

	friend QDebug operator<<(QDebug dbg, const TrMapPoi& poi);

	QString getXmlName() const;

	void setPoiTypeFlags(const uint64_t flags);

	uint64_t getPoiTypeFlags() const;

	void setPoiNumData(const uint64_t data);

	uint64_t getPoiNumData() const;

	void setPoiName(const QString & name);

	virtual void setLayerShowMask(uint64_t mask);

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	bool setSurroundingRect();

#ifdef TR_SERIALIZATION
    QString getXmlDescription();

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_POI_H
