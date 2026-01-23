/******************************************************************
 *
 * @short	directed graph for roads
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapNetRoad
 * superclass:	TrMapNet
 * modul:	tr_map_net_road.cc
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


#include "tr_map_net_road.h"

#include "tr_map_node.h"

#include "tr_name_element.h"

TrMapList * TrMapNetRoad::ms_name_list = nullptr;

TrMapNetRoad::TrMapNetRoad()
	: TrMapNet()
	, m_mask_cmp(TrGeoObject::s_mask)
{
}

TrMapNetRoad::~TrMapNetRoad()
{
}

void TrMapNetRoad::setNameList(TrGeoObject * list)
{
	ms_name_list = dynamic_cast<TrMapList *>(list);
}


bool TrMapNetRoad::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	bool ret = TrMapNet::init(zoom_ref, ctrl, base);

	if(m_link_list == nullptr)
	{
		TR_WRN << "m_link_list == nullptr";
		return false;
	}

	// code to move the oneway link
	if(s_mask & TR_MASK_MOVE_LINE)
	{
		TR_INF << "only one shift!";
		// TODO: set the angles -> check: double use?
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_MV_CLEAN, nullptr);
		m_node_map->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_ND_RESET, nullptr);
		m_node_map->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_ND_ANG, m_primive_map);
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_MV_PAR, nullptr);	// mode base line to the side
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_MV_BASE, nullptr);    // moveBaseLine
		m_node_map->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_ND_MV, m_primive_map);
		m_node_map->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_ND_SET, nullptr);
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 35, nullptr);	// handleSmallElement, remove unneeded points

		// set the link point data from moved node
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 50, nullptr);

		s_mask &= ~(TR_MASK_MOVE_LINE);
	}

	if(!(s_mask & TR_MASK_SET_RAMPS))
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 50, nullptr);
	// code for double line
	m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 14, nullptr);

	// set shadow node on divider
	m_link_list->init(zoom_ref, 10, m_node_map);

	// TODO: check...
	m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 11, m_node_map);

	for (size_t i = 0; i < m_link_list->objCount(); ++i)
	{
		// TODO: check double use! check -> getMovePoint
		// TODO: merge initSecs and initDoubleLine?
#ifdef TR_EDGE
		TrMapEdge * edge = dynamic_cast<TrMapEdge *>(m_link_list->getVecObject(i));
		if(edge != nullptr)
		{
			edge->initSecs(zoom_ref, m_primive_map, *m_node_map);
		}
#endif
	}
	if((s_mask & TR_MASK_SET_RAMPS) != (m_mask_cmp & TR_MASK_SET_RAMPS))
	{
		TR_MSG << "TR_MASK_SET_RAMPS changed";
		// TODO: add time consuming init code...

		m_mask_cmp = s_mask;
	}

	// set angles
	m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 11, nullptr);

	// code for checking the ramps and set cross points
	if(s_mask & TR_MASK_SET_RAMPS)
	{
		m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 12, nullptr);
		m_node_map->init(zoom_ref, TR_INIT_GEOMETRY | TR_INIT_ND_CROSS, m_primive_map);
	}

	// TODO: code = 20 -> 'setRampMode' only edges?
	m_link_list->init(zoom_ref, TR_INIT_GEOMETRY | 20, nullptr);
	return ret;
}

void TrMapNetRoad::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrMapNet::draw(zoom_ref, p, mode);
}

bool TrMapNetRoad::setSurroundingRect()
{
	return TrMapNet::setSurroundingRect();
}


uint64_t TrMapNetRoad::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	return TrMapNet::findSelect(zoom_ref, inside, pos);
}

void TrMapNetRoad::setMask(uint64_t bit_mask)
{
	TrMapNet::setMask(bit_mask);
}

void TrMapNetRoad::removeMask(uint64_t bit_mask)
{
	TrMapNet::removeMask(bit_mask);
}

#ifdef TR_SERIALIZATION
uint64_t TrMapNetRoad::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TrMapNet::readXmlDescription(xml_in);
}

void TrMapNetRoad::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	TrMapNet::writeXmlDescription(xml_out, id);
}
#endif
