/******************************************************************
 *
 * @short	Park Lane
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapParkLane
 * superclass:	TrGeoObject
 * modul:	tr_map_parklane.cc    park lane
 * @version
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2024-2024
 *
 * beginning:	12.2024
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

#include "tr_map_parklane.h"

#include "tr_map_link_road.h"

TrMapParkLane::TrMapParkLane()
	: TrGeoObject()
	, m_parking(0)
	, m_pen_park(nullptr)
	, m_ref(nullptr)
{
}

TrMapParkLane::~TrMapParkLane()
{
}

uint16_t TrMapParkLane::getParking()
{
	return m_parking;
}

void TrMapParkLane::setParking(uint16_t code)
{
	m_parking = code;
}

void TrMapParkLane::setParkingPen(uint16_t type, TrGeoObject * base)
{
	TrMapList * list = dynamic_cast<TrMapList *>(base);
	if(list == nullptr)
		return;

	//V_PARK_PARALLEL_L   0x0000000100000000
	//V_PARK_DIAGONAL_L   0x0000000200000000
	//V_PARK_PERPENDI_L   0x0000000400000000
	// FLAG_PARKING       0x0000000000100000
	// V_PARK_PARALLEL_R  0x0000010000000000
	// V_PARK_DIAGONAL_R  0x0000020000000000
	// V_PARK_PERPENDI_R  0x0000040000000000

	// FLAG_PARKING_R     0x0000000000200000
	// FLAG_PARKING_L     0x0000000000400000
	// FLAG_PARKING_B     0x0000000000800000
	//                           102006

	if(type & 0x0001)	// parking_par
		m_pen_park = list->getObjectPen(0x2002);
	if(type & 0x0002)	// parking_dia
		m_pen_park = list->getObjectPen(0x2004);
	if(type & 0x0004)	// parking_per
		m_pen_park = list->getObjectPen(0x2003);
	if(type & 0x0030)	// parking_no
		m_pen_park = list->getObjectPen(0x2001);
	if(m_pen_park != nullptr)
	{
		//TR_INF << HEX << type;
		m_pen_park->setStyle(Qt::DotLine);
		m_pen_park->setWidth(5);
    }
}

void TrMapParkLane::setLinkRef(TrGeoObject *ref)
{
	m_ref = ref;
}

QPen * TrMapParkLane::getParkPen()
{
	return m_pen_park;
}

bool TrMapParkLane::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	if(ctrl & TR_INIT_COLORS)
	{
		setParkingPen(m_parking >> 8, base);
	}	
	return true;
}

void TrMapParkLane::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	if(m_ref == nullptr)
		return;
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return;
	if((m_parking & 0xff00) && (s_mask & TR_MASK_SHOW_PARKING) && m_pen_park != nullptr)
	{
		if(link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			TrPoint pt_to = link->getCrossingPoint(false);
			TrPoint pt_from = link->getCrossingPoint(true);

			p->setPen(*m_pen_park);
			if(link->getPolygon() == nullptr)
			{
				QPolygon poly(2);
				link->getTwoLine(zoom_ref, poly);
				p->drawPolyline(poly);
			}
			else
			{
				if(link->getOneWay() & TR_LINK_DIR_BWD)
					link->getPolygon()->draw(zoom_ref, p, pt_to, pt_from, 2);
				else
					link->getPolygon()->draw(zoom_ref, p, pt_from, pt_to, 2);
			}
		}
		// TODO: two pens for base and parallel line on oneway links?
		if(link->isAsDoubleLine() && (s_mask & TR_MASK_MORE_LINES ))
		{
			p->setPen(*m_pen_park);
			QVector<QPointF> vptf;
			link->getParScreenLine(zoom_ref, vptf);
			if(vptf.size() > 1)
				p->drawPolyline(vptf);
		}
	}
}

bool TrMapParkLane::setSurroundingRect()
{
	return false;
}


#ifdef TR_SERIALIZATION
uint64_t TrMapParkLane::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TR_NO_VALUE;
}

void TrMapParkLane::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
}
#endif
