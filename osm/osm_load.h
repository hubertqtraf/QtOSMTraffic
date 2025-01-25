/******************************************************************
 *
 * @short	raw reading of nodes and ways
 *
 * project:	Trafalgar/View
 *
 * modul:	osm_load.h	header for XML read
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2019-2023
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

#ifndef OSM_LOAD_H
#define OSM_LOAD_H

#ifdef OSM_C_FILTER
#include "zblock_io.h"
#include "osm_info.h"
#endif

#include "osm_types.h"

#include <stdint.h> 

#define INSIDE_NODE 0x01
#define INSIDE_WAY  0x02
#define INSIDE_SEG  0x04
#define INSIDE_REL  0x08

#define ND_ID		0x10
#define ND_COOR		0x20
#define ND_SEG		0x40
#define ND_LON		0x80
#define ND_K		0x08
#define ND_V		0x04

// TODO: move/remove?
#define ND_F_GATE	0x0000000000000002U
#define ND_F_ACCESS	0x0000000000000004U

#define WAY_ID   	0x04
#define WAY_NODE	0x01
#define WAY_NODE_REF	0x02
#define WAY_K		0x08
#define WAY_V		0x10

#define ID_MASK		0x0fffffffffffffffU

//#define SECTIONS 36

// TODO: change the flags -> 'k' != way flags
// used at 'way_is_inside_class_def'
#define FLAG_W_CLASS        0x0000000000000001U
#define FLAG_ONEWAY         0x0000000000000100U
#define FLAG_WIDTH          0x0000000000000200U
#define FLAG_SIDEWALK       0x0000000000000400U
#define FLAG_CYCLEWAY       0x0000000000000800U
#define FLAG_LANES          0x0000000000001000U
#define FLAG_LANES_F        0x0000000000002000U
#define FLAG_LANES_B        0x0000000000004000U
#define FLAG_TURN           0x0000000000008000U
#define FLAG_K_DISUSED      0x0000000000010000U
#define FLAG_K_PLANNED      0x0000000000020000U

#define FLAG_K_NAME         0x0000000000040000U

#define FLAG_TOURISM        0x0000000000080000U

// TODO: used at world->way_parking, define the parking in trafalgar
// TODO: 32bit on the each side...
// 0x0000 0001 exist
// 0x0000 1110 no, diagonal, parallel, etc.
// 0x0001 0000 restricted
// 0x0010 0000 fee
//
#define FLAG_PARKING        0x0000000000100000U
#define FLAG_PARKING_R      0x0000000000200000U
#define FLAG_PARKING_L      0x0000000000400000U
#define FLAG_PARKING_B      0x0000000000800000U

#define V_PARK_PARALLEL_L   0x0000000100000000U
#define V_PARK_DIAGONAL_L   0x0000000200000000U
#define V_PARK_PERPENDI_L   0x0000000400000000U

#define V_PARK_NO_L         0x0000001000000000U
#define V_PARK_NO_STOP_L    0x0000002000000000U

#define V_PARK_PARALLEL_R   0x0000010000000000U
#define V_PARK_DIAGONAL_R   0x0000020000000000U
#define V_PARK_PERPENDI_R   0x0000040000000000U

#define V_PARK_NO_R         0x0000100000000000U
#define V_PARK_NO_STOP_R    0x0000200000000000U
// end of parking

//#define FLAG_AREA         0x00000400

//#define FLAG_DOUBLE_USE 0x00000200

// use  only 'act_world->node_flags'!
#define TYPE_PT_NAME        0x0000000000001000U
#define TYPE_PT_NAME_EN     0x0000000000002000U

// second class OSM attibute, together with 'highway'
#define TYPE_CROSS          0x0000000008000000U

#define TYPE_FILTER         TYPE_ROAD|TYPE_RAIL|TYPE_STREAM|TYPE_LIFT|TYPE_BUILDING|TYPE_LANDUSE|TYPE_NATURAL|TYPE_PUBLIC|TYPE_RESTRICT|TYPE_ADMIN|TYPE_POWER

#define HAS_TYPE_KEY        0x0100000000000000U

// planned
#define FLAG_PLANNED        0x2000000000000000U
// construction
#define FLAG_CONSTRUCTION   0x4000000000000000U
// disused abandoned
#define FLAG_DISUSED        0x8000000000000000U

#define FLAG_RAMP           0x0000010000000000U

#define FLAG_FEATURE_WAY    0x0000001000000000U
#define FLAG_FEATURE_NODE   0x0000002000000000U
#define FLAG_FEATURE_AERA   0x0000004000000000U
#define FLAG_FEATURE_REL    0x0000008000000000U

// p/c/a                    0x0000000000002000
// oneway                   0x0000000200000000
//                          0000004000200003
//                          0000000000010009

// TODO: check - world_->way_is_inside_class_def, world_->way_lanes
#define TURN_THROUGH        0x0000000000000001U
#define TURN_LEFT           0x0000000000000002U
#define TURN_RIGHT          0x0000000000000004U
#define TURN_RES            0x0000000000000008U

// defines for POI

// use  only 'act_world->node_osm_flags'!
#define TYPE_PT_NAME        0x0000000000001000U
#define TYPE_PT_NAME_EN     0x0000000000002000U
#define TYPE_PT_ALT         0x0000000000004000U


// TODO: c version?
#include <QMap>
#ifdef TESTX
#include <tr_map_list.h>
#endif

struct name_set
{
	uint64_t id;
	uint32_t number;
};

typedef struct
{
	uint64_t osm_base_class;

	uint8_t type_flags;
	uint8_t key_flags;
	uint8_t val_flags;
	uint8_t seg_flags;

#ifdef OSM_C_FILTER
	OsmInfo_t info;
#else
	size_t way_count;
	size_t node_count;
#endif

	Point_t * points;

	// use point struct for nodes
	Point_t * nodes;
	Way_t * ways;

	Rel_t * relations;
	uint64_t rel_tags; 
	uint8_t member_flags;
	uint64_t act_way_id;

	uint64_t act_idx;
	uint64_t member_count;

	// osm 'v' flags
	uint64_t node_osm_flags;
	// trafalgar node flags
	uint64_t node_tr_flags;
	//uint32_t way_flags;
	uint64_t way_flags;
	uint64_t way_lanes;
	// 0-3 -> type
	// 4   -> oneway flag
	// 5   -> lanes flag
	uint64_t way_parking;
	//uint64_t way_sidewalk;

	// way width in [mm]
	uint32_t way_width;

	//uint8_t lanes_dir;

	uint32_t way_is_inside_class_def;

	// TODO: collect data of way: max number of ids in node
	// TODO: replace '10000' with collected data or realloc
	uint64_t tmp_id_buf[10000];

	RelMember_t * rel_id_buf;
	char * act_name;
	uint64_t act_name_idx;
	// Map on string key and integer as value
	QMap<QString, name_set> m_name_map;
	// Map on node id and name
	QMap<uint64_t, QString> m_point_name_map;

	// values of points
	uint32_t altitude;
}World_t;

#ifdef OSM_C_FILTER
int readPoints(z_block * z_read, World_t * act_world);
int readNodes(z_block * z_read, World_t * act_world);
int readWays(z_block * z_read, World_t * act_world);
int readRelations(z_block * z_read, World_t * act_world);
#endif

#endif	// OSM_LOAD_H

