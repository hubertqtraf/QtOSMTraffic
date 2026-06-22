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
 * @author	Schmid Hubert (C)2024-2026
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
#include "tr_map_net_road.h"

TrMapParkLane::TrMapParkLane()
	: TrGeoObject()
	, m_parking(0)
	, m_pen_park(nullptr)
	, m_pen_park_left(nullptr)
	, m_ref(nullptr)
	, m_showOther(true)
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

void TrMapParkLane::setParkingSPenStyle(bool mode)
{
	uint32_t type = m_parking & 0x00000000000000ffU;
	QPen * pen = m_pen_park;
	if(mode)
	{
		type = (m_parking >> 32) & 0x00000000000000ffU;
		pen = m_pen_park_left;
	}
	if(pen == nullptr)
		return;
	type &= 0xfffe;

	if(((type & 0x000e) & V_PARK_NO) || ((type & 0x000e) & V_PARK_NO_STOP))
	{
		pen->setStyle(Qt::DashLine);
		pen->setWidth(3);
		return;
	}
	if(TrGeoObject::getGlobelFlags() & TR_MASK_PARKING_MODE)
	{
		pen->setStyle(Qt::DotLine);
		pen->setWidth(5);
		return;
	}
	pen->setStyle(Qt::SolidLine);
	pen->setWidth(3);
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
	if((type & 0x000e) & V_PARK_NO)
		return 500;
	if((type & 0x000e) & V_PARK_NO_STOP)
		return 500;
	return 7777;
}


void TrMapParkLane::setLinkRef(TrGeoObject *ref)
{
	m_ref = ref;
}

QPen * TrMapParkLane::getParkPen()
{
	return m_pen_park;
}

TrMapParkLane * TrMapParkLane::getParkingObj(TrGeoObject * next)
{
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(next);
	if(link == nullptr)
		return nullptr;
	TrGeoObject *spec = link->getSpecialObj();
	if(spec == nullptr)
		return nullptr;
	TrMapParkLane * next_park = dynamic_cast<TrMapParkLane *>(spec);
	return next_park;
}

int TrMapParkLane::checkNode(const TrZoomMap & zoom_ref, TrGeoObject * node, bool side, bool dir)
{
	TrMapNode * n = dynamic_cast<TrMapNode *>(node);
	if(n == nullptr)
		return -1;
	bool ldir = false;
	double ang = n->getAngleByObj(m_ref);
	TrGeoObject *x = n->getNextObjByAngle(ang, ldir);
	if(x == nullptr)	// angle: over 0.0
		x = n->getNextObjByAngle(0.0, ldir);

	QVector<TrPoint> &line = m_par_line;
	if(side)
	{
		line = m_par_left_line;
	}

	TrMapParkLane * other = this->getParkingObj(x);
	double w_x = 0.0;
	if(other != nullptr)
	{
		w_x = fabs(other->setParkingWidth(other->getParking() & 0x00000000000000ffU) / 1000.0);
	}

	//TrMapLinkRoad * link1 = dynamic_cast<TrMapLinkRoad *>(m_ref);
	TrMapLinkRoad * link2 = dynamic_cast<TrMapLinkRoad *>(x);

	double w = 10.0;
	if(link2 != nullptr)
	{
		w = fabs(link2->getRoadWidth()/1000.0) + w_x;//wp;
	}
	if(side && (w < 0.5))
		w = 5.0;

	TrGeoSegment seg = TrGeoSegment::getSegBorder(line, dir);
	TrPoint pt = {0.0,0.0};
	if(seg.getSection(zoom_ref, pt, w_x + w, dir) == false)
	{
		m_showOther = false;
		return 1;
	}
	if(dir)
		line[0] = pt;
	else
		line[line.size()-1] = pt;
	return 0;
}


int TrMapParkLane::checkNodes(const TrZoomMap & zoom_ref, int32_t w, bool other)
{
	if(m_ref == nullptr)
		return -1;
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return -1;

	TrMapNode * n_t = dynamic_cast<TrMapNode *>(link->getNodeToRef());
	TrMapNode * n_f = dynamic_cast<TrMapNode *>(link->getNodeFromRef());
	if((n_t == nullptr) || (n_f  == nullptr))
		return -1;

	if((n_t->getConFlags() & 0x20) && (n_f->getConFlags() & 0x20)) // yellow
	{
		TrGeoSegment seg = TrGeoSegment(n_t->getPoint(), n_f->getPoint());
		if(seg.getLength(zoom_ref) < 30.0)
		{
			removeMask(TR_MASK_DRAW);
			return 1;
		}
		//TR_INF << HEX << n_t->getConFlags() << n_f->getConFlags() << *n_t << *n_f;
	}
	bool bwd = false;
	if(link->getOneWay() & TR_LINK_DIR_BWD)
		bwd = true;

	if(n_t->getConFlags() & 0x20)
	{
		checkNode(zoom_ref, n_t, other, bwd);
	}
	if(n_f->getConFlags() & 0x20)
	{
		checkNode(zoom_ref, n_f, other, !bwd);
	}
	return 0;
}

bool TrMapParkLane::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	if(m_ref == nullptr)
		return false;

	setMask(TR_MASK_DRAW);
	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return false;

	if(ctrl & TR_INIT_COLORS)
	{
		m_pen_park = setParkingSidePen((m_parking & 0x00000000000000ffU), base);
		if(link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			m_pen_park_left = setParkingSidePen((m_parking >> 32) & 0x00000000000000ffU, base);
		}
		setParkingSPenStyle(true);
		setParkingSPenStyle(false);
	}
	if(ctrl & TR_INIT_GEOMETRY)
	{
		if((ctrl & 0xff) != TR_INIT_INIT_PARK)
			return true;
		setParkingSPenStyle(true);
		setParkingSPenStyle(false);

		if(!m_parking)
			return false;

		int32_t w = getWith(m_parking & 0x00000000000000ffU);
		if(w)
		{
			link->initDoubleLine(zoom_ref, m_par_line, w, false);
			if(!(TrGeoObject::getGlobelFlags() & TR_MASK_PARKING_MODE))
				checkNodes(zoom_ref, w, false);
		}
		if(m_pen_park_left != nullptr)
		{
			w = -setParkingWidth((m_parking >> 32) & 0x00000000000000ffU);
			link->initDoubleLine(zoom_ref, m_par_left_line, w, false);
			if(!(TrGeoObject::getGlobelFlags() & TR_MASK_PARKING_MODE))
				checkNodes(zoom_ref, w, true);
		}
	}
	return true;
}

void TrMapParkLane::drawLine(const TrZoomMap & zoom_ref, QPainter * p, QVector<TrPoint> & par_line)
{
	QVector<QPointF> pointPairs;

	for(int i = 0; i < par_line.size(); i++)
	{
		TrPoint screenx = par_line[i];
		zoom_ref.setMovePoint(&screenx.x, &screenx.y);
		QPointF ptf;
		ptf.setX(screenx.x);
		ptf.setY(screenx.y);
		pointPairs.append(ptf);
	}
	if(pointPairs.size())
	{
		p->drawPolyline(pointPairs);
	}
}

void TrMapParkLane::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	if(m_ref == nullptr)
		return;

	if(!(m_inst_mask & TR_MASK_DRAW))
		return;

	if(m_par_line.size() == 0)
		return;

	TrMapLinkRoad * link = dynamic_cast<TrMapLinkRoad *>(m_ref);
	if(link == nullptr)
		return;

	if(/*(m_parking & 0xff00) &&*/ (s_mask & TR_MASK_SHOW_PARKING))
	{
		if((m_pen_park != nullptr))
		{
			p->setPen(*m_pen_park);
			drawLine(zoom_ref, p, m_par_line);
		}

		if(link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			if(m_showOther && (m_pen_park_left != nullptr))
			{
				p->setPen(*m_pen_park_left);
				drawLine(zoom_ref, p, m_par_left_line);
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
