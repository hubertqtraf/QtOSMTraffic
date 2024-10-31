/******************************************************************
 *
 * @short	Wrapper to the geo lib
 *
 * project:	Trafalgar lib
 *
 * class:	TrZoomMap
 * superclass:  ---
 * modul:	tr_zoom_map.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2008
 *
 * @author	Schmid Hubert (C)2008-2022
 *
 * history:
 *
 ******************************************************************/

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


#include "geo_globe.h"

#include "tr_zoom_map.h"

#include <stdio.h>
#include <math.h>

TrZoomMap::TrZoomMap()
	: m_y_correction(1.0)
	, m_scale(1.0)
{
	m_visibleWorld[0].x = 0.0;
	m_visibleWorld[0].y = 0.0;
	m_visibleWorld[1].x = 1.0;
	m_visibleWorld[1].y = 1.1;

	m_screen_width=200;
	m_screen_height=101;

    TrgGlobeInit(&m_world_ref);

	m_move_x = m_move_y = 0;
}

int TrZoomMap::getErrorCode() const
{
	TrGeo2DRef * ref = (TrGeo2DRef*)&m_world_ref;
	int ret = ref->core.error_code;
	ref->core.error_code = 0;
	return ret;
}

void TrZoomMap::setCenter(double x1, double y1, double x2, double y2, TrPoint * center) const
{
	if(x2>x1)
		center->x = x1+((x2-x1)/2.0);
	else
		center->x = x2+((x1-x2)/2.0);
	if(y2>y1)
		center->y = y1+((y2-y1)/2.0);
	else
		center->y = y2+((y1-y2)/2.0);
}

void TrZoomMap::setScreenDimension(int w, int h)
{
	m_screen_width = w;
	m_screen_height = h;
}

int TrZoomMap::getScreenWidth() const
{
	return m_screen_width;
}

int TrZoomMap::getScreenHeight() const
{
	return m_screen_height;
}

void TrZoomMap::moveToWorldRect(double dx, double dy)
{
	m_visibleWorld[0].x += dx;
	m_visibleWorld[0].y += dy;
	m_visibleWorld[1].x += dx;
	m_visibleWorld[1].y += dy;

	m_move_x = m_move_y = 0;
}

void TrZoomMap::moveToPoint(int x, int y)
{
	m_move_x = m_move_y = 0;

	double xw = (x/(m_scale * m_y_correction)) + m_visibleWorld[0].x;
	double yw = (y/m_scale) + m_visibleWorld[0].y;

	double dx = m_screen_width/(m_scale * m_y_correction);
	double dy = m_screen_height/m_scale;

	this->setVisibleWorld(xw - (dx/2), yw - (dy/2), xw + (dx/2), yw + (dy/2));
}

void TrZoomMap::moveToPoint(int x, int y, double value)
{
	TrPoint pt1;
	TrPoint pt2;

	pt1.x = (m_screen_width/2.0);
	pt1.y = (m_screen_height/2.0);
	pt2.x = x;
	pt2.y = y;
	getPoint(&pt1.x, &pt1.y);
	getPoint(&pt2.x, &pt2.y);

	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	zoom2Center(value);
	zoom2Rect();
	pt2.x = x;
	pt2.y = y;

	getPoint(&pt2.x, &pt2.y);
	//TR_MSG << "move: " << pt2.x-pt1.x << pt2.y-pt1.y << (dx-(pt2.x-pt1.x)) << (dy-(pt2.y-pt1.y));
	moveToWorldRect((dx-(pt2.x-pt1.x)), (dy-(pt2.y-pt1.y)));
}

void TrZoomMap::zoom2Center(double factor)
{
	TrPoint center;

	this->setCenter(m_visibleWorld[0].x, m_visibleWorld[0].y,
		m_visibleWorld[1].x, m_visibleWorld[1].y, &center);

	double dx = center.x - m_visibleWorld[0].x;
	double dy = center.y - m_visibleWorld[0].y;

	m_visibleWorld[0].x = center.x - (dx * factor);
	m_visibleWorld[0].y = center.y - (dy * factor);
	m_visibleWorld[1].x = center.x + (dx * factor);
	m_visibleWorld[1].y = center.y + (dy * factor);
}

void TrZoomMap::setVisibleWorld(double x1, double y1, double x2, double y2)
{
	m_visibleWorld[0].x = x1;
	m_visibleWorld[0].y = y1;
	m_visibleWorld[1].x = x2;
	m_visibleWorld[1].y = y2;

	m_move_x = m_move_y = 0;
}

void TrZoomMap::zoom2Rect()
{
	zoom2Rect(m_visibleWorld[0].x, m_visibleWorld[0].y, m_visibleWorld[1].x, m_visibleWorld[1].y);
}

void TrZoomMap::zoom2Rect(double x1, double y1, double x2, double y2)
{
	TrPoint r_center;

	setCenter(x1, y1, x2, y2, &r_center);
	m_y_correction = Geo2DLocal((TrGeo2DRef*)&m_world_ref, r_center.x, r_center.y);

	double scale_h = fabs((double)m_screen_height / (y2-y1));
	m_scale = fabs((double)m_screen_width / ((x2-x1) * m_y_correction));

	if(scale_h < m_scale)
		m_scale = scale_h;
}

void TrZoomMap::setPoint(double * x, double * y) const
{
    *x -= m_visibleWorld[0].x;
    *y -= m_visibleWorld[0].y;

	*x *= (m_scale * m_y_correction);
	*y *= m_scale;

	*y = m_screen_height - *y;
}

void TrZoomMap::getPoint(double * x, double * y) const
{
	*y = m_screen_height - *y;

	*x /= (m_scale * m_y_correction);
	*y /= m_scale;

    *x += m_visibleWorld[0].x;
    *y += m_visibleWorld[0].y;
}

void TrZoomMap::getMetric(double * x, double * y, bool metric) const
{
	TrPoint res = {*x,*y};

	getMetric(res, metric);
	*x = res.x;
	*y = res.y;
}

void TrZoomMap::getMetric(TrPoint & pt, bool metric) const
{
	TrPoint res;

	if(metric)
	{
		res.x = Geo2DX((TrGeo2DRef*)&m_world_ref, pt.x, pt.y);
		res.y = Geo2DY((TrGeo2DRef*)&m_world_ref, pt.x, pt.y);
	}
	else
	{
		res.x = Geo2DOrigX((TrGeo2DRef*)&m_world_ref, pt.x, pt.y);
		res.y = Geo2DOrigY((TrGeo2DRef*)&m_world_ref, pt.x, pt.y);
	}
	pt = res;
}

void TrZoomMap::setMove(int x, int y)
{
	m_move_x = x;
	m_move_y = y;
}

void TrZoomMap::setMovePoint(double * x, double * y) const
{
	setPoint(x,y);
	*x += m_move_x;
	*y += m_move_y;
}

void TrZoomMap::getStraightLine(const TrPoint p1, const TrPoint p2, straight_line & sec) const
{
	TrPoint pt_m1 = p1;
	TrPoint pt_m2 = p2;
	geoGetLineByPoints((TrGeo2DRef*)&m_world_ref, &sec, pt_m1.x, pt_m1.y, pt_m2.x, pt_m2.y);
}

double TrZoomMap::getLength(double x1, double y1, double x2, double y2) const
{
	return Geo2DLen((TrGeo2DRef*)&m_world_ref, x1, y1, x2, y2);
}

double TrZoomMap::getLength(const TrPoint & p1, const TrPoint & p2) const
{
	return Geo2DLen((TrGeo2DRef*)&m_world_ref, p1.x, p1.y, p2.x, p2.y);
}

double TrZoomMap::getAngle(const TrPoint & p1, const TrPoint & p2) const
{
	straight_line line; //= ;
	TrPoint pt_m1 = p1;
	TrPoint pt_m2 = p2;

	geoGetLineByPoints((TrGeo2DRef*)&m_world_ref, &line, pt_m1.x, pt_m1.y, pt_m2.x, pt_m2.y);

	return geoGetAngle(&line);
}

int TrZoomMap::getSegParPoint(straight_line & sec, TrPoint & pt, double width) const
{
	return geoPolyParPoint((TrGeo2DRef*)&m_world_ref, &sec, pt.x, pt.y, &pt.x, &pt.y, width);
}

int TrZoomMap::getCrossPoint(poly_add & sec1, poly_add & sec2, TrPoint & pt) const
{
	//geoPolyParCrossPoint((TrGeo2DRef*)&m_world_ref, &sec1, &sec2, (double *)&pt,
	//		&(pt.x), &(pt.y),0,0);
	geoPolyCrossPoint((TrGeo2DRef*)&m_world_ref, sec1.sl, sec2.sl, &(pt.x), &(pt.y));
	return 0;
}

void TrZoomMap::getParCrossPoint(poly_add & sec1, poly_add & sec2, TrPoint & pt, double width1, double width2) const
{
	geoPolyParCrossPoint((TrGeo2DRef*)&m_world_ref, &sec1, &sec2, (double *)&pt,
		&(pt.x), &(pt.y), width1, width2);
}

double TrZoomMap::initPolyLen(poly_base * base) const
{
    return geoPolyInitLen(base->pt, base->n_pt, base->add, (TrGeo2DRef*)&m_world_ref);
}

void TrZoomMap::getLineByPoints(poly_add & sec, const TrPoint & first_point, const TrPoint & second_point) const
{
	geoGetSegmentByPoints((TrGeo2DRef*)&m_world_ref, &sec, first_point.x, first_point.y,
		second_point.x, second_point.y);
}

// TODO: polyAddInit/geoPolyClosest: keep in TrZoomMap class?
poly_add * TrZoomMap::polyAddInit(double * coor, int n) const
{
	return geoPolyAddInit(coor, n, (TrGeo2DRef*)&m_world_ref);
	//if(geoPoly2DInit(&m_base, TrGeo2DRef * ref) == (-1))
}

double TrZoomMap::getSegmentClosest(poly_add * orig, TrPoint seg_point, TrPoint & pt) const
{
	poly_add pa2;
	double r;
	int s;

	return geoPoly2DSegmentClosest((TrGeo2DRef*)&m_world_ref, orig, &pa2, seg_point.x, seg_point.y, &pt.x, &pt.y, &r, &s);
}

