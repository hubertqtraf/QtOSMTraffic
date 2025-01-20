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
 * @author	Schmid Hubert (C)2023-2025
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

	QString getPoiName();

	virtual void setLayerShowMask(uint64_t mask);

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	bool setSurroundingRect();

#ifdef TR_SERIALIZATION
	QString getXmlDescription();

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_POI_H
