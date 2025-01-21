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
 * @author	Schmid Hubert (C)2024-2025
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

#define DEF_WITH_P   500
#define DEF_WITH_M  -500

#include "tr_defs.h"

#include "tr_geo_poly.h"
#include "tr_geo_segment.h"
#include "tr_map_node.h"

#ifdef TR_SERIALIZATION
#include "tr_map_assembly.h"
#endif
#include "tr_map_link.h"

#include "tr_map_list.h"

#include "tr_map_parklane.h"

class TrMapLinkRoad : public TrMapLink
{
private:
	// first attempt -> subclass of geopoly?
	QVector<TrPoint> m_par_line;

	// TODO: should be part of assembly
	uint8_t m_lanes;

	uint8_t m_placement;

	// could be one lane or a list
	TrGeoObject * m_parking;

	int32_t m_mm_calc_width;

	// TODO: new road class?
	QPen m_pen_para;
	// TODO: memmory?

	void getNodePoints(TrPoint & pt1, TrPoint & pt2);

	bool getMoveCrossPoint(const TrZoomMap & zoom_ref);

	bool moveBaseLine(const TrZoomMap & zoom_ref);

	void setMoveParLine(const TrZoomMap & zoom_ref);

	void drawParLine(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);

	void triCross(const TrZoomMap & zoom_ref, TrMapNode & node);

	bool setCrossingPoint(TrPoint & pt, bool dir);

protected:
	// copy of moved position of the node
	TrPoint m_pt_from;
	TrPoint m_pt_to;

	int moveCheck(TrMapNode & node);
	TrMapLinkRoad * getNextLink(TrMapNode & node, int n, bool dir, double & ang);

public:
	TrMapLinkRoad();
	virtual ~TrMapLinkRoad();

	friend QDebug operator<<(QDebug dbg, const TrMapLinkRoad& link);

	static int ms_lane_width_p;
	static int ms_lane_width_n;

	virtual QString getXmlName() const;

	void setLanes(uint32_t lanes);

	uint8_t getLanes();

	void setPlacement(uint8_t placement);

	uint8_t getPlacement();

	void setParking(uint16_t park);

	uint16_t getParking();

	int32_t getRoadWidth();

	TrPoint getCrossingPoint(bool dir);

	void getParScreenLine(const TrZoomMap & zoom_ref, QVector<QPointF> & pointPairs);

	virtual bool getSegment(TrGeoSegment & seg, bool dir, bool par);

	virtual bool getParSegment(const TrZoomMap & zoom_ref, poly_add & add, bool dir);

	virtual TrGeoObject * getSegmentWithParm(TrGeoSegment & segment, int64_t nd_id, bool dir);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void initDoubleLine(const TrZoomMap & zoom_ref, QVector<TrPoint> &m_par_line, int32_t width);

	void initDoubleLineWidth(const TrZoomMap & zoom_ref);

	virtual void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

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

