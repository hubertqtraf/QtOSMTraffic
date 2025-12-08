/******************************************************************
 *
 * @short	segment class, defined by two points
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoSegment
 * superclass:	TrGeoObject
 * modul:	tr_geo_segment.cc	geometric selectable object
 * @version	0.1
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2023-2025
 *
 * beginning:	10.2023
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

#include "tr_geo_segment.h"

#include <math.h>

#define TR_DIST 6.0

TrGeoSegment::TrGeoSegment()
	: TrGeoObject()
	, m_first{0.0, 0.0}
	, m_second{0.0, 0.0}
{
        m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
        //m_geo_id = -1;
}

TrGeoSegment::TrGeoSegment(TrPoint first, TrPoint second)
	: TrGeoObject()
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
	setPoints(first, second);
}

TrGeoSegment::~TrGeoSegment()
{
}

/*TrGeoSegment::TrGeoSegment(const TrGeoSegment& other)
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
	setPoints(other.m_first, other.m_second);
}*/

QDebug operator<<(QDebug dbg, const TrGeoSegment& seg)
{
	return dbg << seg.getXmlName() << " (" << TR_COOR(seg.m_first) << ")("
		<< TR_COOR(seg.m_second) <<  ")";
}

QString TrGeoSegment::getXmlName() const
{
	return "segment";
}

void TrGeoSegment::setPoints(TrPoint first, TrPoint second)
{
	m_first = first;
	m_second = second;
	m_inst_mask |= TR_MASK_DATA;
}

void TrGeoSegment::setPoints(TrGeoPolygon & poly, size_t pos)
{
	// TODO: check size
	m_first = poly.getPoint(pos);
	m_second = poly.getPoint(pos + 1);
	m_inst_mask |= TR_MASK_DATA;
	//TR_INF << "X this:  0 " << m_first.x << m_first.y << " 1 " << m_second.x << m_second.y;
}

void TrGeoSegment::setPoints(const TrGeoSegment& other)
{
	setPoints(other.m_first, other.m_second);
}

bool TrGeoSegment::setPointsFromPoly(TrGeoPolygon & poly, uint64_t idx, bool dir)
{
	if(idx > (poly.getSize()-2))
	{
		return false;
	}
	if(dir)
	{
		m_second = poly.getPoint(idx);
		m_first = poly.getPoint(idx + 1);
	}
	else
	{
		m_first = poly.getPoint(idx);
		m_second = poly.getPoint(idx + 1);
	}
	m_inst_mask |= TR_MASK_DATA;
	return true;
}

TrPoint TrGeoSegment::getFirstPoint()
{
	return m_first;
}

TrPoint TrGeoSegment::getSecondPoint()
{
	return m_second;
}

bool TrGeoSegment::hasData()
{
	return ((m_inst_mask & TR_MASK_DATA) == TR_MASK_DATA);
}

void TrGeoSegment::setFirstPoint(TrPoint & pt)
{
	m_inst_mask |= TR_MASK_DATA;
	m_first = pt;
}

void TrGeoSegment::setSecondPoint(TrPoint & pt)
{
	m_inst_mask |= TR_MASK_DATA;
	m_second = pt;
}

TrPoint TrGeoSegment::getScreenPoint(const TrZoomMap & zoom_ref, bool select)
{
	TrPoint pt;
	if(select)
	{
		pt = m_first;
	}
	else
	{
		pt = m_second;
	}
	zoom_ref.setMovePoint(&pt.x,&pt.y);
	return pt;
}

bool TrGeoSegment::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	// not used
	return false;
}

bool TrGeoSegment::setSurroundingRect()
{
	double rect[4];
	rect[0] = rect[2] = m_first.x;
	rect[1] = rect[3] = m_first.y;
	updateSurroundRect(rect, true);
	rect[0] = rect[2] = m_second.x;
	rect[1] = rect[3] = m_second.y;
	updateSurroundRect(rect, false);
	return true;
}

bool TrGeoSegment::getCrossPoint(const TrZoomMap & zoom_ref, TrPoint & pt, TrGeoSegment & other)
{
	poly_add add1;
	poly_add add2;

	this->getSegmentData(zoom_ref, add1, m_first.x);
	other.getSegmentData(zoom_ref, add2, m_first.x);
	zoom_ref.getCrossPoint(add1, add2, pt);
	//TR_INF << pt.x << pt.y;
	pt.x += m_first.x;
	return true;
}

double TrGeoSegment::getAngle(const TrZoomMap & zoom_ref) const
{
	if(m_inst_mask & TR_MASK_DATA)
		return zoom_ref.getAngle(m_first, m_second);
	return 100.0;
}

void TrGeoSegment::doReverse()
{
	TrPoint cp = m_first;
	m_first = m_second;
	m_second = cp;
}

int TrGeoSegment::getAngleCode(const TrZoomMap & zoom_ref, const TrGeoSegment &other)
{
	double ang1 = getAngle(zoom_ref);
	double ang2 = other.getAngle(zoom_ref);
	double level = 0.2;
	int err_code = zoom_ref.getErrorCode();

	/*if(err_code)
	{
		TR_ERR << err_code << *this;
		return -1;
	}*/
	if((ang1 > 99.0) || (ang2 > 99.0))
		return -2;

	double diff = abs(ang1 - ang2);
	if(diff > M_PI)
	{
		diff = abs(diff - (M_PI * 2.0));
	}
	if(diff < level)
	{
		return 1;
	}
	diff = abs(diff - M_PI);
    TR_INF << ang1 << ang2 << diff;
    if(diff < level)
	{
        return 2;
	}
	return 0;
}

double TrGeoSegment::getLength(const TrZoomMap & zoom_ref)
{
	return zoom_ref.getLength(m_first, m_second);
}

void TrGeoSegment::clearData()
{
	m_first = {0.0, 0.0};
	m_second = {0.0, 0.0};
	m_inst_mask &= ~(TR_MASK_DATA);
}


bool TrGeoSegment::getSegmentData(const TrZoomMap & zoom_ref, poly_add & add, double shift)
{
	TrPoint f_move = m_first;
	TrPoint s_move = m_second;
	f_move.x -= shift;
	s_move.x -= shift;
	zoom_ref.getLineByPoints(add, f_move, s_move);
	return false;
}

void TrGeoSegment::getSegList(QList<TrGeoSegment> & seg_list, TrGeoPolygon & poly)
{
	seg_list.clear();
	TrPoint first = m_first;
	for(size_t i = 0; i < poly.getSize(); i++)
	{
		TrGeoSegment seg(first, poly.getPoint(i));
		seg_list.append(seg);
		first = poly.getPoint(i);
	}
	TrGeoSegment seg(first, m_second);
	seg_list.append(seg);
}

bool TrGeoSegment::isEvenPolygon(const TrZoomMap & zoom_ref, QList<TrGeoSegment> & seg_list, double ctrl)
{
	bool ret = true;
	double ang = getAngle(zoom_ref);
	//TR_INF << "====" << ang;
	for (int i = 0; i < seg_list.size(); ++i)
	{
		double diff = fabs(ang - seg_list[i].getAngle(zoom_ref));
		if(diff > ctrl)
			ret = false;
		//TR_INF << " - " << seg_list[i].getAngle(zoom_ref) << diff << ret;
	}
	return ret;
}

bool TrGeoSegment::managePolygon(const TrZoomMap & zoom_ref, TrGeoPolygon & poly,
	QList<TrGeoSegment> & seg_list, int width)
{
	// TODO: segment from nodes
	// calc the distance beween the points
	// remove the points near the nodes (partly done...)
	// .. OR ..
	// use angle between nodes; prove the angle to next polygon point
	// start from nodes to center
	// remove points...

	//double ang = getAngle(zoom_ref);
	//TR_INF << ang << getLength(zoom_ref) << poly.getSize();

	double length_fwd = seg_list[0].getLength(zoom_ref);
	size_t idx_fwd = 1;
	bool do_fwd = true;
	double length_bwd = seg_list[seg_list.size()-1].getLength(zoom_ref);
	size_t idx_bwd = poly.getSize();
	bool do_bwd = true;
	//TR_INF << "*** start: " << poly.getSize() << "F: " << length_fwd << "B: " << length_bwd;
	bool ret = false;

	double d_width = fabs(width/1000.0);
	//TR_INF << d_width << width;

	while(do_fwd || do_bwd)
	{
		//TR_INF << "F:" << length_fwd << "B:" << length_bwd << d_width << " | " << idx_bwd << idx_fwd << do_fwd << do_bwd;
		if((length_fwd < d_width) && (idx_bwd >= (idx_fwd + 1)))
		{
			//TR_INF << "Poly size: " << poly.getSize();
			if(poly.getSize() > 0)
			{
				ret = true;
				poly.removePoint(1);
			}
			idx_fwd++;
			length_fwd += seg_list[static_cast<int>(idx_fwd) -1].getLength(zoom_ref);
		}
		else
			do_fwd = false;
		if((length_bwd < d_width) && (idx_bwd >= (idx_fwd + 1)))
		{
			if(poly.getSize() > 0)
			{
				ret = true;
				poly.removePoint(poly.getSize());
			}
			idx_bwd--;
			length_bwd += seg_list[static_cast<int>(idx_bwd) -1].getLength(zoom_ref);
		}
		else
			do_bwd = false;
	}
	// TODO: check last point, remove the point?
	//TR_INF << "### end:   " << poly.getSize() << "F: " << length_fwd << idx_fwd << "B: " << length_bwd << idx_bwd;
	return ret;
}

void TrGeoSegment::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	if(!(m_inst_mask & TR_MASK_DRAW))
	{
        //return;
	}

	if(this->clip(zoom_ref))
		return;

	TrPoint screen_1 = m_first;
	zoom_ref.setMovePoint(&screen_1.x,&screen_1.y);
	TrPoint screen_2 = m_second;
	zoom_ref.setMovePoint(&screen_2.x,&screen_2.y);
	//TR_INF << screen_1.x << screen_1.y << screen_2.x << screen_2.y;
	p->setPen(QPen(QColor(0,200,0)));
	//p->drawLine(screen_1.x, screen_1.y, screen_2.x, screen_2.y);
	p->drawEllipse(static_cast<int>(screen_1.x)-5, static_cast<int>(screen_1.y)-5, 10, 10);
	p->drawRect(static_cast<int>(screen_2.x)-5, static_cast<int>(screen_2.y)-5, 10, 10);
}

#ifdef TR_SERIALIZATION
bool TrGeoSegment::exportGeoJson(QJsonObject & geojson, uint64_t mode)
{
	return false;
}

bool TrGeoSegment::importGeoJson(const QJsonObject & geojson, uint64_t mode)
{
	return false;
}

uint64_t TrGeoSegment::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TR_NO_VALUE;
}

void TrGeoSegment::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
}
#endif
