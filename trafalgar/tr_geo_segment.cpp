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
 * @author	Schmid Hubert (C)2023-2026
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

double TrGeoSegment::ms_min_size = 0.000001;
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
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW | TR_MASK_DATA);
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

bool TrGeoSegment::pointsAreIdentical()
{
	return ((fabs(m_first.x - m_second.x) < ms_min_size) && (fabs(m_first.y - m_second.y) < ms_min_size));
}

void TrGeoSegment::setPoints(TrPoint first, TrPoint second)
{
	m_first = first;
	m_second = second;
	m_inst_mask |= TR_MASK_DATA;
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

TrGeoSegment TrGeoSegment::getSegBorder(const QVector<TrPoint> &vec, bool dir)
{
	TrGeoSegment ret;
	if(vec.size() > 1)
	{
		if(dir)
			ret.setPoints(vec[0], vec[1]);
		else
			ret.setPoints(vec[vec.size()-2], vec[vec.size()-1]);
	}
	return ret;
}

bool TrGeoSegment::getSection(const TrZoomMap &zoom_ref, TrPoint &pt, double sec, bool dir)
{
	double len = getLength(zoom_ref);
	if(sec > len)
		return false;

	double fac = sec/len;
	double dx = m_second.x - m_first.x;
	double dy = m_second.y - m_first.y;
	if(dir)
	{
		pt.x = m_first.x + (dx * fac);
		pt.y = m_first.y + (dy * fac);
	}
	else
	{
		pt.x = m_second.x - (dx * fac);
		pt.y = m_second.y - (dy * fac);
	}
	return true;
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

	if(!(m_inst_mask & TR_MASK_DATA))
	{
		TR_WRN << "missing data";
		return false;
	}
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

double TrGeoSegment::getAngleDiff(const TrZoomMap & zoom_ref, double other) const
{
	double diff = fabs(getAngle(zoom_ref) - other);
	if(diff > (M_PI * 2.0))
		diff -= (M_PI * 2.0);
	return diff;
}

void TrGeoSegment::doReverse()
{
	TrPoint cp = m_first;
	m_first = m_second;
	m_second = cp;
}

int TrGeoSegment::getAngleCode(const TrZoomMap & zoom_ref, const TrGeoSegment &other, double & ang, double level)
{
	double ang1 = getAngle(zoom_ref);
	double ang2 = other.getAngle(zoom_ref);
	int err_code = zoom_ref.getErrorCode();

	if(err_code)
	{
		//TR_ERR << err_code << *this;
		return -1;
	}
	if((ang1 > 99.0) || (ang2 > 99.0))
		return -2;

	double diff = ang = abs(ang1 - ang2);
	if(diff > (M_PI * 2.0))
	{
		ang = abs(diff - (M_PI * 2.0));
	}
	if(diff > M_PI)
	//if(diff > (M_PI * 2.0))
	{
		diff = abs(diff - (M_PI * 2.0));
	}
	if(diff < level)
	{
		return 1;
	}
	diff = abs(diff - M_PI);
	if(diff < level)
	{
		return 2;
	}
	diff = abs(diff - M_PI_2);
	if(diff < level)
	{
		return 3;
	}
	return 0;
}


double TrGeoSegment::getLength(const TrZoomMap & zoom_ref)
{
	if(m_inst_mask & TR_MASK_DATA)
		return zoom_ref.getLength(m_first, m_second);
	TR_WRN << "missing data";
	return 0.0;
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

TrPoint TrGeoSegment::getPointByAngle(const TrZoomMap & zoom_ref, double ang, bool dir)
{
	TrPoint ret = zoom_ref.getPointByAngle(m_first, m_second, ang);//ang + thisAng);
	return ret;
}

bool TrGeoSegment::managePoints(const TrZoomMap & zoom_ref, QVector<TrPoint> & pts, int mode, double value)
{
	TrGeoSegment seg;
	bool fwd = true;

	// cut the ends on small segments
	if(mode == 1)
	{
		double len_fwd = -1.0;
		double len_bwd = -1.0;

		while(pts.size())
		{
			if(fwd)
			{
				seg.setPoints(m_first, pts[0]);
				len_fwd = seg.getLength(zoom_ref);
				if(len_fwd < value)
					pts.removeFirst();
				fwd = false;
			}
			else
			{
				seg.setPoints(pts.last(), m_second);
				len_bwd = seg.getLength(zoom_ref);
				if(len_bwd < value)
					pts.removeLast();
				fwd = true;
			}
			if((len_fwd >= value) && (len_bwd >= value))
				return true;
		}
		return false;
	}
	// isEvenPolygon
	if(mode == 2)
	{
		double diff = 400.0;
		double ang = getAngle(zoom_ref);

		TrPoint start = m_first;
		TrGeoSegment seg;
		for (int i = 0; i < pts.size(); ++i)
		{
			seg.setPoints(start, pts[i]);
			diff = getAngleDiff(zoom_ref, seg.getAngle(zoom_ref));
			//TR_INF << " - " << seg.getAngle(zoom_ref) << diff << seg;
			if(diff > value)
				return false;
			start = pts[i];
		}
		seg.setPoints(pts.last(), m_second);
		diff = fabs(ang - seg.getAngle(zoom_ref));
		if(diff > value)
			return false;
		return true;
	}
	return false;
}

void TrGeoSegment::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	if(!(m_inst_mask & TR_MASK_DRAW))
	{
		//return;
	}
	if(!(m_inst_mask & TR_MASK_DATA))
		return;

	if(this->clip(zoom_ref))
		return;

	TrPoint screen_1 = m_first;
	zoom_ref.setMovePoint(&screen_1.x,&screen_1.y);
	TrPoint screen_2 = m_second;
	zoom_ref.setMovePoint(&screen_2.x,&screen_2.y);
	//TR_INF << screen_1.x << screen_1.y << screen_2.x << screen_2.y;
	QPen xpen = QPen(QColor(0,200,0));
	xpen.setWidth(5);
	p->setPen(xpen);
	p->drawLine(screen_1.x, screen_1.y, screen_2.x, screen_2.y);
	p->drawEllipse(static_cast<int>(screen_1.x)-8, static_cast<int>(screen_1.y)-5, 18, 10);
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
