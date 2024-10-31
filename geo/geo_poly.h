/******************************************************************
 *
 * @short	geometrical functions, polygon handling 
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_poly.h	geometrical  function defs	
 * @version:	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2005-2020
 *
 * beginning:	12.2005
 *
 * history:
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


#ifndef _geo_poly_h
#define _geo_poly_h

#include <stdlib.h>

#include "geo_base.h"
#include "geo_ref.h"

typedef struct
{
	straight_line sl;
	double len_part;
}poly_add;

typedef struct
{
	unsigned int n_pt;
	double length;
	double * pt;
	poly_add * add;
}poly_base;

int geoPoly2DNew(poly_base * base, int n);
int geoPoly2DInit(poly_base * base, TrGeo2DRef * ref);
void geoPoly2DDelete(poly_base * base);

poly_add * geoPolyAddInit(double * coor, int n, TrGeo2DRef * ref);
double geoPolyInitLen(double * coor, int n, poly_add * add, TrGeo2DRef * ref);

void geoGetSegmentByPoints(TrGeo2DRef * ref, poly_add * result,
	double x1, double y1, double x2, double y2);

int geoPolyGet2DCoor(poly_base * base, double offset, double * dest);

int geoPolyParPoint(TrGeo2DRef * ref, straight_line * pa, double org_x, double org_y,
		double * x, double * y, double width);

void geoPolyCrossPoint(TrGeo2DRef * ref, straight_line sl1, straight_line sl2, double * x, double * y);

void geoPolyParCrossPoint(TrGeo2DRef * ref, poly_add * pa1, poly_add * pa2, double * coor,
	double * x, double * y, double width1, double width2);

double geoPoly2DSection(double x1, double y1, double x2, double y2,
	double sec_x, double sec_y);

double geoPoly2DSegmentClosest(TrGeo2DRef * ref, poly_add * pa1, poly_add * perx,  double org_x, double org_y,
	double * x, double * y, double * rel_dist, int * idx_seg);
// TODO do remove this replace at trafalgar lib
//double geoPoly2DClosest(poly_base * base, TrGeo2DRef * ref,
//	double * x, double * y, double * rel_dist, int * idx_seg);

void geoPoly2DGetMaxExt(poly_base * base, double * ext, TrGeo2DRef * ref);

#endif // _geo_poly_h

