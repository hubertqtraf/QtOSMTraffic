/******************************************************************
 *
 * @short	geometrical functions, polygon handling 
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_poly.c	geometrical  functions	
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


#include <stdio.h> // tmp

#include <string.h>

#include <math.h>

#include "geo_poly.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

int geoPoly2DNew(poly_base * base, int n)
{
	int i;

	base->n_pt=n;
	base->length=0.0;
	base->add=NULL;

	if(n==0)
	{
		base->pt=NULL;
		return(0);
	}

	if((base->pt=(double*)malloc(sizeof(double)*2*n))==NULL)
		return(-1);

	for(i=0;i<(2*n);i++)
		base->pt[i]=0.0;

	return(0);
}

void geoPoly2DDelete(poly_base * base)
{
	if(base->pt != NULL)
	{
		free(base->pt);
		base->pt = NULL;
	}
	if(base->add != NULL)
	{
		free(base->add);
		base->add = NULL;
	}
}

int geoPoly2DInit(poly_base * base, TrGeo2DRef * ref)
{
	if((base->add=geoPolyAddInit(base->pt,base->n_pt,ref))==NULL)
		return(-1);

	base->length = geoPolyInitLen(base->pt,base->n_pt,base->add,ref);

	return(0);
}


void geoGetSegmentByPoints(TrGeo2DRef * ref, poly_add * result,
	double x1, double y1, double x2, double y2)
{
	result->len_part = geoGetLineByPoints(ref, &(result->sl), x1, y1, x2, y2);
}

double geoPoly2DSection(double x1, double y1, double x2, double y2,
		double sec_x, double sec_y)
{
	if(x2 > x1)
	{
		if((sec_x > x1) && (sec_x < x2))
		{
			return 1.0;
		}
	}
	else
	{
		if((sec_x > x2) && (sec_x < x1))
		{
			return 1.0;
		}
	}
	if(y2 > y1)
	{

	}
	return (-1);
}

poly_add * geoPolyAddInit(double * coor, int n, TrGeo2DRef * ref)
{
	int i;
	poly_add * ret;
	double x1;
	double y1;
	double x2;
	double y2;

	if(n<2)
		return(NULL);

	if((ret=(poly_add *)malloc(sizeof(poly_add)*(n-1)))==NULL)
		return(NULL);
	memset(ret, 0x00, sizeof(poly_add)*(n-1));

	x1 = coor[0];
	y1 = coor[1];

	for(i=0; i<(n-1); i++)
	{
		x2 = coor[(i*2)+2];
		y2 = coor[(i*2)+3];

		ret[i].sl.intercept = 0.0;
		geoGetSegmentByPoints(ref, ret+i, x1, y1, x2, y2);

		x1 = x2;
		y1 = y2;
	}
	return(ret);
}

double geoPolyInitLen(double * coor, int n, poly_add * add, TrGeo2DRef * ref)
{
	int i;
	double abs_len = 0.0;
	double x1,y1,x2,y2 = 0.0;

	if(n<2)
		return(0.0);

	x1 = coor[0];
	y1 = coor[1];

	for(i=1; i<n; i++)
	{
		x2 = coor[i*2];
		y2 = coor[(i*2)+1];

		add[i-1].len_part = Geo2DLen(ref, x1, y1, x2, y2);

		abs_len += add[i-1].len_part;

		x1 = x2;
		y1 = y2;
	}

	return(abs_len);
}

int geoPolyGet2DCoor(poly_base * base, double offset, double * dest)
{
	unsigned int i;

	double d1;
	double d2 = 0.0;
	double dist = 0.0;

	for(i=0;i<(base->n_pt-1);i++)
	{
		d1=dist;
		if(i==0)
			d1=0.0-MIN_DIF;
		dist+=base->add[i].len_part;
		if(i==(base->n_pt-2))
			d2=dist+MIN_DIF;

		if((offset>d1)&&(offset<=d2))
		{
			dist-=offset;
			d1=(base->pt[i*2]-base->pt[(i+1)*2])*dist;
			d2=(base->pt[(i*2)+1]-base->pt[((i+1)*2)+1])*dist;

			dest[0]=base->pt[(i+1)*2]+d1;
			dest[1]=base->pt[((i+1)*2)+1]+d2;

			return(1);
		}
	}
	return(0);
}

int geoPolyParPoint(TrGeo2DRef * ref, straight_line * pa, double org_x, double org_y,
		double * x, double * y, double width)
{
	double xa = Geo2DX(ref, 0, org_y);
	double ya = Geo2DY(ref, 0, org_y);
	ref->core.error_code = 0;

	straight_line mv_pa = *pa;
	mv_pa.intercept = ya;

	straight_line per;
	straight_line par;

	geoGetPerpendicular(ref, &mv_pa, &per, xa, ya);
	geoGetParallel(ref, &mv_pa, &par, width);
	geoGetCrossPoint(ref, &par, &per, &xa, &ya);

	*x = Geo2DOrigX(ref, xa, ya) + org_x;
	*y = Geo2DOrigY(ref, xa, ya);

	return ref->core.error_code;
}

void geoPolyCrossPoint(TrGeo2DRef * ref, straight_line sl1, straight_line sl2, double * x, double * y)
{
	// TODO: add x shift and VERT flag!
	geoGetCrossPoint(ref, &(sl1), &(sl2), x, y);
	*x = Geo2DOrigX(ref, *x, *y);
	*y = Geo2DOrigY(ref, *x, *y);
}

void geoPolyParCrossPoint(TrGeo2DRef * ref, poly_add * pa1, poly_add * pa2, double * coor,
	double * x, double * y, double width1, double width2)
{
	double base_x = coor[0];

	straight_line par1;
	straight_line par2;

	straight_line sl1 = pa1->sl;
	straight_line sl2 = pa2->sl;

	// set the intercept but ignore this if there is a verical line
	double i = Geo2DY(ref, coor[0], coor[1]);
	if(!(sl1.flags & DIR_VERT))
		sl1.intercept = i;
	if(!(sl2.flags & DIR_VERT))
		sl2.intercept = i;

	geoGetParallel(ref, &sl1, &par1, width1);
	geoGetParallel(ref, &sl2, &par2, width2);

	if(geoGetCrossPoint(ref, &par1, &par2, x, y) == (-1))
	{
		// 'geoPolyParPoint' includes the calculation of the original point
		geoPolyParPoint(ref, &(pa1->sl), coor[0], coor[1], x, y, width1);
		return;
	}
	*x = Geo2DOrigX(ref, *x, *y);
	*y = Geo2DOrigY(ref, *x, *y);
	*x = base_x + *x;
}

double geoPoly2DSegmentClosest(TrGeo2DRef * ref, poly_add * pa1, poly_add * perx,  double org_x, double org_y,
	double * x, double * y, double * rel_dist, int * idx_seg)
{
	straight_line mv_pa = pa1->sl;
	double dx = (*x) - org_x;
	mv_pa.intercept = Geo2DY(ref, 0, org_y);
	double pt_x = Geo2DX(ref, dx, *y);
	double pt_y = Geo2DY(ref, dx, *y);
	dx = pt_x;
	double dy = pt_y;

	if(mv_pa.flags & DIR_VERT)
	{
		*x = org_x;
		return dx;
	}

	straight_line per;
	geoGetPerpendicular(ref, &mv_pa, &per, pt_x, pt_y);
	geoGetCrossPoint(ref, &mv_pa, &per, &pt_x, &pt_y);

	dx -= pt_x;
	dy -= pt_y;

	*x = Geo2DOrigX(ref, pt_x, pt_y) + org_x;
	*y = Geo2DOrigY(ref, pt_x, pt_y);

	return(sqrt((dx*dx)+(dy*dy)));
}

void geoPoly2DGetMaxExt(poly_base * base, double * ext, TrGeo2DRef * ref)
{
        unsigned int i;
	double x;
	double y;

	if(base->n_pt<2)
	{
		ext[0]=ext[2]=ext[1]=ext[3]=0.0;
		return;
	}

	ext[0]=ext[2]=Geo2DX(ref,base->pt[0],base->pt[1]);
	ext[1]=ext[3]=Geo2DY(ref,base->pt[0],base->pt[1]);

	for(i=1;i<base->n_pt;i++)
        {
		x=Geo2DX(ref,base->pt[i*2],base->pt[(i*2)+1]);
		y=Geo2DY(ref,base->pt[i*2],base->pt[(i*2)+1]);

		if(x<ext[0])				/* check left upper */
			ext[0]=x;
		if(y<ext[1])				/* check left upper */
			ext[1]=y;
		if(x>ext[2])				/* check right lower */
			ext[2]=x;
		if(y>ext[3])				/* check right lower */
			ext[3]=y;
        }
}

#pragma GCC diagnostic pop
