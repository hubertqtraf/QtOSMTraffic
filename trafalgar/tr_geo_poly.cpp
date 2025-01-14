/******************************************************************
 *
 * @short	polygon class of trafalgar geometric selectable objects 
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPolygon
 * superclass:	TrGeoObject
 * modul:	tr_geo_poly.cc	geometric selectable object	
 * @version	0.1
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2009-2025
 *
 * beginning:	06.2009
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

#include <math.h>


#include "tr_geo_poly.h"

#include "tr_map_list.h"

#include "tr_map_node.h"

TrGeoPolygon::TrGeoPolygon() 
	: TrGeoObject()
	, stdPen(nullptr)
{
	m_base.pt = nullptr;
	m_base.n_pt = 0;
	m_base.add = nullptr;
}

TrGeoPolygon::~TrGeoPolygon()
{
	geoPoly2DDelete(&m_base);
}

QDebug operator<<(QDebug dbg, const TrGeoPolygon& poly)
{
	return dbg << poly.m_base.n_pt;
}

// TODO check all fields deleted?
void TrGeoPolygon::clearData()
{
	//TR_MSG;
	geoPoly2DDelete(&m_base);
	m_base.n_pt = 0;
}

// static
int TrGeoPolygon::calcParPoint(const TrZoomMap & zoom_ref, TrPoint & pt, straight_line & seg, int32_t width)
{
	return zoom_ref.getSegParPoint(seg, pt, width/1000.0);
}

bool TrGeoPolygon::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	//if(ctrl & TR_INIT_GEOMETRY)
	{
		//TR_MSG;
		this->setInfo(zoom_ref);
	}
	if(ctrl & TR_INIT_COLORS)
	{
		if(base != nullptr)
		{
			TrMapList * list = dynamic_cast<TrMapList *>(base);
			if(list != nullptr)
			{
				// 0x1005 -> construction_1
				QPen * str_pen = list->getObjectPen(0x1005);
				if(str_pen != nullptr)
				{
					m_geo_active_pen = str_pen;
				}
			}
		}
	}
	return true;
}

size_t TrGeoPolygon::getSize()
{
	return m_base.n_pt;
}

bool TrGeoPolygon::hasSegmentInfo()
{
	return (m_base.add != nullptr);
}

TrPoint TrGeoPolygon::getPoint(size_t id)
{
	//TR_INF << hasSegmentInfo() << getSize();
    if(m_base.n_pt <= id)
    {
        TrPoint pt = {0.0,0.0};
        return pt;
    }
    TrPoint * pt = (TrPoint*)(m_base.pt + (id * 2));

	return *pt;
}

TrPoint TrGeoPolygon::getFirstPoint()
{

	return this->getPoint(0);
}

TrPoint TrGeoPolygon::getLastPoint()
{
	return this->getPoint(m_base.n_pt-1);
}

bool TrGeoPolygon::getPoints(QVector<TrPoint> & points)
{
	points.clear();
	for(size_t i = 0; i < getSize(); i++)
	{
		//TrPoint sec = getPoint(i);
		points.append(getPoint(i));
	}
	return false;
}

void TrGeoPolygon::getScreenPoints(const TrZoomMap & zoom_ref, QPolygon & poly)
{
	poly.clear();
	TrPoint screen;
	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		screen.x = m_base.pt[i*2];
		screen.y = m_base.pt[(i*2)+1];
		zoom_ref.setMovePoint(&screen.x,&screen.y);
		poly.append(QPoint(static_cast<int>(screen.x), static_cast<int>(screen.y)));
	}
}

bool TrGeoPolygon::setPoint(size_t id, TrPoint & pt)
{
	//TR_INF << "set:" << id << pt.x << pt.y;
	// TODO: check the id/size
	// poly_points[id] = pt;
	m_base.pt[id*2] = pt.x;
	m_base.pt[(id*2)+1] = pt.y;
	return true;
}

const poly_add & TrGeoPolygon::getGeoSegmentData(size_t id)
{
	return m_base.add[id];
}


void TrGeoPolygon::createSelectPoints()
{
}

// TODO check both functions
double TrGeoPolygon::getLength(const TrZoomMap & zoom_ref)
{
	return zoom_ref.initPolyLen(&m_base);
}

double TrGeoPolygon::getLength(const TrZoomMap & zoom_ref, TrPoint pta, TrPoint ptb)
{
	if(getSize() > 0)
	{
		TrPoint first = pta;
		double len = 0;
		for(size_t i = 0; i < getSize(); i++)
		{
			TrPoint sec = getPoint(i);
			len += zoom_ref.getLength(first.x, first.y, sec.x, sec.y);
			first = sec;
		}
		return(len + zoom_ref.getLength(first.x, first.y, ptb.x, ptb.y));
	}
	return zoom_ref.getLength(pta.x, pta.y, ptb.x, ptb.y);
}


double TrGeoPolygon::getSegmentsLength()
{
	double length = 0.0;

	if((m_base.add != nullptr) && (m_base.n_pt > 1))
	{
		for (unsigned int i = 0; i < m_base.n_pt-1; ++i)
		{
			//TR_MSG << m_base.add[i].len_part;
			length += m_base.add[i].len_part;
		}
	}
	return length;
}

void TrGeoPolygon::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	//TR_MSG << HEX << m_inst_mask << mode;

	if(!m_geo_active_pen)
	{
		TR_MSG << "m_geo_active_pen is nullptr" << m_geo_active_pen;
		return;
	}

	QPolygon poly(m_base.n_pt);

	TrPoint screen;

	if(!(m_inst_mask & TR_MASK_DRAW))
		return;

	if(this->clip(zoom_ref))
		return;

	// TODO: points use the default/active pen - set a marker pen?
	p->setPen(*m_geo_active_pen);
	p->setBrush(Qt::NoBrush);
	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		screen.x = m_base.pt[i*2];
		screen.y = m_base.pt[(i*2)+1];
		zoom_ref.setMovePoint(&screen.x,&screen.y);

		poly.setPoint(i, screen.x, screen.y);

		if(m_inst_mask & TR_MASK_SHOW_POINTS)
			p->drawRect(static_cast<int>(screen.x-4), static_cast<int>(screen.y-4), 8, 8);
	}

	if(!poly.size())
	{
		//TR_MSG << "no size";
		return;
	}

	p->setPen(*m_geo_active_pen);
	//p->setPen(QPen(QColor(0,0,255)));

	if(mode == 0x02)
	{
		p->setBrush(m_geo_active_pen->color());
		p->drawPolygon(poly);
	}
	// TODO: test -> draw selected objects
	else if(mode == 0x03)
	{
		p->drawPolyline(poly);
	}
	else
	{
		if(m_inst_mask & TR_POLY_TYPE_DEF)
		{
			if(m_inst_mask & TR_POLY_SHOW_ROUTE)
			{
				// TODO: replace hard coded values
				m_geo_active_pen->setWidth(5);
				p->setPen(*m_geo_active_pen);
				p->drawPolyline(poly);
			}
			if(m_inst_mask & TR_POLY_SHOW_TRACK)
			{
				m_geo_active_pen->setWidth(2);
				p->setPen(*m_geo_active_pen);
				p->drawPolyline(poly);
			}
			if((s_mask & TR_MASK_SHOW_CONSTRUCT) && (m_inst_mask & TR_MASK_SHOW_CONSTRUCT))
				p->drawPolyline(poly);
			if(!(m_inst_mask & TR_MASK_SHOW_CONSTRUCT))
				p->drawPolyline(poly);
		}
	}
	if(m_inst_mask & TR_MASK_SELECTED)
	{
		// TODO: check the angle error -> link?
		if(m_inst_mask & TR_POLY_SHOW_ANG_ERR)
			TrGeoObject::drawSurroundingRect(zoom_ref, p, TR_OBJ_DRAW_ERR);
		else
			TrGeoObject::drawSurroundingRect(zoom_ref, p, 0);
	}
}

void TrGeoPolygon::draw(const TrZoomMap & zoom_ref, QPainter * p, TrPoint from, TrPoint to, uint8_t mode)
{
	//TR_MSG << mode;;

	TrPoint pt = from;
	TrPoint screen;

	QPolygon poly(m_base.n_pt + 2);

	screen.x = pt.x;
	screen.y = pt.y;
	zoom_ref.setMovePoint(&screen.x,&screen.y);
	poly.setPoint(0, static_cast <int>(screen.x),
		static_cast <int>(screen.y));

	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		screen.x = m_base.pt[i*2];
		screen.y = m_base.pt[(i*2)+1];
		zoom_ref.setMovePoint(&screen.x,&screen.y);

		poly.setPoint(i+1, static_cast <int>(screen.x), screen.y);
		// TODO: test
		if(mode != 0x02)
			p->drawRect(static_cast <int>(screen.x-4),
				static_cast <int>(screen.y-4), 8, 8);
	}
	pt = to;
	screen.x = pt.x;
	screen.y = pt.y;
	zoom_ref.setMovePoint(&screen.x,&screen.y);
	poly.setPoint(m_base.n_pt+1, screen.x, screen.y);

	if(m_geo_active_pen == nullptr)
	{
		p->setPen(QPen(QColor(0,250,0)));
	}
	else
	{
		//p->setPen(QPen(QColor(250,0,0)));
		p->setPen(*m_geo_active_pen);
	}
	p->drawPolyline(poly);
}

void TrGeoPolygon::appendPoints(const QVector<TrPoint> & next_points)
{
	QVector<TrPoint> poly_points;

	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		TrPoint pt;
		pt.x = m_base.pt[i*2];
		pt.y = m_base.pt[(i*2)+1];
		poly_points.append(pt);
	}

	//poly_points.append(next_point);
	poly_points += next_points;

	if(m_base.pt != nullptr)
	{
		delete m_base.pt;
		m_base.pt = nullptr;
	}

	geoPoly2DNew(&m_base, poly_points.size());

	for (int i = 0; i < poly_points.size(); ++i)
	{
		m_base.pt[i*2] = poly_points.at(i).x;
		m_base.pt[(i*2)+1] = poly_points.at(i).y;
	}
}

bool TrGeoPolygon::setSurroundingRect()
{
	if(m_base.n_pt <1)
	{
		return false;
	}

	double rect[4];
	rect[0] = rect[2] = m_base.pt[0];
	rect[1] = rect[3] = m_base.pt[1];
	updateSurroundRect(rect, true);

	for (unsigned int i = 1; i < m_base.n_pt; ++i)
	{
		rect[0] = rect[2] = m_base.pt[i*2];
		rect[1] = rect[3] = m_base.pt[(i*2)+1];
		updateSurroundRect(rect, false);
	}
	return true;
}

#ifdef TR_SERIALIZATION
QString TrGeoPolygon::getXmlDescription()
{
	return QString();
}
#endif

void TrGeoPolygon::setInfo(const TrZoomMap & zoom_ref)
{
	if(m_base.n_pt < 2)
	{
		if(m_base.add != nullptr)
		{
			TR_INF << "m_base.add != nullptr";
		}
		m_base.add = nullptr;
		return;
	}
	m_base.add = zoom_ref.polyAddInit(m_base.pt, m_base.n_pt);

	if(m_base.add == nullptr)
	{
		TR_ERR << "init";
	}
	this->getLength(zoom_ref);
}


void TrGeoPolygon::setInfoSect(const TrZoomMap & zoom_ref, poly_add & sec,
	TrPoint first_point, TrPoint second_point)
{
	sec.len_part = zoom_ref.getLength(first_point.x, first_point.y, second_point.x, second_point.y);

	//TR_MSG << first_point.x << first_point.y << " # " << second_point.x << second_point.y;
	zoom_ref.getLineByPoints(sec, first_point, second_point);
	int code = zoom_ref.getErrorCode();
	if(code)
	{
		switch(code)
		{
		case 1:
			// TODO: check the warnig
			TR_WRN << "DIR" << code;
			break;
		case 2:
			TR_WRN << "INF" << code;
			break;
		default:
			TR_WRN << "UNKNOWN" << code;
			break;
		}
	}
	// TODO: check the length
	//sec.len_part = zoom_ref.getLength(first_point.x, first_point.y, second_point.x, second_point.y);
}



bool TrGeoPolygon::checkAngle(poly_add & pa, poly_add & pa1, double angle_b, double angle_a)
{	
	if((fabs(pa1.sl.slope - pa.sl.slope) > 0.00001) &&
		(((fabs(angle_b - angle_a) < 3.00)) ||
		((fabs(angle_b - angle_a) > 3.28))))
	{
		return true;
	}
	// TODO: more checks
	if((pa.sl.flags & DIR_RIGHT) == (pa1.sl.flags & DIR_RIGHT))
	{
		//TR_MSG << (pa.sl.flags & DIR_RIGHT) << (pa1.sl.flags & DIR_RIGHT);
		return true;
	}
	//TR_MSG << pa1.sl.slope << " " << pa.sl.slope << " " << angle_b << " " << angle_a << pa.sl.flags << pa1.sl.flags;
	return false;
}


int TrGeoPolygon::parallel(TrGeoPolygon * base, const TrZoomMap & zoom_ref, int32_t width)
{
    int ret = 0;
	double float_width = width/1000.0;
	size_t n = base->getSize();

	if(!base->hasSegmentInfo())
	{
		TR_WRN << "no SegmentInfo";
		return -1;
	}

	poly_add sl1 = base->getGeoSegmentData(0);
	QVector<TrPoint> par_line;

	TrPoint pt = base->getPoint(0);

	if(TrGeoPolygon::calcParPoint(zoom_ref, pt, sl1.sl, width))
	{
		TR_WRN << zoom_ref.getErrorCode();
		ret = -1;
	}
	par_line.append(pt);

	for (unsigned int i = 0; i < (n-2); ++i)
	{
		poly_add sl1a = base->getGeoSegmentData(i);
		poly_add sl2a = base->getGeoSegmentData(i+1);

		TrPoint pt = base->getPoint(i+1);
		zoom_ref.getParCrossPoint(sl1a, sl2a, pt, float_width, float_width);
		par_line.append(pt);
	}

	pt = base->getPoint(n-1);
	poly_add sl2 = base->getGeoSegmentData(n-2);

	if(TrGeoPolygon::calcParPoint(zoom_ref, pt, sl2.sl, width))
	{
		TR_WRN << zoom_ref.getErrorCode();
		ret = -1;
	}
	par_line.append(pt);

	geoPoly2DNew(&m_base, par_line.size());
	for(int i=0; i < par_line.size(); i++)
	{
		m_base.pt[i*2] = par_line[i].x;
		m_base.pt[(i*2)+1] = par_line[i].y;
	}
	return ret;
}


int TrGeoPolygon::parallel(QVector<TrPoint> * par_line, const TrZoomMap & zoom_ref,
	straight_line first, straight_line last, int32_t width)
{
    double float_width = width/1000.0;
	TrPoint pt1;
	poly_add pa, pa1;

	//TR_MSG << par_line->size() << " - " << poly_points.size();

	if((m_base.add == nullptr) && (m_base.n_pt > 1))
	{
		TR_WRN << "m_base.add == nullptr" << getSize();
		return -1;
	}

	pa.sl.intercept = first.intercept;
        pa.sl.slope = first.slope;
        pa.sl.flags = first.flags;
	pa.len_part = 0.0;

	pt1.x = m_base.pt[0];
	pt1.y = m_base.pt[1];

	double angle_a = geoGetAngle(&first);
	// test -> as init, TODO: check -> done? 
	double angle_b = angle_a;

	m_inst_mask &= (~TR_POLY_SHOW_ANG_ERR);

	for (unsigned int i = 0; i < m_base.n_pt-1; ++i)
	{
		pa1.sl.intercept = m_base.add[i].sl.intercept;
		pa1.sl.slope = m_base.add[i].sl.slope;
		pa1.sl.flags = m_base.add[i].sl.flags;
		pa1.len_part = 0.0;

		angle_b = geoGetAngle(&pa1.sl);

		if(TrGeoPolygon::checkAngle(pa1, pa, angle_a, angle_b))
		{
			zoom_ref.getParCrossPoint(pa, pa1, pt1, float_width, float_width);
			par_line->append(pt1);
		}
		else
		{
			m_inst_mask |= TR_POLY_SHOW_ANG_ERR;
		}

		pt1.x = m_base.pt[(i+1)*2];
		pt1.y = m_base.pt[((i+1)*2)+1];

		angle_a = angle_b;
		pa.sl.intercept = pa1.sl.intercept;
		pa.sl.slope = pa1.sl.slope;
		pa.sl.flags = pa1.sl.flags;
		pa.len_part = 0.0;
	}

	pa1.sl.intercept = last.intercept;
	pa1.sl.slope = last.slope;
	pa1.sl.flags = last.flags;
	pa1.len_part = 0.0;

	angle_b = geoGetAngle(&pa1.sl);

	if(checkAngle(pa1, pa, angle_a, angle_b))
	{
		zoom_ref.getParCrossPoint(pa, pa1, pt1, float_width, float_width);
		par_line->append(pt1);
	}
	else
	{
		m_inst_mask |= TR_POLY_SHOW_ANG_ERR;
	}
	//par_line->append(pt1);
	//TR_MSG << par_line->size();
	return 0;
}

uint64_t TrGeoPolygon::findSelectInside(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	TrPoint pt;

	for(size_t i = 0; i < m_base.n_pt; ++i)
	{
		pt.x = m_base.pt[i*2];
		pt.y = m_base.pt[(i*2)+1];

		if(TrMapNode::isCloseToPoint(zoom_ref, inside, pt))
			return i+1;
	}
	return TR_NO_VALUE;
}

bool TrGeoPolygon::removePoint(uint64_t id)
{
	if(id == TR_NO_VALUE)
		return false;

	QList<TrPoint> poly_points;

	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		TrPoint pt;
		pt.x = m_base.pt[i*2];
		pt.y = m_base.pt[(i*2)+1];
		poly_points.append(pt);
	}
	// TODO: first point index is '1' not '0'-> change?
    //TR_INF << id << (id-1);
	if(id == 0)
	{
		TR_WRN << id;
		return false;
	}
	poly_points.removeAt(id-1);
	clearData();
	appendPoints(poly_points.toVector());
	return true;
}

bool TrGeoPolygon::addPoint(const TrZoomMap & zoom_ref, const TrPoint & pt)
{
	QList<TrPoint> poly_points;
	QList<double> poly_dist;

	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		TrPoint ppt;
		ppt.x = m_base.pt[i*2];
		ppt.y = m_base.pt[(i*2)+1];
		poly_points.append(ppt);
		if(i)
		{
			double d = zoom_ref.getLength(m_base.pt[(i-1)*2], m_base.pt[((i-1)*2)+1], pt.x, pt.y);
			d += zoom_ref.getLength(ppt.x, ppt.y, pt.x, pt.y);
			poly_dist.append(d);
		}
	}
	// TODO: just hack! need the second point for the link -> rework! (addPoint for links?)
	if(poly_dist.size() == 0)
	{
		poly_points.insert(1, pt);
		clearData();
		appendPoints(poly_points.toVector());
		init(zoom_ref,0);
		return false;
	}
	double dist = 1000000.0;
	int idx = 0;
	for (int i = 0; i < poly_dist.size(); ++i)
	{
		if(dist > poly_dist[i])
		{
			//TR_MSG << i << idx;
			dist = poly_dist[i];
			idx = i;
		}
	}

	TR_MSG << idx;

	// TODO: use getPolyClosest?
	//zoom_ref.getPolyClosest(poly_base * base, double * x, double * y, double * rel_dist, int * seg)

	poly_points.insert(idx+1, pt);

	clearData();
	appendPoints(poly_points.toVector());

	return true;
}

double TrGeoPolygon::getClosest(const TrZoomMap & zoom_ref, TrPoint & pt, int & seg, double & rel_dist)
{
	double dist = 10000.0; // -1.0;

	if(m_base.add == nullptr)
	{
		TR_WRN << "m_base.add == nullptr";
		return -1.0;
	}

	if(getSize() < 2)
		return -1.0;

	TrPoint test = pt;
	TrPoint test1 = pt;
	int n = -1;

	TrPoint first = getPoint(0);
	TrPoint second = getPoint(1);

	for(unsigned int i = 0; i < (m_base.n_pt-1); i++)
	{
		test = pt;
		first = getPoint(i);
		second = getPoint(i+1);
		double len = zoom_ref.getLength(first, second);
		double xdist1 = zoom_ref.getSegmentClosest(m_base.add + i, first, test);
		double d1 = zoom_ref.getLength(first, test);
		double d2 = zoom_ref.getLength(second, test);

		if((d1 < len) && (d2 < len))
		{
			if(xdist1 < dist)
			{
				dist = xdist1;
				n = i;
				test1 = test;
                rel_dist = d1/len;
			}
		}
	}
	seg = n;
	pt = test1;

	return dist;
}

#ifdef TR_SERIALIZATION
bool TrGeoPolygon::exportGeoJson(QJsonObject & geojson, uint64_t mode)
{
	//TR_MSG;
	QJsonObject geometry;
	QJsonArray points;

	geojson["type"] = "Feature";

	for (unsigned int i = 0; i < m_base.n_pt; ++i)
	{
		QJsonArray point;
		double coor = m_base.pt[i*2];
		point.append(coor/TR_COOR_FACTOR);
		coor = m_base.pt[(i*2)+1];
		point.append(coor/TR_COOR_FACTOR);
		points.append(point);
	}
	geometry["type"] = "LineString";
	geometry["coordinates"] = points;

	geojson["geometry"] = geometry;
	QJsonValue tr_value(geojson);

	return true;
}

bool TrGeoPolygon::importArrayJson(const QJsonArray & arrjson, uint64_t mode)
{
	QVector<TrPoint> next_points;

	for(int i = 0; i < arrjson.size(); ++i)
	{
		QJsonArray xyArray = arrjson[i].toArray();

		TrPoint pt;
		for(int j = 0; j < xyArray.size(); ++j)
		{
			if(j == 0)
				pt.x = xyArray[j].toDouble() * TR_COOR_FACTOR;
			if(j == 1)
				pt.y = xyArray[j].toDouble() * TR_COOR_FACTOR;
		}
		next_points.append(pt);
	}
	appendPoints(next_points);
	setSurroundingRect();
	return true;
}

bool TrGeoPolygon::importGeoJson(const QJsonObject & geojson, uint64_t mode)
{
	if(geojson.contains("type"))
	{
		if(geojson.value("type").toString() != "LineString")
			return false;
	}
	if(geojson.contains("coordinates"))
	{
		QJsonArray coordinatesArray = geojson["coordinates"].toArray();

		return importArrayJson(coordinatesArray, 0);
	}
	return false;
}

bool TrGeoPolygon::readXmlPoint(QXmlStreamReader & xml_in, QVector<TrPoint> & poly_points)
{
	while(!xml_in.atEnd())
	{
		if(xml_in.isCharacters())
		{
			xml_in.readNext();
		}
		else if(xml_in.isStartElement())
		{
			if(xml_in.name() == "point")
			{
				QXmlStreamAttributes attr = xml_in.attributes();
		
				TrPoint pt;
				pt.x = attr.value("", "lon").toDouble() * TR_COOR_FACTOR;
				pt.y = attr.value("", "lat").toDouble() * TR_COOR_FACTOR;

				poly_points.append(pt);
			}
			xml_in.readNext();
		}
		else if(xml_in.isEndElement())
		{
			xml_in.readNext();
			return true;
		}
	}

	return false;		
}

uint64_t TrGeoPolygon::readXmlDescription(QXmlStreamReader & xml_in)
{
	int n_check = (-1);
	uint64_t id_check = TR_NO_VALUE;

	QVector<TrPoint> poly_points;

	QXmlStreamAttributes attr = xml_in.attributes();
	n_check = attr.value("", "n").toInt();
	id_check = attr.value("", "id").toInt();
	uint64_t type = attr.value("", "type").toInt();
	m_inst_mask = 0;
    if(type & TR_POLY_RW_DEF)
		setMask(TR_POLY_TYPE_DEF);
	if(type & TR_POLY_RW_CONSTRUCT)
		setMask(TR_MASK_SHOW_CONSTRUCT);
	xml_in.readNext();

	while(!xml_in.atEnd())	
	{
		if(xml_in.isComment())
		{
			TR_WRN << xml_in.tokenString() << ": " << xml_in.text();
			xml_in.readNext();
		}	
		else if(xml_in.isCharacters())
		{
			xml_in.readNext();
		}
		else if(xml_in.isStartElement())
		{
			if(xml_in.name() == "point")
			{
				if(readXmlPoint(xml_in, poly_points) == false)
				{
					TR_ERR << "read point";
					return TR_NO_VALUE;
				}	
			}
	
			xml_in.readNext();

		}
		else if(xml_in.isEndElement())
		{
			if(xml_in.name() == "geo_poly")
			{
				xml_in.readNext();
				if(n_check != poly_points.size())
				{
					TR_ERR << "size" << poly_points.size() << " - " << n_check;
					//return TR_NO_VALUE;
				}
				double start_x;
				double start_y;
				int n = 0;
				if(poly_points.size())
				{
					start_x = poly_points[0].x;
					start_y = poly_points[0].y;
					n++;
				}
				for(int i=1; i<poly_points.size(); i++)
				{
					if(((fabs(start_x - poly_points[i].x)) < 0.01) &&
						(fabs(start_y - poly_points[i].y) < 0.01))
					{
						TR_INF << TR_COOR_VAL(start_x) << TR_COOR_VAL(poly_points[i].x)
							<< TR_COOR_VAL(start_y) << TR_COOR_VAL(poly_points[i].y);
						// TODO: rework! just a hack ;-)
                        setMask(TR_MASK_DRAW | TR_MASK_SHOW_POINTS);
                    }
					start_x = poly_points[i].x;
					start_y = poly_points[i].y;
				}
				geoPoly2DNew(&m_base, poly_points.size());
				for(int i=0; i<poly_points.size(); i++)
				{
					m_base.pt[i*2] = poly_points[i].x;
					m_base.pt[(i*2)+1] = poly_points[i].y;
				}
			}
			return id_check;
		}
	}
	return TR_NO_VALUE;
}

void TrGeoPolygon::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement("geo_poly");

	xml_out.writeAttribute("n", QVariant(m_base.n_pt).toString());

	xml_out.writeAttribute("id", QVariant((qulonglong)id).toString());

	uint64_t mode = 0;
	if(m_inst_mask & TR_POLY_TYPE_DEF)
		mode |= TR_POLY_RW_DEF;
	if(m_inst_mask & TR_MASK_SHOW_CONSTRUCT)
		mode |= TR_POLY_RW_CONSTRUCT;

	xml_out.writeAttribute("type", QVariant((qulonglong)(mode)).toString());

	// write out cordinates
	for (unsigned int i = 0; i < m_base.n_pt;  ++i)
	{
		xml_out.writeStartElement("point");

		xml_out.writeAttribute("lon",QVariant((m_base.pt[i*2])/TR_COOR_FACTOR).toString());
		xml_out.writeAttribute("lat",QVariant((m_base.pt[(i*2)+1])/TR_COOR_FACTOR).toString());

		xml_out.writeEndElement();
	}
	xml_out.writeEndElement();
}
#endif


