/******************************************************************
 *
 * @short	segment class, defined by two points
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoSegment
 * superclass:	TrGeoObject
 * modul:	tr_geo_segment.h	geometric selectable object
 * @version	0.1
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2023-2025
 *
 * beginning:	10.2023
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

#ifndef TR_GEO_SEGMENT_H
#define TR_GEO_SEGMENT_H

#include "tr_geo_object.h"

#include <geo_poly.h>
#include <tr_geo_poly.h>

class TrGeoSegment : public TrGeoObject
{
private:
	TrPoint m_first;
	TrPoint m_second;
	//static bool checkAngle(poly_add & pa, poly_add & pa1, double angle_b, double angle_a);

public:
	TrGeoSegment();
	TrGeoSegment(TrPoint first, TrPoint second);
	virtual ~TrGeoSegment();

	//TrGeoSegment(const TrGeoSegment& other);

	friend QDebug operator<<(QDebug dbg, const TrGeoSegment& seg);
	//friend QDataStream& operator << (QDataStream&, TrGeoSegment);

	QString getXmlName() const;

	void setPoints(TrPoint first, TrPoint second);

	void setPoints(TrGeoPolygon & poly, size_t pos);

	void setPoints(const TrGeoSegment& other);

	TrPoint getFirstPoint();

	TrPoint getSecondPoint();

	bool hasData();

	void setFirstPoint(TrPoint & pt);

	void setSecondPoint(TrPoint & pt);

	TrPoint getScreenPoint(const TrZoomMap & zoom_ref, bool select);

	bool setPointsFromPoly(TrGeoPolygon & poly, uint64_t idx, bool dir);

	bool getCrossPoint(const TrZoomMap & zoom_ref, TrPoint & pt, TrGeoSegment & other);

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	bool setSurroundingRect();

	void clearData();

	bool getSegmentData(const TrZoomMap & zoom_ref, poly_add & add, double shift);

	double getLength(const TrZoomMap & zoom_ref);

	double getAngle(const TrZoomMap & zoom_ref) const;

	void doReverse();

	int getAngleCode(const TrZoomMap & zoom_ref, const TrGeoSegment& other);

	void getSegList(QList<TrGeoSegment> & seg_list, TrGeoPolygon & poly);

	bool managePolygon(const TrZoomMap & zoom_ref, TrGeoPolygon & poly,
			QList<TrGeoSegment> & seg_list, int width);

	bool isEvenPolygon(const TrZoomMap & zoom_ref, QList<TrGeoSegment> & seg_list, double ctrl);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);
#ifdef TR_SERIALIZATION
	virtual bool exportGeoJson(QJsonObject & geojson, uint64_t mode);

	bool importArrayJson(const QJsonArray & arrjson, uint64_t mode);

	virtual bool importGeoJson(const QJsonObject & geojson, uint64_t mode);

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif

};

#endif // TR_GEO_SEGMENT_H
