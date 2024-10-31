/******************************************************************
 *
 * @short	defines of realtive and absolute points
 *
 * project:	Trafalgar lib
 *
 * class:	---
 * superclass:	---
 * modul:	tr_point.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2008
 *
 * @author	Schmid Hubert (C)2008-2023
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

#ifndef TR_POINT
#define TR_POINT

#include <QPoint>
#include <stdint.h>

#define TR_POINT_IS_REF		0x0001
#define TR_POINT_REF_DIR        0x0002

#define TR_COOR_FACTOR          100000.0

// create longitude/latitude pair as string with factor
#define TR_COOR(v) (QString("%1 %2").arg(v.x/TR_COOR_FACTOR, 0, 'g', 10).arg(v.y/TR_COOR_FACTOR, 0, 'g', 10))
// create longitude or atitude as string with factor
#define TR_COOR_VAL(v) (QString("%1").arg(v/TR_COOR_FACTOR, 0, 'g', 10))

#define TR_COOR_REF(v) (QString("%1 %2 %3 %4 %5").arg(v->dist).arg(v->id).arg(v->intercept).arg(v->type).arg(v->no))

#define TR_POINT_V(v) (static_cast <double>(v))

typedef struct
{
	double x;	// longitude
	double y;	// latitude
}TrPoint;

typedef struct
{
	int32_t x;	// longitude
	int32_t y;	// latitude
}TrPoint32;

typedef struct
{
	double dist;
	uint64_t  id;
	int32_t intercept;
	uint16_t  type;
	uint16_t  no;
}TrPointRef;


#endif //TR_POINT

