/******************************************************************
 *
 * @short	earth globe reference  
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_globe.c	defs	
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


#include <stdio.h>	// tmp

#include <math.h>

#include "geo_globe.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

double TrgXGlobe(void * ref, double x, double y);
double TrgYGlobe(void * ref, double x, double y);
double TrgXOrigGlobe(void * ref, double x, double y);
double TrgYOrigGlobe(void * ref, double x, double y);
double TrgLenGlobe(void * ref, double x1, double y1, double x2, double y2);
double TrgLocalGlobe(void * ref, double x, double y);

void TrgGlobeInit(TrGeo2DGlobe * ref/*, double x_base*/)
{
	ref->core.trg_x = TrgXGlobe;
	ref->core.trg_y = TrgYGlobe;
	ref->core.trg_orig_x = TrgXOrigGlobe;
	ref->core.trg_orig_y = TrgYOrigGlobe;
	ref->core.trg_len = TrgLenGlobe;
	ref->core.trg_local = TrgLocalGlobe;

	ref->arc_fac = M_PI_2/(COOR_FAC*90.0);
	//printf("TrgGlobeInit: %f %f %f\n",COOR_FAC,M_PI_2,ref->arc_fac);

    TrgGlobeSet(ref, CIRCUMFERENCE_EQU, CIRCUMFERENCE_POL/*,x_base*/);

	ref->core.error_code = 0;
}

void TrgGlobeSet(TrGeo2DGlobe * ref, double equ, double pol/*, double x_base*/)
{
	ref->circ_equ = equ;
	ref->circ_pol = pol;
	ref->y_fac = ref->circ_pol/(360.0 * COOR_FAC);
	ref->x_fac = ref->circ_equ/(360.0 * COOR_FAC);
	//ref->x_base=x_base*COOR_FAC;
}

double TrgXGlobe(void * ref, double x, double y)
{
	TrGeo2DGlobe * g_ref = (TrGeo2DGlobe *)ref;

	double f = y * g_ref->arc_fac;
	f = cos(f);
	return(x * g_ref->x_fac * f);
}

double TrgYGlobe(void * ref, double x, double y)
{
	TrGeo2DGlobe * g_ref = (TrGeo2DGlobe *)ref;

	return(y * g_ref->y_fac);
}

double TrgXOrigGlobe(void * ref, double x, double y)
{
	TrGeo2DGlobe * g_ref = (TrGeo2DGlobe *)ref;

	double g = y / g_ref->y_fac;
	g *= g_ref->arc_fac;
	g = cos(g);
	return (x/(g_ref->x_fac * g));
}

double TrgYOrigGlobe(void * ref, double x, double y)
{
	TrGeo2DGlobe * g_ref = (TrGeo2DGlobe *)ref;

	return(y/g_ref->y_fac);
}

double TrgLenGlobe(void * ref, double x1, double y1, double x2, double y2)
{
	double dx = TrgXGlobe(ref, x2 - x1, y1);
	double dy = (TrgYGlobe(ref, x1, y1) - TrgYGlobe(ref, x2, y2));

	return(sqrt((dx*dx)+(dy*dy)));
}

double TrgLocalGlobe(void * ref, double x, double y)
{
	TrGeo2DGlobe * g_ref=(TrGeo2DGlobe *)ref;

    return(cos(y*g_ref->arc_fac));
}

#pragma GCC diagnostic pop
