/******************************************************************
 *
 * @short	earth globe reference  
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_ref.h	defs	
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


#ifndef _geo_globe_h
#define _geo_globe_h

#include "geo_ref.h"

/*
 Aequatorumfang / Aequatorlaenge 	40.075.036 m 
 Nullter Laengengrad (über die Pole)	40.007.881 m 

 http://de.wikipedia.org/wiki/Erde
 Aequatorumfang				40.075,004 km
 Polumfang				39.940,638 km
 Mittelumfang				40.041,455 km

 http://www.xasa.com/wiki/de/wikipedia/e/er/erde.html
 Durchmesser am Aequator		12756,2 km 
 Durchmesser von Pol zu Pol		12713,6 km 
 Erdumfang am Aequator			40.076,592 km 
 Erdumfang über die Pole		40.009,153 km
*/

#define CIRCUMFERENCE_EQU 	40076592.00
#define CIRCUMFERENCE_POL 	40009153.00

#define COOR_FAC 		100000.0

typedef struct
{
	TrGeo2DCore core;
	double circ_equ;
	double circ_pol;
	double y_fac;
	double x_fac;
	double arc_fac;
}TrGeo2DGlobe;

void bra();

void TrgGlobeInit(TrGeo2DGlobe * ref);
void TrgGlobeSet(TrGeo2DGlobe * ref, double equ, double pol/*, double x_base*/);


#endif // _geo_globe_h

