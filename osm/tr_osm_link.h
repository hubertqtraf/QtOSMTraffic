/******************************************************************
 * project:	Trafalgar/View
 *
 * class:	TrOsmLink
 * superclass:	---
 * modul:	tr_osm_link.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2016
 *
 * (C)		Schmid Hubert 2016-2025
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

#ifndef TR_OSM_LINK_H
#define TR_OSM_LINK_H

#include <QtCore/qvector.h>

#include <stdint.h>
#include <tr_map_link.h>

#include "tr_map_link_road.h"

class TrOsmLink : public TrMapLinkRoad
{
private:
	uint32_t m_osm_lanes;
	uint64_t m_osm_parking;
	uint8_t m_osm_placement;
	uint32_t m_osm_sidewalk;

	uint32_t m_osm_width;

	QVector <uint64_t> m_raw_nodes;

protected:

public:
	TrOsmLink();

	virtual ~TrOsmLink();

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	void setOsmLanes(uint32_t lanes);

	uint32_t getOsmWidth();

	void setOsmWidth(uint32_t width);

	uint32_t getOsmLanesFwd();

	uint32_t getOsmLanesBwd();

	void setOsmParking(uint64_t park);

	uint64_t getOsmParkingFwd();

	uint64_t getOsmParkingBwd();

	void setOsmPlacement(uint8_t placement);

	uint8_t getOsmPlacement();

	uint32_t getOsmSidewalk();

	void setOsmSidewalk(uint32_t side);

	TrMapLink * cloneLink(bool fwd, bool sideway, bool is_road);

	QVector <uint64_t> * getRawNodes();

	void addRawNode(uint64_t raw_node);

	void clearRawNodes();
};

#endif // TR_OSM_LINK_H

