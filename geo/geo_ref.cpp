/******************************************************************
 *
 * @short	geometrical functions  
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_ref.c	geometrical  function defs	
 * @version:	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2005-2012
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


#include <stdlib.h>
#include <math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

//#pragma GCC diagnostic push
// TODO: -Wzero-as-null-pointer-constant
#include "geo_ref.h"
//#if __cplusplus
//#define NULL nullptr
//#endif

double TrgXDefault(void * ref, double x, double y);
double TrgYDefault(void * ref, double x, double y);
double TrgXOrigDefault(void * ref, double x, double y);
double TrgYOrigDefault(void * ref, double x, double y);
double TrgLenDefault(void * ref, double x1, double y1, double x2, double y2);
double TrgLocalDefault(void * ref, double x, double y);

void GeoIoRefInit(TrGeo2DRef * ref)
{
    ref->core.trg_x=NULL;
    ref->core.trg_y=NULL;
	ref->core.trg_orig_x=NULL;
	ref->core.trg_orig_y=NULL;
	ref->core.trg_len=NULL;
	ref->core.trg_local=NULL;

	ref->core.error_code = 0;
}

void GeoIoRefInitDefault(TrGeo2DRef * ref)
{
	ref->core.trg_x=TrgXDefault;
	ref->core.trg_y=TrgYDefault;
	ref->core.trg_orig_x=TrgXOrigDefault;
	ref->core.trg_orig_y=TrgYOrigDefault;
	ref->core.trg_len=TrgLenDefault;
	ref->core.trg_local=TrgLocalDefault;

	ref->core.error_code = 0;
}

double TrgXDefault(void * ref, double x, double y)
{
	return(x);
}

double TrgYDefault(void * ref, double x, double y)
{
	return(y);
}

double TrgXOrigDefault(void * ref, double x, double y)
{
	return(x);
}

double TrgYOrigDefault(void * ref, double x, double y)
{
	return(y);
}

double TrgLenDefault(void * ref, double x1, double y1, double x2, double y2)
{
	double dx=x2-x1;
	double dy=y2-y1;

	return(sqrt((dx*dx)+(dy*dy)));
}

double TrgLocalDefault(void * ref, double x, double y)
{
	return(1.0);
}

double Geo2DX(TrGeo2DRef * ref, double x, double y)
{
	if(ref->core.trg_x!=NULL)
		return(ref->core.trg_x(ref,x,y));
	return(0.0);
}

double Geo2DY(TrGeo2DRef * ref, double x, double y)
{
	if(ref->core.trg_y!=NULL)
		return(ref->core.trg_y(ref,x,y));
	return(0.0);
}

double Geo2DOrigX(TrGeo2DRef * ref, double x, double y)
{
	if(ref->core.trg_orig_x!=NULL)
		return(ref->core.trg_orig_x(ref,x,y));
	return(0.0);
}

double Geo2DOrigY(TrGeo2DRef * ref, double x, double y)
{
	if(ref->core.trg_orig_y!=NULL)
		return(ref->core.trg_orig_y(ref,x,y));
	return(0.0);
}

double Geo2DLen(TrGeo2DRef * ref, double x1, double y1, double x2, double y2)
{
	if(ref->core.trg_len!=NULL)
		return(ref->core.trg_len(ref,x1,y1,x2,y2));
	return(1000000.0);
}

double Geo2DLocal(TrGeo2DRef * ref, double x, double y)
{
	if(ref->core.trg_local!=NULL)
		return(ref->core.trg_local(ref,x,y));
	return(1.0);
}

#pragma GCC diagnostic pop
