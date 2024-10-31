/******************************************************************
 *
 * @short	linear correction  
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_globe.c	defs	
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


//#include <stdio.h>	// tmp

#include <math.h>

#include "geo_lin.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

double TrgXLin(void * ref, double x, double y);
double TrgYLin(void * ref, double x, double y);
double TrgXOrigLin(void * ref, double x, double y);
double TrgYOrigLin(void * ref, double x, double y);
double TrgLenLin(void * ref, double x1, double y1, double x2, double y2);

void TrgLinInit(TrGeo2DLin * ref, double x_fac, double y_fac)
{
	ref->core.trg_x=TrgXLin;
	ref->core.trg_y=TrgYLin;
	ref->core.trg_orig_x=TrgXOrigLin;
	ref->core.trg_orig_y=TrgYOrigLin;
	ref->core.trg_len=TrgLenLin;

	ref->x_fac=x_fac;
	ref->y_fac=y_fac;
}

void TrgLinSet(TrGeo2DLin * ref, double x_fac, double y_fac)
{
	ref->x_fac=x_fac;
	ref->y_fac=y_fac;
}

double TrgXLin(void * ref, double x, double y)
{
	TrGeo2DLin * g_ref=(TrGeo2DLin *)ref;

	return(x*g_ref->x_fac);
}

double TrgYLin(void * ref, double x, double y)
{
	TrGeo2DLin * g_ref=(TrGeo2DLin *)ref;

	return(y*g_ref->y_fac);
}

double TrgXOrigLin(void * ref, double x, double y)
{
	TrGeo2DLin * g_ref=(TrGeo2DLin *)ref;

	return(x/g_ref->x_fac);
}

double TrgYOrigLin(void * ref, double x, double y)
{
	TrGeo2DLin * g_ref=(TrGeo2DLin *)ref;

	return(y/g_ref->y_fac);
}

double TrgLenLin(void * ref, double x1, double y1, double x2, double y2)
{
	double dx;
	double dy;

	TrGeo2DLin * g_ref=(TrGeo2DLin *)ref;

	dx=(x2-x1)*g_ref->x_fac;
	dy=(y2-y1)*g_ref->y_fac;

	return(sqrt((dx*dx)+(dy*dy)));
}

#pragma GCC diagnostic pop
