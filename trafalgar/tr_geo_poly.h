/******************************************************************
 *
 * @short	polygon class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPolygon
 * superclass:	TrGeoObject
 * modul:	tr_geo_poly.h	geometric selectable object	
 * @version	0.1
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2009-2025
 *
 * beginning:	06.2009
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

#ifndef TR_GEO_POLY_H
#define TR_GEO_POLY_H

#include "tr_geo_object.h"

#include <geo_poly.h>


#define TR_POLY_TYPE_DEF     0x0001000000000000
#define TR_POLY_SHOW_ROUTE   0x0040000000000000
#define TR_POLY_SHOW_TRACK   0x0020000000000000
#define TR_POLY_SHOW_ANG_ERR 0x0080000000000000

#define TR_POLY_RW_CONSTRUCT 0x0000000000000008
#define TR_POLY_RW_DEF       0x0000000000000001

class TrGeoPolygon : public TrGeoObject 
{
private:
	QPen * stdPen;
    poly_base m_base;

	bool readXmlPoint(QXmlStreamReader & xml_in, QVector<TrPoint> & poly_points);

	static bool checkAngle(poly_add & pa, poly_add & pa1, double angle_b, double angle_a);

public:
	TrGeoPolygon();
	virtual ~TrGeoPolygon();

	friend QDebug operator<<(QDebug dbg, const TrGeoPolygon& poly);

	void clearData();

	size_t getSize();

	bool hasSegmentInfo();

	int static calcParPoint(const TrZoomMap & zoom_ref, TrPoint & pt, straight_line & seg, int32_t width);

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	TrPoint getPoint(size_t id);

	TrPoint getFirstPoint();

	TrPoint getLastPoint();

	bool getPoints(QVector<TrPoint> & points);

	void getScreenPoints(const TrZoomMap & zoom_ref, QPolygon & poly);

	const poly_add & getGeoSegmentData(size_t id);

	bool setPoint(size_t id, TrPoint & pt);

	// TODO: check the functions - remove?
	double getLength(const TrZoomMap & zoom_ref);

	double getLength(const TrZoomMap & zoom_ref, TrPoint pta, TrPoint ptb);

	double getSegmentsLength();

	void createSelectPoints();

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	void draw(const TrZoomMap & zoom_ref, QPainter * p, TrPoint from, TrPoint to, uint8_t mode);

	//void setCenter(double x, double y);

	void appendPoints(const QVector<TrPoint> & next_points);

	bool setSurroundingRect();

	void setInfo(const TrZoomMap & zoom_ref);

	static void setInfoSect(const TrZoomMap & zoom_ref, poly_add & sec,
		TrPoint first_point, TrPoint second_point);

	int parallel(TrGeoPolygon * base, const TrZoomMap & zoom_ref, int32_t width);

	int parallel(QVector<TrPoint> * par_line, const TrZoomMap & zoom_ref,
		straight_line first, straight_line last, int32_t width);

	uint64_t findSelectInside(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	bool removePoint(uint64_t id);

	bool addPoint(const TrZoomMap & zoom_ref, const TrPoint & pt);

	double getClosest(const TrZoomMap & zoom_ref, TrPoint & pt, int & seg, double & rel_dist);
#ifdef TR_SERIALIZATION
	QString getXmlDescription();

	virtual bool exportGeoJson(QJsonObject & geojson, uint64_t mode);

	bool importArrayJson(const QJsonArray & arrjson, uint64_t mode);

	virtual bool importGeoJson(const QJsonObject & geojson, uint64_t mode);

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_GEO_POLY_H
