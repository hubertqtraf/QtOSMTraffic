/******************************************************************
 *
 * @short	directed graph for roads
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapNetRoad
 * superclass:	TrMapNet
 * modul:	tr_map_net_road.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2021
 *
 * @author	Schmid Hubert (C)2021-2025
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

#ifndef TR_MAP_NET_ROAD_H
#define TR_MAP_NET_ROAD_H

#include "tr_defs.h"

#include "tr_geo_object.h"

#include "tr_map_list.h"
#include "tr_map_link.h"
#include "tr_map_net.h"

#include <stdint.h>

class TrMapNetRoad : public TrMapNet
{
private:
	uint64_t m_mask_cmp;

public:
	// TODO: name list for only roads?
	static TrMapList * ms_name_list;

	TrMapNetRoad();

	virtual ~TrMapNetRoad();

	TrMapList * getNetList(uint64_t set, bool create);

	void appendLink(TrMapLink * link);

	bool addNode(uint64_t id);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void setNameList(TrGeoObject * list);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode);

	bool setSurroundingRect();

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	virtual void setMask(uint64_t bit_mask);

	virtual void removeMask(uint64_t bit_mask);
#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif	// TR_MAP_NET_ROAD_H

