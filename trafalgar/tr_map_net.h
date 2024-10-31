/******************************************************************
 *
 * @short	directed graph
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapNet
 * superclass:	TrGeoObject
 * modul:	tr_map_net.h 
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2023
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



// container for polygon objects -> graph -> road network...

#ifndef TR_MAP_NET_H
#define TR_MAP_NET_H

#include "tr_defs.h"

#include "tr_geo_object.h"

#include "tr_map_list.h"

#include "tr_map_link.h"

#ifdef TESTX
#include "tr_map_edge.h"
#endif

// just for debug
#include "tr_geo_segment.h"

#include <stdint.h>

#define LIST_DELETE true
#define LIST_CREATE false

class TrMapNet : public TrGeoObject
{
private:
	QString m_name;

	uint64_t m_mask_cmp;

	// for test issues only
	QPen m_default_pen;

protected:
	TrMapList * m_link_list;
	TrMapList * m_node_map;
	TrMapList * m_primive_map;
	TrMapList * m_complex_map;

	bool manageList(TrMapList ** list, bool del, const QString & name);
	bool createNodeInOut();

public:
	// debug segments for cross poins
	//static TrGeoSegment * ms_seg_1;
	//static TrGeoSegment * ms_seg_2;

	TrMapNet();

	virtual ~TrMapNet();

	virtual QString getName() const;

	virtual bool setName(const QString & name);

	virtual void setNameList(TrGeoObject * list);

	bool init();

	void setLayerShowMask(uint64_t mask);

	TrMapList * getNetList(uint64_t set, bool create);

	bool addPen(const QString & group, int idx, const QPen & pen);

	bool appendObjectBrush(int idx, QBrush brush);

	void appendLink(TrMapLink * link);

	bool addNode(uint64_t id);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);

	bool setSurroundingRect();

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos, uint64_t mask);

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	// TODO: remove...?
	uint64_t editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids);

	TrGeoObject * manageGap(const TrZoomMap & zoom_ref, TrMapLink & link, uint8_t mode,
			const TrPoint & pt, TrGeoObject * obj = nullptr);

	virtual void setMask(uint64_t bit_mask);

	virtual void removeMask(uint64_t bit_mask);

#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif	// TR_MAP_NET_H

