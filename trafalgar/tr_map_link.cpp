/******************************************************************
 *
 * @short	link class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapLink
 * superclass:	TrGeoObject
 * modul:	tr_map_link.cc
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert(C) 2012-2025
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

#include "tr_map_link.h"

// TODO: only for the name list?
#include "tr_map_net_road.h"
#include "tr_name_element.h"


#include <math.h>

#define SEGMENT_TO true
#define SEGMENT_FROM false

TrMapLink::TrMapLink()
	: TrGeoObject()
	//, m_poly_flags(0)
	, m_mm_load_width(0)
	, m_name_id(0)
	, m_node_from(nullptr)
	, m_node_to(nullptr)
	, m_pline(nullptr)
	, m_geo_id(0)
	, m_seg_from{{DIR_INIT, 1.0, 2.0}, 3.0}
	, m_seg_to{{DIR_INIT, 1.0, 2.0}, 3.0}
	, m_one_way(0)
{
	m_inst_mask = TR_MASK_DRAW;
}

TrMapLink::~TrMapLink()
{
}

QDebug operator<<(QDebug dbg, const TrMapLink& link)
{
	if((link.m_node_from == nullptr) || (link.m_node_to == nullptr))
		return dbg << link.getXmlName() << "----" << "----" <<
		"class " << HEX << link.getType() << link.m_one_way;
	return dbg << link.getXmlName() << " " << DEC <<
		link.m_node_from->getGeoId() << " - " <<
		link.m_node_to->getGeoId() << " class " << HEX <<
		link.getType() << " " <<
		link.m_one_way << " ";
}

QString TrMapLink::getXmlName() const
{
	return "map_link";
}

uint16_t TrMapLink::getRdClass()
{
	return getType();
}

void TrMapLink::setRdClass(uint16_t road_class)
{
	setType(road_class);
}

int32_t TrMapLink::getWidth()
{
	return m_mm_load_width;
}

void TrMapLink::setWidth(int32_t width)
{
	// if not oneway -> half width
	if(m_one_way & TR_LINK_DIR_ONEWAY)
		m_mm_load_width = width;
	else
		m_mm_load_width = width >> 1;
}

void TrMapLink::setOneWay(uint8_t dir)
{
	m_one_way = dir;
}

uint8_t TrMapLink::getOneWay()
{
	return m_one_way;
}

/* for osm filter and edit */
// TODO: net claas?
void TrMapLink::setNodeFrom(TrMapList * node_list, int64_t node)
{
	m_node_from = dynamic_cast<TrMapNode *> (node_list->getMapObject(node));
	if(m_node_from == nullptr)
	{
		TR_WRN << "node" << node;
		return;
	}
	TrPoint pt = m_node_from->getPoint();
	setCrossingPoint(pt, true);
}

void TrMapLink::setNodeTo(TrMapList * node_list, int64_t node)
{
	m_node_to = dynamic_cast<TrMapNode *> (node_list->getMapObject(node));
	if(m_node_to == nullptr)
	{
		TR_WRN << "node" << node;
		return;
	}
	TrPoint pt = m_node_to->getPoint();
	setCrossingPoint(pt, false);
}
/* only osm filter */

int64_t TrMapLink::getNodeFrom()
{
	if(m_node_from != nullptr)
		return m_node_from->getGeoId();
	TR_WRN << "m_node_from == nullptr";
	return 0;
}

int64_t TrMapLink::getNodeTo()
{
	if(m_node_to != nullptr)
		return m_node_to->getGeoId();
	TR_WRN << "m_node_from == nullptr";
	return 0;
}

// TODO: to road link
bool TrMapLink::switchShadowNode(bool dir)
{
	if((m_node_from == nullptr) || (m_node_to == nullptr))
		return false;
	if(dir)
	{
		TrMapNode * node = m_node_to->getShadow(false);
		if(node != nullptr)
			m_node_to = node;
	}
	else
	{
		TrMapNode * node = m_node_from->getShadow(false);
		if(node != nullptr)
			m_node_from = node;
	}
	return true;
}

bool TrMapLink::setNodeRef(TrMapNode * nd, bool dir)
{
	if(nd == nullptr)
	{
		TR_WRN << "node == nullptr";
		return false;
	}
	if(dir == TR_IS_TO_DIR)
		m_node_from = nd;
	else
		m_node_to = nd;
	return true;
}

TrMapNode * TrMapLink::getNodeFromRef()
{
	return m_node_from;
}

TrMapNode * TrMapLink::getNodeToRef()
{
	return m_node_to;
}

bool TrMapLink::getNodePoint(TrPoint & point, bool dir)
{
	if((m_node_from == nullptr) || (m_node_to == nullptr))
		return false;
	if(dir)
		point = m_node_from->getPoint();
	else
		point = m_node_to->getPoint();
	return true;
}

uint32_t TrMapLink::getNameId()
{
	return m_name_id;
}

void TrMapLink::setNameId(uint32_t id)
{
	m_name_id = id;
}

TrGeoObject * TrMapLink::getNameElement()
{
    if(TrMapNetRoad::ms_name_list == nullptr)
        return nullptr;
    return TrMapNetRoad::ms_name_list->getMapObject(m_name_id);
}


QString TrMapLink::getElementName()
{
	if(TrMapNetRoad::ms_name_list == nullptr)
		return QString();
	TrNameElement * elm = dynamic_cast<TrNameElement*>
		(TrMapNetRoad::ms_name_list->getMapObject(m_name_id));
	if(elm == nullptr)
		return QString();
	return elm->getName();
}

void TrMapLink::setGeoId(int64_t id)
{
	m_geo_id = id;
}

int64_t TrMapLink::getGeoId()
{
	return m_geo_id;
}

TrGeoPolygon * TrMapLink::getPolygon()
{
	return dynamic_cast<TrGeoPolygon *>(m_pline);
	//return m_pline;
}

void TrMapLink::setPolygon(TrGeoPolygon * poly)
{
	m_pline = poly;
	if(poly == nullptr)
	{
		m_geo_id = 0;
	}
}

// TODO: unused?
poly_add & TrMapLink::getGeoSegmentData(const TrZoomMap & zoom_ref, bool dir)
{
	if(dir)
		return m_seg_from;
	return m_seg_to;
}

bool TrMapLink::getSegment(TrGeoSegment & seg, bool dir, bool par)
{
	TR_INF << "unused...";
	return false;
}

bool TrMapLink::getParSegment(const TrZoomMap & zoom_ref, poly_add & add, bool dir)
{
	TR_INF << "unused";
	return false;
}


// TODO: rework! -> used at osm filter?
bool TrMapLink::setPrimiveById(TrMapList * primive)
{
	if(m_pline != nullptr)
		delete m_pline;

	if(m_geo_id != 0)
	{
		m_pline = dynamic_cast<TrGeoPolygon *>(primive->getMapObject(m_geo_id));
		if(m_pline == nullptr)
		{
			TR_WRN << "m_geo_id" << m_geo_id << "is not 0 but m_pline is nullptr";
		}
	}
	return true;
}

bool TrMapLink::isAsDoubleLine()
{
	return ((getType() & 0x00ff) < 9);
}

// TODO: function: check if ramp is needed...

// TODO: check! -> called from readXmlDescription
// TODO: not called - to remove???
// TODO: is called! -> node is only a place holder -> get real node from list
bool TrMapLink::reSetNodes(TrMapList * node_map)
{
	int64_t nd_from = getNodeFrom();
	int64_t nd_to = getNodeTo();

	if((nd_from == 0) || (nd_to == 0) || (m_node_from == nullptr) ||
			(m_node_to == nullptr) || (node_map == nullptr))
	{
		TR_WRN << "node nullptr: " << nd_from << nd_to;
		return false;
	}
	if(m_node_from->getViewOpt() != 0xff)
	{
		TR_WRN << "!= 0xff";
	}
	delete m_node_from;
	m_node_from = dynamic_cast<TrMapNode *> (node_map->getMapObject(nd_from));

	if(m_node_to->getViewOpt() != 0xff)
	{
		TR_WRN << "!= 0xff";
	}
	delete m_node_to;
	m_node_to = dynamic_cast<TrMapNode *> (node_map->getMapObject(nd_to));

	return true;
}

// TODO: setLayerShowMask obsolete???
void TrMapLink::setLayerShowMask(uint64_t mask)
{
	m_inst_mask &= ~(TR_MASK_DRAW);

	uint64_t class_mask = 1;
	if((getType() & 0x000f) > 1)
		class_mask = static_cast <uint64_t>((1 << ((getType() & 0x000f) -1)));
	//TR_INF << HEX << (class_mask & mask) << class_mask << mask;
	if((class_mask & mask) == class_mask)
	{
		// don't show the link if it is a backward link and single line
		if(m_one_way == TR_LINK_DIR_BWD)
		{
			if((!(s_mask & TR_MASK_MORE_LINES)) && (!isAsDoubleLine()))
			{
				return;
			}
		}
		m_inst_mask |= TR_MASK_DRAW;
	}
}

void TrMapLink::setLinkPen(TrGeoObject * base)
{
	TrMapList * list = dynamic_cast<TrMapList *>(base);
	if(list != nullptr)
	{
		uint8_t rd_class = (this->getType() & 0x1f);
		if((rd_class == 0) || (rd_class > 16))
		{
			setActivePen(list->getObjectPen(1));
		}
		else
		{
			setActivePen(list->getObjectPen(rd_class));
		}
	}
}

bool TrMapLink::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	if((m_node_from == nullptr) || (m_node_to == nullptr))
	{
		TR_WRN << "node == nullptr";
		return false;
	}

	// TODO: first test, use in other class?
	/*if((m_geo_active_pen != nullptr) && (ctrl & TR_INIT_COLORS))
	{
		m_pen_para.setColor(m_geo_active_pen->color());
		// TODO: '2' -> from static value?
		m_pen_para.setWidth(2);
	}*/
	if(ctrl & TR_INIT_COLORS)
	{
		setLinkPen(base);
		//setParkingPen(m_parking >> 8, base);
	}

	if(ctrl & TR_INIT_GEOMETRY)
	{
		ctrl &= 0xff;
		// TODO: code = 20 -> 'setRampMode' only edges?
		if(ctrl == 20)
			return true;

		// code for shadow nodes
		if(ctrl == 10)
		{
			// TODO: no shadow nodes now... and move to road link
			/*if(!(m_one_way & TR_LINK_DIR_DIV))
				return false;
			TrMapList * node_list = dynamic_cast<TrMapList*>(base);
			if(node_list == nullptr)
				return false;
			return m_node_to->markDivider(*node_list, this);*/
			return true;
		}

		// code for the angles
		if(ctrl == 11)
		{
			return true;
		}

		// code for double line
		// TODO: remove!
		if(ctrl == 14)
		{
			//if(s_mask & TR_MASK_MORE_LINES)
		//	initDoubleLine(zoom_ref, 100);
			//return true;
		}

		// code for checking the ramps and set cross points in now road link class
		// TODO: ->to road link
	}
	return true;
}

bool TrMapLink::initSecs(const TrZoomMap & zoom_ref, TrMapList * pr_list, TrMapList & nd_list)
{
	return false;
}

uint8_t TrMapLink::handleCrossing(const TrZoomMap & zoom_ref, TrGeoObject * other, TrGeoObject * node)
{
	return 0xff;
}

// TODO: to check the length
// virtual -> road link
double TrMapLink::getLength(const TrZoomMap & zoom_ref)
{
	TrPoint pt1 = m_node_from->getPoint();
	TrPoint pt2 = m_node_to->getPoint();
	// TODO getNodePoints(pt1, pt2);
	if(m_one_way & TR_LINK_DIR_BWD)
	{
		pt1 = pt2;
		pt2 = m_node_from->getPoint();
	}
	if(m_pline)
	{
		return m_pline->getLength(zoom_ref, pt1, pt2);
	}
	return zoom_ref.getLength(pt1.x, pt1.y, pt2.x, pt2.y);
}

// the 'initDoubleLine->getLength' function needes the length
// of the segment for all links, not usable for routing
void TrMapLink::initDoubleLine(const TrZoomMap & zoom_ref, QVector<TrPoint> &m_par_line, int32_t width)
{
	TR_INF << "unused";
}

TrGeoObject * TrMapLink::manageGap(const TrZoomMap & zoom_ref, uint8_t mode, const TrPoint & pt, TrGeoObject * obj)
{
	TrGeoPolygon * poly = nullptr;
	TrMapLink * parallel_link = nullptr;

	switch(mode)
	{
	case TR_NET_GAP_ADD:            //0x01
		poly =  dynamic_cast<TrGeoPolygon *>(obj);
		if(poly != nullptr)
		{
			QVector<TrPoint> to_add;
			to_add.append(pt);
			poly->appendPoints(to_add);
			poly->setSurroundingRect();
			// TODO check!
			//this->setPolygon(poly);
			this->setSurroundingRect();
			parallel_link = this->getParallelLink();
			if(parallel_link != nullptr)
			{
				// TODO check!
				//parallel_link->setPolygon(poly);
				parallel_link->setSurroundingRect();
			}
		}
		break;

	case TR_NET_GAP_REMOVE:         //0x02
		//TR_INF << "del" << m_pline;
		if(m_pline != nullptr)
		{
			parallel_link = this->getParallelLink();
			if(parallel_link != nullptr)
			{
				if(m_pline == parallel_link->getPolygon())
				{
					// TODO check!
					parallel_link->setPolygon(nullptr);
                    //parallel_link->initDoubleLine(zoom_ref, 100);
				}
			}
			// TODO: crash on m_pline, check if only one point is left
			// creates errors/warnigs
			// Warning: setSurroundingRect tr_map_link.cc 1230 ,  no point in pline (tr_map_link.cc:1230,
			//     virtual bool TrMapLink::setSurroundingRect())
			// Error: : setDirectionAngles tr_map_node.cc 521 ,  (tr_map_node.cc:521,
			//     bool TrMapNode::setDirectionAngles(const TrZoomMap&, QVector<TrConnectionMember>&, bool))
			//delete m_pline;
			// missing init?
			// thread problem? -> parallel link!?
			setPolygon(nullptr);
            //initDoubleLine(zoom_ref, 100);
			//TR_INF << "del 2" << m_pline << *this << m_node_from << m_node_to;
		}
		break;

	case TR_NET_GAP_REPLACE:        //0x04
		// placeholder
		TR_WRN << "TODO, do not use!";
		break;

	case TR_NET_GAP_UPDATE:
		parallel_link = this->getParallelLink();
        //if(parallel_link != nullptr)
        //	parallel_link->initDoubleLine(zoom_ref, 100);
        //initDoubleLine(zoom_ref, 100);
		break;

	default:
		TR_WRN << "unused code!" << mode;
		break;
	}
	return nullptr;
}

// get the angle from the node to point inside fos the link or the next node
double TrMapLink::getAngle(const TrZoomMap & zoom_ref, bool dir)
{
	double ang = 10.0;
	TrGeoSegment seg;
	TrPoint next_point = {0.0,0.0};

	if((m_node_to == nullptr) || (m_node_from == nullptr))
		return ang;
	if(getInsidePoint(next_point, dir) == false)
		return ang;
	if(!dir)
	{
		seg.setPoints(m_node_from->getPoint(), next_point);
	}
	else
	{
		seg.setPoints(m_node_to->getPoint(), next_point);
	}
	//TR_INF << seg << TR_COOR(next_point) << TR_COOR(m_node_from->getPoint()) << TR_COOR(m_node_to->getPoint());
	ang = seg.getAngle(zoom_ref);
    int err_code = zoom_ref.getErrorCode();
	if(err_code)
		TR_ERR << err_code;

	return ang;
}

bool TrMapLink::removeSmallSeg(const TrZoomMap & zoom_ref, double l_limit, bool dir)
{
        if(m_pline == nullptr)
                return false;

        TrGeoSegment seg;
        seg.setPoints(m_node_from->getPoint(), m_node_to->getPoint());
        QList<TrGeoSegment> seg_list;

        seg.getSegList(seg_list, *m_pline);

        //double len = seg_list[seg_list.size()-1].getLength(zoom_ref);
        if(seg_list.size() < 1)
                return false;
        double len;
        if(dir)
        {
                len = seg_list[seg_list.size()-1].getLength(zoom_ref);
        }
        else
        {
                len = seg_list[0].getLength(zoom_ref);
        }
        if(len > l_limit)
                return false;

        //TR_INF << len << m_pline->getSize();
        if(dir)
                m_pline->removePoint(m_pline->getSize());
        else
                m_pline->removePoint(1);

        if(m_pline->getSize())
        {
                m_pline->init(zoom_ref, TR_INIT_GEOMETRY, nullptr);
        }
        else
        {
            this->setPolygon(nullptr);
            TrMapLink * par = this->getParallelLink();
            if(par != nullptr)
                par->setPolygon(nullptr);
        }
        return true;
}



bool TrMapLink::handleSmallElement(const TrZoomMap & zoom_ref, double a_limit, double l_limit)
{
	// node to node with no polygon -> exit
	if(m_pline == nullptr)
		return false;
	// backward link points to the same polygon
	if(m_one_way & TR_LINK_DIR_BWD)
		return false;
	if(removeSmallSeg(zoom_ref, l_limit, true))
	{
		if(removeSmallSeg(zoom_ref, l_limit, true))
		{
		}
	}
	if(removeSmallSeg(zoom_ref, l_limit, false))
	{
		if(removeSmallSeg(zoom_ref, l_limit, false))
		{
		}
	}
	return true;
}

// get the next point near the node
bool TrMapLink::getInsidePoint(TrPoint & point, bool dir)
{
	if(m_pline == nullptr)
	{
		return getNodePoint(point, dir);
	}
	if(m_pline->getSize() < 1)
	{
		return false;
	}
	bool fwd = (getOneWay() & TR_LINK_DIR_BWD) != TR_LINK_DIR_BWD;
	if(dir)
		 fwd =! fwd;
	if(fwd)
	{
		point = m_pline->getFirstPoint();
	}
	else
	{
		point = m_pline->getLastPoint();
	}
	return true;
}

bool TrMapLink::getParPoint(bool first, TrPoint & pt)
{
	TR_WRN << "do not use";
	return false;
}

bool TrMapLink::setParPoint(bool first, TrPoint & pt)
{
	TR_WRN << "do not use";
	return false;
}

bool TrMapLink::setRamp(const TrZoomMap & zoom_ref, bool dir)
{
	TR_WRN << "do not use";
	return false;
}

// used for ramp and normal crossing on base line (oneway)
bool TrMapLink::setCrossingPoint(TrPoint & pt, bool dir)
{
	// for rail etc.
	return false;
}

void TrMapLink::getTwoLine(const TrZoomMap & zoom_ref, QPolygon & poly, TrPoint pta, TrPoint ptb)
{
	if(m_pline != nullptr)
		return;
	// TODO: to road link
	//QPolygon poly(2);
	TrPoint pt1 = pta;
	TrPoint pt2 = ptb;

	if(m_one_way & TR_LINK_DIR_BWD)
	{
		pt1 = ptb;
		pt2 = pta;
	}
	zoom_ref.setMovePoint(&pt1.x,&pt1.y);
	zoom_ref.setMovePoint(&pt2.x,&pt2.y);
	poly.setPoint(0, static_cast <int>(pt1.x),
		static_cast <int>(pt1.y));
	poly.setPoint(1, static_cast <int>(pt2.x),
		static_cast <int>(pt2.y));
}

void TrMapLink::drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrGeoObject::drawSurroundingRect(zoom_ref, p, mode);
	if((m_node_from == nullptr) || (m_node_to == nullptr))
		return;
	m_node_from->drawSelect(zoom_ref, p, 0);
	m_node_to->drawSelect(zoom_ref, p, 1);
	if(m_pline != nullptr)
	{
		// TODO: handling of 'TR_MASK_DRAW' flag is OK?
		m_pline->setMask(TR_MASK_SHOW_POINTS | TR_MASK_DRAW);
		m_pline->draw(zoom_ref, p, 0);
		m_pline->removeMask(TR_MASK_SHOW_POINTS | TR_MASK_DRAW);
	}
}

void TrMapLink::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	if(!(m_inst_mask & TR_MASK_DRAW))
		return;

	// TODO: draw without double line -> "road link"
	//TR_INF << *this;
	if(this->clip(zoom_ref))
		return;

	if(m_geo_active_pen == nullptr)
	{
		TR_WRN << "no active_pen -> exiting!" << HEX << getType();
		return;
	}
	if(m_inst_mask & TR_MASK_SELECTED)
		drawSelect(zoom_ref, p, mode);

	if(m_pline == nullptr)
	{
		QPolygon poly(2);
		getTwoLine(zoom_ref, poly, m_node_from->getPoint(), m_node_to->getPoint());
		p->drawPolyline(poly);
	}
	else
	{
		// TODO: check -> to road link?
		m_pline->setActivePen(getActivePen());
		if(m_one_way & TR_LINK_DIR_BWD)
			m_pline->draw(zoom_ref, p, m_node_to->getPoint(), m_node_from->getPoint(), 2);
		else
			m_pline->draw(zoom_ref, p, m_node_from->getPoint(), m_node_to->getPoint(), 2);
	}
}

double TrMapLink::getLength()
{
	double length = m_seg_from.len_part;
	//TR_MSG << "to" << length;

	if(m_pline)
	{
		length += m_pline->getSegmentsLength();
		length += m_seg_to.len_part;
	}
	//TR_MSG << "from" << length;
	return length;
}

// TODO: remove?, check dir option...
TrMapLink * TrMapLink::getParallelLink()
{
	// TODO: m_one_way, flag for parallel link?
	if(m_node_from != nullptr)
		return dynamic_cast<TrMapLink *>(m_node_from->getParallelElement(this, false));
	return nullptr;
}

// for OSM filter or edit!
// TODO: face <> link -> object?
bool TrMapLink::appendPolyPoint(TrPoint & pt)
{
	//TR_MSG << pt.x;
	bool is_new = false;

	if(m_pline == nullptr)
	{
		m_pline = new TrGeoPolygon();
		is_new = true;
	}
	QVector<TrPoint> next_point;
	next_point.append(pt);
	m_pline->appendPoints(next_point);
	return is_new;
}

bool TrMapLink::setSurroundingRect()
{
	if((m_node_to == nullptr) || (m_node_from == nullptr))
		return false;

	// TR_MSG;
	bool first = true;
	if(m_node_from->setSurroundingRect() == true)
	{
		updateSurroundRect(*m_node_from, first);
		first = false;
	}
	if(m_node_to->setSurroundingRect() == true)
	{
		updateSurroundRect(*m_node_to, first);
		first = false;
	}

	if(m_pline != nullptr)
	{
		if(m_pline->setSurroundingRect())
		{
			updateSurroundRect(*m_pline, first);
		}
		else
		{
			//TR_WRN << "no point in pline";
			return false;
		}
	}
	if(first)
		return false;
	return true;
}

void TrMapLink::setPolyPoints(TrGeoPolygon & line)
{
	//TR_INF << "do dont use";
	QVector<TrPoint> next_points;

	if(m_pline)
	{
		//TR_MSG << m_pline->getSize();

		for(size_t i=0; i < m_pline->getSize(); i++)
		{
			TrPoint pt;
			if(m_one_way & TR_LINK_DIR_BWD)
			{
				pt = m_pline->getPoint(i);
				//TR_MSG << "-" << pt.x << pt.y;
			}
			else
			{
				pt = m_pline->getPoint(m_pline->getSize()-(i+1));
				//TR_MSG << "+" << pt.x << pt.y;
			}
			next_points.append(pt);
		}
	}
	// TODO: check
	next_points.append(m_node_from->getPoint());
	line.appendPoints(next_points);
}

double TrMapLink::getClosest(const TrZoomMap & zoom_ref, TrPoint & pt, double & rel_dist)
{
	TR_MSG << "not tested!";

	// TODO: not used -> not tested!
	double dist = -1.0;

	TrMapNode * nd_f = TrMapLink::getNodeFromRef();
	TrMapNode * nd_t = TrMapLink::getNodeToRef();

	if((nd_f == nullptr) || (nd_t == nullptr))
		return dist;

	double len = zoom_ref.getLength(nd_t->getPoint(), nd_f->getPoint());

	if(m_pline != nullptr)
	{
		int seg = -1;

		if(m_pline->getSize() > 1)
		{
			dist = m_pline->getClosest(zoom_ref, pt, seg, rel_dist);
		}
		else
		{
			TrPoint test = pt;
			double xdist1 = zoom_ref.getSegmentClosest(&m_seg_from, nd_f->getPoint(), test);
			test = pt;
			double xdist2 = zoom_ref.getSegmentClosest(&m_seg_to, nd_f->getPoint(), test);

			if(xdist2 < xdist1)
				dist = zoom_ref.getSegmentClosest(&m_seg_to, nd_f->getPoint(), pt);
			else
				dist = zoom_ref.getSegmentClosest(&m_seg_from, nd_f->getPoint(), pt);
			return dist;
		}
	}
	else
	{
		dist = zoom_ref.getSegmentClosest(&m_seg_from, nd_f->getPoint(), pt);
	}

	double dist1 = zoom_ref.getLength(nd_f->getPoint(), pt);
	double dist2 = zoom_ref.getLength(nd_t->getPoint(), pt);

	//TR_MSG << len << dist1 << dist2;

	if((dist1 > len) || (dist2 > len))
	{
		return -1.0;
	}
	return dist;
}

#ifdef TR_SERIALIZATION
uint64_t TrMapLink::readXmlDescription(QXmlStreamReader & xml_in)
{
	QStringRef name = readXmlHeader(xml_in);

	//TR_MSG << name;

	if(name != getXmlName())
	{
		TR_ERR;
		return TR_NO_VALUE;
	}
	return readXmlData(xml_in, getXmlName());
}

uint64_t TrMapLink::readXmlData(QXmlStreamReader & xml_in, const QString & block)
{
	int ramp = 0;
	//bool ok;
	uint64_t ret = 0;

	QXmlStreamAttributes attr = xml_in.attributes();
	m_one_way  = attr.value("", "one_way").toInt();
	m_type = attr.value("", "class").toInt();
	// TODO: 'ramp' to the road link class
	ramp   = attr.value("", "ramp").toInt();

	m_name_id = attr.value("", "name_id").toInt();

	if(ramp)
		m_type |= TR_LINK_RAMP_FLAG;

	// buffer, replaced by node list
	m_node_from = new TrMapNode();
	m_node_from->setViewOpt(0xff);
	m_node_to = new TrMapNode();
	m_node_to->setViewOpt(0xff);

	while(!xml_in.atEnd())
	{
		if(xml_in.isComment())
		{
			TR_WRN << xml_in.tokenString() << ": " << xml_in.text();
		}
		else if(xml_in.isCharacters())
		{
		}
		else if(xml_in.isStartElement())
		{
			if(xml_in.name() == "node")
			{
				// <node to="30976830" from="1611237964" id="1"
				QXmlStreamAttributes attr = xml_in.attributes();

				if((m_node_from != nullptr) && (m_node_to != nullptr))
				{
					uint64_t n_to = attr.value("", "to").toLong();
					uint64_t n_from = attr.value("", "from").toLong();
					if((n_to == 0) || (n_from == 0))
					{
						abortOnLine(xml_in, "illegal node value(s): " +
							attr.value("", "to") + ", " + attr.value("", "from") +
							" at file " + __FILE__ +  " (" + __func__ + ")");
						ret = TR_NO_VALUE;
					}
					m_node_from->setGeoId(n_to);
					m_node_to->setGeoId(n_from);
				}
				m_geo_id = attr.value("", "id").toLong();
			}
		}
		else if(xml_in.isEndElement())
		{
			if(xml_in.name() == block)
			{
				xml_in.readNext();
				return ret;
			}
			if(xml_in.name() == "node")
			{
			}
		}
		xml_in.readNext();
	}

	return TR_NO_VALUE;
}

void TrMapLink::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());
	writeXmlLinkPart(xml_out, id);
	xml_out.writeEndElement();
}

void TrMapLink::writeXmlLinkPart(QXmlStreamWriter & xml_out, uint64_t id)
{
	// TODO: 'ramp' to the road link class
	uint16_t rd_ramp = (m_type & TR_LINK_RAMP_FLAG);
	uint16_t rd_class_n = (m_type & ~TR_LINK_RAMP_FLAG);

	// TODO: ramp flag at road link...
	xml_out.writeAttribute("class", QVariant(rd_class_n).toString());
	if(rd_ramp)
		xml_out.writeAttribute("ramp", "1");

	xml_out.writeAttribute("one_way", QVariant(m_one_way).toString());

	if(m_name_id)
	{
		xml_out.writeAttribute("name_id", QVariant(m_name_id).toString());
	}

	xml_out.writeStartElement("node");
	// TODO: change the node names for all net files!
	TrMapNode * node = m_node_to->getShadow(true);
	if(node)
	{
		m_node_to = node;
		TR_INF << *this;
	}
	node = m_node_from->getShadow(true);
	if(node)
	{
		m_node_from = node;
		TR_INF << *this;
	}
	xml_out.writeAttribute("to", QVariant((qulonglong)getNodeFrom()).toString());
	xml_out.writeAttribute("from", QVariant((qulonglong)getNodeTo()).toString());

	xml_out.writeAttribute("id", QVariant((qulonglong)m_geo_id).toString());

	xml_out.writeEndElement();
}
#endif
