/******************************************************************
 *
 * @short	Park Lane
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapParkLane
 * superclass:	TrGeoObject
 * modul:	tr_map_parklane.h    park lane
 * @version
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2024-2024
 *
 * beginning:	12.2024
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

#ifndef TR_MAP_PARKLANE_H
#define TR_MAP_PARKLANE_H

#include "tr_geo_object.h"

class TrMapParkLane : public TrGeoObject
{
public:
	TrMapParkLane();
	virtual ~TrMapParkLane();

	uint16_t getParking();

	void setParking(uint16_t code);

	QPen *getParkPen();

	void setParkingPen(uint16_t type, TrGeoObject *base);

	void setLinkRef(TrGeoObject * ref);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);	

	bool setSurroundingRect();

#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif

private:
	uint16_t m_parking;
	QPen * m_pen_park;
	TrGeoObject * m_ref;
};

#endif // TR_MAP_PARKLANE_H
