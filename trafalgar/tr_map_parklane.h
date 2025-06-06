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
 * @author	Schmid Hubert (C)2024-2025
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

#define PARK_CODE_R         0x0000000000000001UL
#define PARK_CODE_L         0x0000000100000000UL

#define V_PARK_NO           0x0000000000000002UL
#define V_PARK_NO_STOP      0x0000000000000004UL

#define V_PARK_PARALLEL     0x0000000000000010UL
#define V_PARK_DIAGONAL     0x0000000000000020UL
#define V_PARK_PERPENDI     0x0000000000000040UL
#define V_PARK_SEPARATE     0x0000000000000080UL

class TrMapParkLane : public TrGeoObject
{
public:
	TrMapParkLane();
	virtual ~TrMapParkLane();

	friend QDebug operator<<(QDebug dbg, const TrMapParkLane& link);

	QString getXmlName() const;

	uint64_t getParking();

	void setParking(uint64_t code);

	QPen *getParkPen();

	int32_t setParkingWidth(uint16_t type);

	void setLinkRef(TrGeoObject * ref);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	bool setSurroundingRect();

#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif

private:
	uint64_t m_parking;
	QPen * m_pen_park;
	// own object -> list of parklane objects?
	QPen * m_pen_park_left;
	TrGeoObject * m_ref;
	QVector<TrPoint> m_par_line;
	// own object -> list of parklane objects?
	QVector<TrPoint> m_par_left_line;

	int32_t getWith(uint8_t code);
	QPen * setParkingSidePen(uint16_t type, TrGeoObject * base);
};

#endif // TR_MAP_PARKLANE_H
