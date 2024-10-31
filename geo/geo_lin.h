/******************************************************************
 *
 * @short	linear correction   
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_ref.h	defs	
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


#ifndef _geo_lin_h
#define _geo_lin_h

#include "geo_ref.h"

typedef struct
{
	TrGeo2DCore core;
	double x_fac;
	double y_fac;
}TrGeo2DLin;

void TrgLinInit(TrGeo2DLin * ref, double x_fac, double y_fac);
void TrgLinSet(TrGeo2DLin * ref, double x_fac, double y_fac);

#endif // _geo_lin_h

