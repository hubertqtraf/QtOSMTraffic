/******************************************************************
 *
 * @short	link class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapLink
 * superclass:	TrGeoObject
 * modul:	tr_map_link.h
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2025
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

#ifndef TR_MAP_LINK_H
#define TR_MAP_LINK_H

// TODO: to road link...
#define TR_LINK_RAMP_FLAG     0x8000

#define TR_LINK_DIR_ONEWAY    0x01
#define TR_LINK_DIR_BWD	      0x04
#define TR_LINK_DIR_DIV       0x08

#define TR_IS_TO_DIR true
#define TR_IS_FROM_DIR false

#include "tr_defs.h"

#include "tr_geo_poly.h"
#include "tr_geo_segment.h"
#include "tr_map_node.h"

#include "tr_map_list.h"

#define TR_NET_GAP_ADD     0x01
#define TR_NET_GAP_REMOVE  0x02
#define TR_NET_GAP_REPLACE 0x04
#define TR_NET_GAP_UPDATE  0x10

class TrMapLink : public TrGeoObject
{
private:
	// width of the link defined in the file
	int32_t m_mm_load_width;

	// id of the name (street name...), 32 bit should be OK
	uint32_t m_name_id;

	bool removeSmallSeg(const TrZoomMap &zoom_ref, double l_limit, bool dir);

protected:
	TrMapNode * m_node_from;
	TrMapNode * m_node_to;
	TrGeoPolygon * m_pline;

	// store the id while the lists are loaded
	int64_t m_geo_id;

	// TODO: use virtual func getSegment, remove member?
	poly_add m_seg_from;
	poly_add m_seg_to;

	uint8_t m_one_way;

#ifdef TR_SERIALIZATION
	void writeXmlLinkPart(QXmlStreamWriter & xml_out, uint64_t id);
	uint64_t readXmlData(QXmlStreamReader & xml_in, const QString & block);
#endif
	void setLinkPen(TrGeoObject * base);

public:
	TrMapLink();
	virtual ~TrMapLink();

	friend QDebug operator<<(QDebug dbg, const TrMapLink& link);

	virtual QString getXmlName() const;

	uint16_t getRdClass();

	void setRdClass(uint16_t);

	virtual int32_t getWidth();

	virtual void setWidth(int32_t width);

	// update the oneway mode (for example edge)
	virtual void setOneWay(uint8_t dir);

	uint8_t getOneWay();

	// used at editor and osm filter
	void setNodeFrom(TrMapList * node_list, int64_t node);
	void setNodeTo(TrMapList * node_list, int64_t node);

	int64_t getNodeFrom();
	int64_t getNodeTo();

	void getTwoLine(const TrZoomMap & zoom_ref, QPolygon & poly, TrPoint pt1, TrPoint pt2);

	bool switchShadowNode(bool dir);

// TODO: use bool to the direction -> getNodeToRef(bool dir) setNodeToRef(TrMapNode * nd, bool dir)
	TrMapNode * getNodeFromRef();
	TrMapNode * getNodeToRef();

	bool getNodePoint(TrPoint & point, bool dir);

	bool setNodeRef(TrMapNode * nd, bool dir);

	void setGeoId(int64_t id);
	int64_t getGeoId();

	void setNameId(uint32_t id);
	uint32_t getNameId();
	QString getElementName();
	TrGeoObject * getNameElement();

	bool isAsDoubleLine();

	// only for osm filter
	bool appendPolyPoint(TrPoint & pt);

	TrGeoPolygon * getPolygon();

	virtual bool getSegment(TrGeoSegment & seg, bool dir, bool par);

	// TODO - check...
	void setPolygon(TrGeoPolygon * poly);

	virtual poly_add & getGeoSegmentData(const TrZoomMap & zoom_ref, bool dir);

	virtual bool getParSegment(const TrZoomMap & zoom_ref, poly_add & add, bool dir);

	TrMapLink * getParallelLink();

	double getAngle(const TrZoomMap & zoom_ref, bool dir);

	bool handleSmallElement(const TrZoomMap & zoom_ref, double a_limit, double l_limit);

	virtual bool getInsidePoint(TrPoint & point, bool dir);

	bool setPrimiveById(TrMapList * primive);

	bool reSetNodes(TrMapList * node_map);

	virtual void setLayerShowMask(uint64_t mask);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	// TODO: rework, only for edge...
	virtual bool initSecs(const TrZoomMap & zoom_ref, TrMapList * pr_list, TrMapList & nd_list);

	// TODO: virtual - needed?
	virtual void initDoubleLine(const TrZoomMap & zoom_ref, QVector<TrPoint> &m_par_line, int32_t width);

	virtual void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	double getLength();
	double getLength(const TrZoomMap & zoom_ref);

	bool setSurroundingRect();

	virtual uint8_t handleCrossing(const TrZoomMap & zoom_ref, TrGeoObject * other, TrGeoObject * node, uint8_t mode);

	TrGeoObject * manageGap(const TrZoomMap & zoom_ref, uint8_t mode, const TrPoint & pt, TrGeoObject * obj = nullptr);

	virtual bool getParPoint(bool first, TrPoint & pt);
	virtual bool setParPoint(bool first, TrPoint & pt);

	virtual bool setRamp(const TrZoomMap & zoom_ref, bool dir);
	virtual bool setCrossingPoint(TrPoint & pt, bool dir);

	virtual void setPolyPoints(TrGeoPolygon & line);

	double getClosest(const TrZoomMap & zoom_ref, TrPoint & pt, double & rel_dist);
#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_LINK_H

