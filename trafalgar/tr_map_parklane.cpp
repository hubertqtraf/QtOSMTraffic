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
 * @author	Schmid Hubert (C)2024-2025
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
	, m_pen_park_left(nullptr)
	, m_ref(nullptr)
{
}

TrMapParkLane::~TrMapParkLane()
{
}

QDebug operator<<(QDebug dbg, const TrMapParkLane& lane)
{
	return dbg << lane.getXmlName() << " " << HEX << " parking " << lane.m_parking;
}

QString TrMapParkLane::getXmlName() const
{
	return "map_lane_park";
}

uint64_t TrMapParkLane::getParking()
{
	return m_parking;
}

void TrMapParkLane::setParking(uint64_t code)
{
	m_parking = code;
}

QPen * TrMapParkLane::setParkingSidePen(uint16_t type, TrGeoObject * base)
{

	QPen * ret = nullptr;

	//if((type & 0x0001) != 0x0001)
	//	return ret;

	type &= 0xfffe;

	TrMapList * list = dynamic_cast<TrMapList *>(base);
	if(list == nullptr)
		return ret;

	if((type & 0x00f0) == V_PARK_PARALLEL)
		ret = list->getObjectPen(0x2002);
	if((type & 0x00f0) == V_PARK_DIAGONAL)
		ret = list->getObjectPen(0x2004);
	if((type & 0x00f0) == V_PARK_PERPENDI)
		ret = list->getObjectPen(0x2003);
	if((type & 0x00f0) == V_PARK_SEPARATE)
		ret = list->getObjectPen(0x2005);
	if((type & 0x000e) & V_PARK_NO)
		ret = list->getObjectPen(0x2001);
	if((type & 0x000e) & V_PARK_NO_STOP)
		ret = list->getObjectPen(0x2001);

	if(ret != nullptr)
	{
		ret->setStyle(Qt::DotLine);
		ret->setWidth(5);
	}
	return ret;
}

int32_t TrMapParkLane::setParkingWidth(uint16_t type)
{
	if((type & 0x00f0) == V_PARK_PARALLEL)
		return 1500;
	if((type & 0x00f0) == V_PARK_DIAGONAL)
		return 3000;
	if((type & 0x00f0) == V_PARK_PERPENDI)
		return 5000;
	if((type & 0x00f0) == V_PARK_SEPARATE)
		return 4000;
	if(type & 0x0030)	// parking_no
		return 500;
	return 777;
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
	if(m_ref == nullptr)
		return false;
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return false;
	//if(link->getNodeFrom() == 9400840110)

	if(ctrl & TR_INIT_COLORS)
	{
		//TR_INF << HEX << ((m_parking >> (4+32)) & 0x000000000000000fU) + 0x2000 << m_parking;
		m_pen_park = setParkingSidePen((m_parking & 0x00000000000000ffU), base);
		if(link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			m_pen_park_left = setParkingSidePen((m_parking >> 32) & 0x00000000000000ffU, base);
		}
	}
	if(ctrl & TR_INIT_GEOMETRY)
	{
		if((m_ref != nullptr) && (m_parking))
		{
			if(link != nullptr)
			{
				//int cl = link->getRdClass();
				int32_t w = getWith(m_parking & 0x00000000000000ffU); //m_parking >> 8);
				//int32_t
				//w = 2000;
				if(w)
					link->initDoubleLine(zoom_ref, m_par_line, w);
				w = -setParkingWidth(2); //m_parking & 0x00ff);
				//w = -2000;
				link->initDoubleLine(zoom_ref, m_par_left_line, w);
			}
		}
	}
	return true;
}

void TrMapParkLane::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	if(m_ref == nullptr)
		return;
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return;
	QVector<QPointF> pointPairs;

	if(/*(m_parking & 0xff00) &&*/ (s_mask & TR_MASK_SHOW_PARKING))
	{
		for(int i = 0; i<m_par_line.size(); i++)
		{
			TrPoint screenx = m_par_line[i];
			zoom_ref.setMovePoint(&screenx.x, &screenx.y);
			QPointF ptf;
			ptf.setX(screenx.x);
			ptf.setY(screenx.y);
			pointPairs.append(ptf);
		}
		if((pointPairs.size() > 1) && (m_pen_park != nullptr))
		{
			p->setPen(*m_pen_park);
			p->drawPolyline(pointPairs);
		}
		if(link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			pointPairs.clear();
			for(int i = 0; i<m_par_left_line.size(); i++)
			{
				TrPoint screenx = m_par_left_line[i];
				zoom_ref.setMovePoint(&screenx.x, &screenx.y);
				QPointF ptf;
				ptf.setX(screenx.x);
				ptf.setY(screenx.y);
				pointPairs.append(ptf);
			}
			if((pointPairs.size() > 1) && (m_pen_park_left != nullptr))
			{
				p->setPen(*m_pen_park_left);
				p->drawPolyline(pointPairs);
			}
		}
	}
}

bool TrMapParkLane::setSurroundingRect()
{
    return false;
}

int32_t TrMapParkLane::getWith(uint8_t code)
{
	int32_t w = 0;

	if(m_ref == nullptr)
		return 0;
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return 0;
	if(m_parking == 0)
		return 0;
	w = link->getWidth();
	int32_t w_r = link->getRoadWidth();
	if(w_r < 0)
	{
		return (w_r - setParkingWidth(code)); //m_parking >> 8));
	}
	// TODO: check 'getWidth()' -> 'width' option usefull?
	//if(w_r > w)
	{
		w = w_r + setParkingWidth(code); //m_parking >> 8);
	}
	//if(link->getOneWay() & TR_LINK_DIR_BWD)
	//	w = 0 - w;

	return w;
}


#ifdef TR_SERIALIZATION
uint64_t TrMapParkLane::readXmlDescription(QXmlStreamReader & xml_in)
{
	QXmlStreamAttributes attr = xml_in.attributes();

	bool ok = false;
	m_parking = attr.value("", "parking").toInt(&ok, 16);
	if(!ok)
	{
		if(attr.value("", "parking") != QString(""))
			TR_WRN << "parking: " << attr.value("", "parking") << m_parking;
		return TR_NO_VALUE;
	}
	return 0;
}

void TrMapParkLane::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());
	QString hex;

	hex.setNum(m_parking, 16);
	xml_out.writeAttribute("parking", "0x" + hex);
	xml_out.writeEndElement();
}
#endif
