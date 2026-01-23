/******************************************************************
 *
 * @short	Wrapper to the geo lib
 *
 * project:	Trafalgar lib
 *
 * class:	TrZoomMap
 * superclass:	---
 * modul:	tr_zoom_map.h
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


#ifndef TR_ZOOM_MAP
#define TR_ZOOM_MAP

//#include "geo_ref.h"
#include "geo_globe.h"
#include "geo_poly.h"
#include "tr_point.h"

class TrZoomMap
{
public:	
	TrZoomMap();

	int getErrorCode() const;

	void setVisibleWorld(double x1, double y1, double x2, double y2);
	void setScreenDimension(int w, int h);
	
	int getScreenWidth() const;
	int getScreenHeight() const;
	
	void moveToPoint(int x, int y);
	void moveToPoint(int x, int y, double value);
	void moveToWorldRect(double dx, double dy);
	void setCenter(double x1, double y1, double x2, double y2, TrPoint * center) const;
	void zoom2Rect(double x1, double y1, double x2, double y2);
	void zoom2Rect();
	void zoom2Center(double factor);

	void setPoint(double * x, double * y) const;
	void getPoint(double * x, double * y) const;

	void getMetric(double * x, double * y, bool metric) const;
	void getMetric(TrPoint & pt, bool metric) const;

	void setMove(int x, int y);
	void setMovePoint(double * x, double * y) const;
	void getStraightLine(const TrPoint p1, const TrPoint p2, straight_line & sec) const;

	double getLength(double x1, double y1, double x2, double y2) const;
	double getLength(const TrPoint & p1, const TrPoint & p2) const;

	double getAngle(const TrPoint & p1, const TrPoint & p2) const;

	int getSegParPoint(straight_line & sec, TrPoint & pt, double width) const;

	int getCrossPoint(poly_add & sec1, poly_add & sec2, TrPoint & pt) const;
	void getParCrossPoint(poly_add & sec1, poly_add & sec2, TrPoint & pt, double width1, double width2) const;

	double initPolyLen(poly_base * base) const;
	poly_add * polyAddInit(double * coor, int n) const;

	void getLineByPoints(poly_add & sec, const TrPoint & first_point, const TrPoint & second_point) const;

	TrPoint getPointByAngle(const TrPoint & first_point, const TrPoint & second_point, double ang) const;

	double getSegmentClosest(poly_add * orig,  TrPoint seg_point, TrPoint & pt) const;
private:
	double m_y_correction;
	int m_screen_width;
	int m_screen_height;
	TrPoint m_visibleWorld[2];
	double m_scale;
	TrGeo2DGlobe m_world_ref;
	int m_move_x;
	int m_move_y;
};

#endif // TR_ZOOM_MAP

