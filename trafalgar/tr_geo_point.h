/******************************************************************
 *
 * @short	point class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoint
 * superclass:	TrGeoObject
 * modul:	tr_geo_point.h	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2021-2025
 *
 * beginning:	05.2021
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

#ifndef TR_GEO_POINT_H
#define TR_GEO_POINT_H

#include "tr_geo_object.h"
#include "tr_map_list.h"

#define SELECT_SIZE 6
#define DRAW_SIZE 4

//used in m_view_opt
#define TR_POINT_MOVE       0x80
#define TR_POINT_MOVE_CROSS 0x40

// TODO: unused for now...
//#define TR_POINT_ONEWAY 0x01
//#define TR_POINT_REVERS 0x02

class TrGeoPoint : public TrGeoObject
{
private:
	uint8_t m_view_opt;

	QPen * stdPen;

	bool readXmlPoint(QXmlStreamReader * xml_in);

protected:
	TrPoint m_pt;
	int64_t m_geo_id;
	TrPointRef * m_pt_ref;

	void writeXmlPointPart(QXmlStreamWriter & xml_out, uint64_t id);
	uint64_t readXmlPointData(QXmlStreamReader & xml_in, const QString & block);

public:
	TrGeoPoint();

	TrGeoPoint (const TrGeoPoint& other);

	virtual ~TrGeoPoint();

	friend QDebug operator<<(QDebug dbg, const TrGeoPoint& point);

	QString getXmlName() const;

	int64_t getGeoId() const;

	uint8_t getViewOpt();

	void setViewOpt(uint8_t opt);

	void setGeoId(int64_t gid);

	void setPointRef(const TrPointRef & ref);

	TrPointRef * getPointRef() const;

	void resetRef();

	TrPoint getPoint() const;

	void setPoint(const TrPoint & point);

	void resetIoOut();

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	void drawText(const TrZoomMap & zoom_ref, QPainter * p, const QString & text);

	void drawElement(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode);

	virtual void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	static bool isCloseToPoint(const TrZoomMap & zoom_ref, const TrPoint & inside, const TrPoint & pos);

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	bool setSurroundingRect();

	void setInfo(const TrZoomMap & zoom_ref);

#ifdef TR_SERIALIZATION
	QString getXmlDescription();

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_GEO_POINT_H
