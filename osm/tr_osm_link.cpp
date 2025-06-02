/******************************************************************
 * project:	Trafalgar/View
 *
 * class:	TrOsmLink
 * superclass:  ---
 * modul:	tr_osm_link.cc
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

#include "tr_osm_link.h"

#include "osm_load.h"

TrOsmLink::TrOsmLink()
	: m_osm_lanes(0x00000000)
	, m_osm_parking(0x00000000)
	, m_osm_placement(0x00)
	, m_osm_sidewalk(0x00000000)
	, m_osm_width(0x00000000)
{
}

TrOsmLink::~TrOsmLink()
{
}

bool TrOsmLink::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	return false;
}

void TrOsmLink::setOsmLanes(uint32_t lanes)
{
	m_osm_lanes = lanes;
}

uint32_t TrOsmLink::getOsmWidth()
{
	return m_osm_width;
}

void TrOsmLink::setOsmWidth(uint32_t width)
{
	m_osm_width = width;
}

uint32_t TrOsmLink::getOsmLanesFwd()
{
	if(m_osm_lanes & FLAG_LANES_F)
	{
		return 0x0000000f & (m_osm_lanes >> 16);
	}
	return 0;
}

uint32_t TrOsmLink::getOsmLanesBwd()
{
	if(m_osm_lanes & FLAG_LANES_B)
	{
		return 0x0000000f & (m_osm_lanes >> 24);
	}
	return 0;
}

void TrOsmLink::setOsmParking(uint64_t park)
{
	m_osm_parking = park;
}

uint8_t TrOsmLink::getOsmPlacement()
{
	return m_osm_placement;
}

void TrOsmLink::setOsmPlacement(uint8_t placement)
{
	m_osm_placement = placement;
}

uint32_t TrOsmLink::getOsmSidewalk()
{
	return m_osm_sidewalk;
}

void TrOsmLink::setOsmSidewalk(uint32_t side)
{
	m_osm_sidewalk = side;
}

//void TrOsmLink::cloneLink(TrMapLink * orig, bool fwd, bool sideway)
TrMapLink * TrOsmLink::cloneLink(bool fwd, bool sideway, bool is_road)
{

	TrMapLinkRoad * orig_rd = nullptr;
	TrMapLink * orig = nullptr;
	if(is_road)
	{
		orig_rd = new TrMapLinkRoad();
		orig = orig_rd;
	}
	else
		orig = new TrMapLink();

	if(orig == nullptr)
		return nullptr;

	if(sideway && getOneWay())
	{
		// OneWay and sidewalk -> create a footway for the routing
		// TODO: check...
		//if(!m_osm_sidewalk)
		//	return orig;
		//orig->setOneWay(1);
		if(orig_rd != nullptr)
			orig_rd->setLanes(1);
		orig->setRdClass(15);
		return orig;
	}
	orig->setOneWay(getOneWay());
	//orig->setLanes(getLanes());
	orig->setRdClass(getRdClass());
	orig->setNameId(getNameId());
	orig->setWidth(getOsmWidth());

	if(orig_rd != nullptr)
	{
		orig_rd->setLanes(getLanes());
		//if(!fwd)
		if(fwd)
		{
			if(getOneWay())
				orig_rd->setParking(m_osm_parking);
			else
				orig_rd->setParking(m_osm_parking & 0x00000000ffffffff);
		}
		else
		{
			orig_rd->setParking(m_osm_parking >> 32);
		}
		if(getLanes() == 0)
			orig_rd->setLanes(1);
		//TR_INF << m_osm_placement;
		orig_rd->setPlacement(m_osm_placement);
	}

	if((!getOneWay()) && (orig_rd != nullptr))
	{
		if((m_osm_lanes & (FLAG_LANES_F | FLAG_LANES_B)) == 0)
		{
			if(getLanes() > 1)
			{
				orig_rd->setLanes(getLanes() >> 1);
				//TR_MSG << "DOUBLE" << getLanes() << (getLanes() >> 1) << (osm_lanes & 0x000000000000000f);
			}
		}
		else
		{
			if((m_osm_lanes & FLAG_LANES_F) && (!(m_osm_lanes & FLAG_LANES_B)))
			{
				int lane = getLanes() - getOsmLanesFwd();
				//TR_MSG << "F" << getLanes() << "|" << getOsmLanesFwd() << getOsmLanesBwd() << lane;
				if(fwd)
				{
					orig_rd->setLanes(getOsmLanesFwd());
				}
				else
				{
					if(lane > 0)
						orig_rd->setLanes(static_cast<uint32_t>(lane));
					else
						orig_rd->setLanes(getLanes());
				}	
			}
			if((m_osm_lanes & FLAG_LANES_B) && (!(m_osm_lanes & FLAG_LANES_F)))
			{
				int lane = getLanes() - getOsmLanesBwd();
				//TR_MSG << "B" << getLanes() << "|" << getOsmLanesFwd() << getOsmLanesBwd() << lane;
				if(fwd)
				{
					if(lane > 0)
						orig_rd->setLanes(static_cast<uint32_t>(lane));
					else
						orig_rd->setLanes(getLanes());
				}
				else
				{
					orig_rd->setLanes(getOsmLanesBwd());
				}
			}
			if((m_osm_lanes & FLAG_LANES_F) && (m_osm_lanes & FLAG_LANES_B))
			{
				if(fwd)
				{
					if(getOsmLanesFwd())
					{
						orig_rd->setLanes(getOsmLanesFwd());
					}
				}
				else
				{
					if(getOsmLanesBwd())
					{
						orig_rd->setLanes(getOsmLanesBwd());
					}
				}
			}
		}
    }
    return orig;
}

QVector <uint64_t> * TrOsmLink::getRawNodes()
{
	return &m_raw_nodes;
}

void TrOsmLink::addRawNode(uint64_t next_node)
{
	m_raw_nodes.append(next_node);
}

void TrOsmLink::clearRawNodes()
{
	m_raw_nodes.clear();
}

