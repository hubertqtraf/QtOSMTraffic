/******************************************************************
 *
 * @short	definitions of class numbers of the XML profile file
 *
 * project:	Trafalgar/View
 *
 * modul:	tr_prof_class_def.h	header for XML profile
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2023-2025
 *
 * beginning:	04.2023
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

#ifndef TR_XML_PROF_H
#define TR_XML_PROF_H

// local flags to define landuse (getLanduseClass)
// definition: see XML file
#define LANDUSE_INDUSTRIAL  0x0001U
#define LANDUSE_RESIDENTIAL 0x0002U
#define LANDUSE_MIXED       0x0007U
#define LANDUSE_GRASS       0x0003U
#define LANDUSE_BUSHES      0x000aU
#define LANDUSE_GARDEN      0x0005U
#define LANDUSE_WOOD        0x0004U
#define LANDUSE_WATER       0x0006U
#define LANDUSE_UNKLOWN     0x0008U

#define BUILDING_HOUSE      0x0001U
#define BUILDING_APAR       0x0002U
#define BUILDING_INDUST     0x0004U
#define BUILDING_PUBLIC     0x0003U
#define BUILDING_LEARN      0x0003U
#define BUILDING_CAR        0x0003U
#define BUILDING_REL        0x0003U
#define BUILDING_SERVICE    0x0003U
#define BUILDING_MEDI       0x0003U
#define BUILDING_FARM       0x0005U
#define BUILDING_RUINS      0x0006U
#define BUILDING_POWER      0x0008U


// TODO: merge natural and landuse
//<value name="coast" rgb="#16183f" index="1" type="color"/>
#define NATURAL_COAST       0x0001U
//<value name="water" rgb="#7c99a3" index="2" type="color"/>
#define NATURAL_WATER       0x0002U
//<value name="wood" rgb="#5f7c62" index="3" type="color"/>
#define NATURAL_WOOD        0x0003U
//<value name="forest" rgb="#567d5d" index="4" type="color"/>
#define NATURAL_FOREST      0x0004U
//<value name="rock" rgb="#737373" index="5" type="color"/>
#define NATURAL_ROCK        0x0005U
//<value name="ice" rgb="#b3c8ff" index="6" type="color"/>
#define NATURAL_ICE         0x0006U
//<value name="wet" rgb="#2a4747" index="7" type="color"/>
#define NATURAL_WET         0x0007U
#define FIELD_PARKING       0x0008U
#define FIELD_SCHOOL        0x0009U
#define FIELD_CYCLE         0x000aU
#define FIELD_TRAFFIC       0x000bU
#define FIELD_AREA_ROAD     0x000cU

// <value rgb="#4a5051" type="color" index="0" name="default"/>
#define WATER_COAST         0x0001U
// <value rgb="#3c8990" type="color" index="1" name="coast"/>
#define WATER_LAKE          0x0002U
// <value rgb="#2e9c8f" type="color" index="2" name="lake"/>
#define WATER_RIVER         0x0003U
// <value rgb="#6262a6" type="color" index="3" name="river"/>
#define WATER_STREAM        0x0004U
// <value rgb="#5c82a6" type="color" index="4" name="stream"/>
#define WATER_CANAL         0x0005U
// <value rgb="#91a6a3" type="color" index="5" name="canal"/>

/*
// use  only 'act_world->node_flags'!
#define TYPE_PT_NAME        0x0000000000001000
#define TYPE_PT_NAME_EN     0x0000000000002000*/

// used 'act_world->way_flags' or 'act_world->node_flags'
#define TYPE_ROAD           0x0000000000010000U
#define TYPE_RAIL           0x0000000000020000U
#define TYPE_STREAM         0x0000000000040000U
// https://wiki.openstreetmap.org/wiki/DE:Key:aerialway
// aerialway -> drag_lift/cable_car/gondola etc. / goods
#define TYPE_LIFT           0x0000000000080000U
#define TYPE_BUILDING       0x0000000000100000U
#define TYPE_LANDUSE        0x0000000000200000U
#define TYPE_NATURAL        0x0000000000400000U
// was 'amenity': replace 'place_of_worship'/'shop'/'cafe' etc.
#define TYPE_PUBLIC         0x0000000000800000U
// 'leisure -> public'
//#define TYPE_LEISURE      0x0000000001000000U
// gate etc.
#define TYPE_RESTRICT       0x0000000001000000U
#define TYPE_ADMIN          0x0000000002000000U

#define TYPE_POWER          0x0000000004000000U

// use  only 'act_world->node_flags'!
#define TYPE_PT_NAME        0x0000000000001000U
#define TYPE_PT_NAME_EN     0x0000000000002000U
#define TYPE_PT_ALT         0x0000000000004000U

// natural
#define TYPE_POI_N_PEAK     0x0000000100000000U
#define TYPE_POI_N_TREE     0x0000000200000000U
#define TYPE_POI_N_CAVE     0x0000000400000000U

// public
#define TYPE_POI_P_PARKING  0x0000001000000000U
#define TYPE_POI_P_CHURCH   0x0000002000000000U
// anything to eat or drink
#define TYPE_POI_P_FOOD     0x0000004000000000U
// ATM or bank
#define TYPE_POI_P_MONEY    0x0000008000000000U
// doctor or phrarm.
#define TYPE_POI_P_MED      0x0000010000000000U
#define TYPE_POI_P_SHOP     0x0000020000000000U
#define TYPE_POI_P_SPORT    0x0000040000000000U
// hotel, app. etc.
#define TYPE_POI_P_GUEST    0x0000080000000000U
#define TYPE_POI_P_FUEL     0x0000100000000000U
// info, souvenir, etc
#define TYPE_POI_P_TOUR     0x0000200000000000U
// bench, viewpoint, etc
#define TYPE_POI_P_NICE     0x0000400000000000U

#define TYPE_POI_P_ALPINE   0x0000800000000000U

// TODO: TMP!
#define TYPE_POI_A_VILL     0x0001000000000000U


#endif	// TR_XML_PROF_H

