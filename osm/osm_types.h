/******************************************************************
 *
 * @short	types for raw reading
 *
 * project:	Trafalgar/View
 *
 * modul:	osm_types.h	header for XML read
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2019-2021
 *
 * beginning:	02.2019
 *
 * history:
 */
/******************************************************************/

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

#ifndef TR_TYPES_OSM_H
#define TR_TYPES_OSM_H 

#include <stdint.h>

typedef struct
{
	// Node_t part
	uint64_t id;
	int32_t x;
	int32_t y;

	// Point_t part
	uint64_t pt_type;
	uint64_t pt_data;
}Point_t;

typedef struct
{
	uint64_t id;
	// TODO: combine id and name_id?
	uint64_t name_id;
	uint64_t * nd_id;
	int n_nd_id;
	uint64_t type;
	// uint16_t type;
	// 1 road, 2 rail, ...
	// uint16_t data;
	// data: class 0x001f , dir << 9 (0-2) 
	uint32_t lanes;
	// lanes: lanes 7, lanesF << 16 lanesB << 24
	uint32_t parking;
	// width in [mm]
	uint32_t width;
}Way_t;

typedef struct
{
	uint64_t id;
	uint16_t flags;
}RelMember_t;

typedef struct
{
	uint64_t id;
	uint64_t flags;
	uint32_t r_count;
	RelMember_t * members;
}Rel_t;

#endif // TR_TYPES_OSM_H
