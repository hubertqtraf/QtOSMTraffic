/******************************************************************
 *
 * @short	road link class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapLinkRoad
 * superclass:	TrMapLink
 * modul:	tr_map_link_road.h
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	05.2024
 *
 * @author	Schmid Hubert (C)2024-2024
 *
 * history:
 *
 ******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software foundation; either version 2, or (at your
   option) any later version.

   The GNU trafalgar package is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with the trafalgar package; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
   Boston, MA 02110-1301, USA. */


// network_link -> connection with start node, end node, polygon (or something else?) and attrinutes

#ifndef TR_MAP_LINK_ROAD_H
#define TR_MAP_LINK_ROAD_H

// it was 3500 -> 3.5m
#define LANE_WITH_P  3200
#define LANE_WITH_M  -3200

#define DEF_WITH_P   500
#define DEF_WITH_M  -500

#include "tr_defs.h"

#include "tr_geo_poly.h"
#include "tr_geo_segment.h"
#include "tr_map_node.h"

#ifdef TESTX
#include "tr_map_assembly.h"
#endif
#include "tr_map_link.h"

#include "tr_map_list.h"

class TrMapLinkRoad : public TrMapLink
{
private:
	// first attempt -> subclass of geopoly?
	QVector<TrPoint> m_par_line;

	// TODO: should be part of assembly
	uint8_t m_lanes;

	uint16_t m_parking;

	int32_t m_mm_calc_width;

	QPen m_pen_para;
	// TODO: memmory?

	QPen * m_pen_park;

	void setParkingPen(uint16_t type, TrGeoObject * base);

	void getNodePoints(TrPoint & pt1, TrPoint & pt2);

	void getParScreenLine(const TrZoomMap & zoom_ref, QVector<QPointF> & pointPairs);

	bool getMoveCrossPoint(const TrZoomMap & zoom_ref);

	bool moveBaseLine(const TrZoomMap & zoom_ref);

	void setMoveParLine(const TrZoomMap & zoom_ref);

	void drawParLine(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);

	void triCross(const TrZoomMap & zoom_ref, TrMapNode & node);

protected:
	int moveCheck(TrMapNode & node);
	TrMapLinkRoad * getNextLink(TrMapNode & node, int n, bool dir, double & ang);

public:
	TrMapLinkRoad();
	virtual ~TrMapLinkRoad();

	friend QDebug operator<<(QDebug dbg, const TrMapLinkRoad& link);

	virtual QString getXmlName() const;

	void setLanes(uint32_t lanes);

	uint8_t getLanes();

	void setParking(uint16_t park);

	uint16_t getParking();

	virtual bool getSegment(TrGeoSegment & seg, bool dir, bool par);

	virtual bool getParSegment(const TrZoomMap & zoom_ref, poly_add & add, bool dir);

	virtual TrGeoObject * getSegmentWithParm(TrGeoSegment & segment, int64_t nd_id, bool dir);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void initDoubleLine(const TrZoomMap & zoom_ref, int32_t width);

	void initDoubleLineWidth(const TrZoomMap & zoom_ref);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	bool setSurroundingRect();

	bool setRamp(const TrZoomMap & zoom_ref, bool dir);

	bool checkRamps(const TrZoomMap & zoom_ref, bool do_reset);

	virtual bool getParPoint(bool first, TrPoint & pt);
	virtual bool setParPoint(bool first, TrPoint & pt);

#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_LINK_ROAD_H

