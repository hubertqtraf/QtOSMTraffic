/******************************************************************
 *
 * @short	geometrical functions 
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_base.c	geometrical  functions	
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


#include <math.h>

#include "geo_base.h"

#include <stdio.h>

double geoGetLineByPoints(TrGeo2DRef * ref, straight_line * result, double x1, double y1,
	double x2, double y2)
{
	double dx = Geo2DX(ref, (x2 - x1), y1);
	double dy = Geo2DY(ref, x2, y2) - Geo2DY(ref, x1, y1);

	double len = sqrt((dx*dx)+(dy*dy));

	if((fabs(dx) < MIN_DIF) && (fabs(dy) < MIN_DIF))
	{
		//printf("base error %f %i\n", len, ref->core.error_code);
		ref->core.error_code = 1;
		result->flags = DIR_ERROR;
		return len;
	}

	result->flags = 0x00;
	if(fabs(dx) < MIN_DIF)
	{
		result->flags = DIR_VERT;
		result->slope = 1.0 / MIN_DIF;
		result->intercept = Geo2DX(ref, x1, y1); //x1;
		if(y1 > y2)
			result->flags |= DIR_RIGHT;
		return len;
	}
	if(x2 > x1)
	{
		result->flags |= DIR_RIGHT;
	}
    result->slope = dy / dx;
	result->intercept = Geo2DY(ref, x1, y1) - (result->slope * Geo2DX(ref, x1, y1));
	if(fabs(dy) < MIN_DIF)
		result->flags |= DIR_HORIZ;
	return len;
}


void geoInitLineL(straight_line * result, double intercept, double slope)
{
	result->intercept = intercept;
	result->slope = slope;
	result->flags = 0x00;
	
	if(slope > (1.0/MIN_DIF))
		result->flags = DIR_VERT;
	if(slope<MIN_DIF)
		result->flags = DIR_HORIZ;
}

void geoInitLineR(straight_line * result, double intercept, double slope)
{
	geoInitLineL(result,intercept,slope);
	result->flags|=DIR_RIGHT;
}

void geoInitVertLineU(straight_line * result, double x)
{
	result->intercept = x;
	result->slope = (1.0/MIN_DIF);
	result->flags = DIR_VERT;
}

void geoInitVertLineD(straight_line * result, double x)
{
	geoInitVertLineU(result,x);
	result->flags|=DIR_RIGHT;
}

double geoGetLineX(straight_line * line, double y)
{
	if(line->flags&DIR_HORIZ)
		return(0.0);
	return((y-line->intercept)/line->slope);
}

double geoGetLineY(straight_line * line, double x)
{
	return((line->slope*x)+line->intercept);
}

void geoGetParallel(TrGeo2DRef * ref, straight_line * orig, straight_line * dest, double dist)
{
	double a;
	double dy_intercept;

	dest->slope = orig->slope;
	dest->flags = orig->flags;

	if(orig->flags & DIR_HORIZ)
	{
		if(orig->flags & DIR_RIGHT)
			dest->intercept = orig->intercept - dist;
		else
			dest->intercept = orig->intercept + dist;
		return;
	}
	if(orig->flags & DIR_VERT)
	{
		if(orig->flags & DIR_RIGHT)
            dest->intercept = 0 - dist;
		else
			dest->intercept = dist;
		return;
	}
	a = dist * dest->slope;

	dy_intercept = sqrt((a*a)+(dist*dist));

	if(orig->flags & DIR_RIGHT)
		dy_intercept = 0.0 - dy_intercept;

	if(dist < 0.0)
		dest->intercept = orig->intercept - dy_intercept;
	else
		dest->intercept = orig->intercept + dy_intercept;
}


void geoGetPerpendicular(TrGeo2DRef * ref, straight_line * orig, straight_line * dest,
	double x, double y)
{
	dest->flags=0x00;

	if(orig->flags & DIR_VERT)
	{
		dest->flags = DIR_HORIZ;
		dest->slope = 0.0;
		dest->intercept = y;
		if(!(orig->flags & DIR_RIGHT))
			dest->flags |= DIR_RIGHT;
		return;
	}
	if(orig->flags & DIR_HORIZ)
	{
		dest->flags = DIR_VERT;
		dest->slope = (1.0/MIN_DIF);
		dest->intercept = x;
		if(orig->flags & DIR_RIGHT)
			dest->flags |= DIR_RIGHT;
		return;
	}
	dest->slope=(0.0-(1.0/orig->slope));
	dest->intercept=(y-(dest->slope*x));
	if(((orig->slope<0)&&(!(orig->flags&DIR_RIGHT)))||
		((orig->slope>0)&&(orig->flags&DIR_RIGHT)))
		dest->flags|=DIR_RIGHT;
}

int geoGetCrossPoint(TrGeo2DRef * ref, straight_line * a, straight_line * b,
	double * x, double * y)
{
	double d_slope = a->slope - b->slope;

	if(fabs(d_slope) < MIN_DIF)
		return(-1);
	if((d_slope == INFINITY) || (d_slope == NAN))
	{
		printf("error: slope is INFINITY or NAN: %f %f\n", a->slope, b->slope);
		ref->core.error_code = 2;
		return(-1);
	}

	*x = (b->intercept-a->intercept) / d_slope;

	if(a->flags & DIR_VERT)
	{
		*x = a->intercept;
		*y = ((b->slope*(*x)) + b->intercept);
		return(0);
	}
	if(b->flags & DIR_VERT)
	{
		*x = b->intercept;
		*y = ((a->slope*(*x))+a->intercept);
		return(0);
	}
	*x = (b->intercept-a->intercept)/d_slope;
	*y = ((a->slope*(*x))+a->intercept);
	return(0);
}

double geoGetAngle(straight_line * line)
{
	double angle;

	if(line->flags & DIR_VERT)		// vertical line
	{
		if(line->flags & DIR_RIGHT)	// vertival down
			return(M_PI_2+M_PI);
		return(M_PI_2);			// vertical up
	}

	angle = atan(line->slope);		// angle, first sector

	if(!(line->flags & DIR_RIGHT))		// sector two and three
		return(angle + M_PI);

	if(line->slope < 0.0)			// sector four
		return((2*M_PI) + angle);

	return(angle);				// default, first sector
}

double geoInvertAngle(double angle)
{
	angle += M_PI;

	if(angle > (M_PI * 2.0))
		angle -= (M_PI * 2.0);
	return angle;
}

double geoGetInterceptByPoint(straight_line * line, double x, double y)
{
	if(line->flags & DIR_VERT)
	{
		return x;
	}
	if(line->flags & DIR_HORIZ)
	{
		return y;
	}
	return(y - (line->slope * x));
}


