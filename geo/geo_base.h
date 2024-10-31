/******************************************************************
 *
 * @short	geometrical functions 
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_base.h	defines
 * @version:	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2005-2014
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


#ifndef _geo_base_h
#define _geo_base_h

#define MIN_DIF 0.0000001

#define DIR_VERT  0x02
#define DIR_HORIZ 0x01
#define DIR_RIGHT 0x04
#define DIR_ERROR 0x10
#define DIR_INIT  0x20

#include "geo_ref.h"

typedef struct
{
	unsigned char flags;
	double intercept;		// y-axis intercept
	double slope; 			// raising of straight line
}straight_line;

double geoGetLineByPoints(TrGeo2DRef * ref, straight_line * result,
		double x1, double y1, double x2, double y2);

void geoInitLineL(straight_line * result, double intercept, double slope);
void geoInitLineR(straight_line * result, double intercept, double slope);

void geoInitVertLineD(straight_line * result, double x);
void geoInitVertLineU(straight_line * result, double x);

double geoGetLineX(straight_line * line, double y);
double geoGetLineY(straight_line * line, double x);

void geoGetParallel(TrGeo2DRef * ref, straight_line * orig, straight_line * dest, double dist);

void geoGetPerpendicular(TrGeo2DRef * ref, straight_line * orig, straight_line * dest,
	double x, double y);

int geoGetCrossPoint(TrGeo2DRef * ref, straight_line * a, straight_line * b,
	double * x, double * y);

double geoGetAngle(straight_line * line);

double geoInvertAngle(double angle);

double geoGetInterceptByPoint(straight_line * line, double x, double y);

#endif /* _geo_base_h */

