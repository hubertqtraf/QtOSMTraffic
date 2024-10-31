/******************************************************************
 *
 * @short	raw reading of relations
 *
 * project:	Trafalgar/View
 *
 * modul:	osm_load_rel.h	reader for relation XML read
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2019-2023
 *
 * beginning:	05.2023
 *
 * history:
 */
/******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software foundation; either version 2, or (at your
   option) any later version.

   The GNU trafalgar package is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with the trafalgar package; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
   Boston, MA 02110-1301, USA. */

#ifndef OSM_LOAD_REL_H
#define OSM_LOAD_REL_H

#define REL_TAG         0x01
#define REL_MEMBER      0x02
#define REL_ID          0x04
#define REL_K           0x08
#define REL_V           0x10
#define REL_ROLE        0x20
#define REL_REF         0x40
#define REL_M_TYPE      0x80

// member_flags
#define REL_MEMBER_NODE 0x01
#define REL_MEMBER_WAY  0x02
#define REL_MEMBER_REL  0x04

#define REL_MEM_ROLE_IN    0x10
#define REL_MEM_ROLE_OUT   0x20
#define REL_MEM_ROLE_ROUTE 0x40

// relation type flags
#define FLAG_MULTI_POLY 0x0100
#define FLAG_ROUTE      0x0200

// TODO: to osm_load.h?
#define REL_TYPE_CLASS        0x0000000000008000
// check wether base groups (landuse, nature...) are set
#define BASE_GROUP_CHECK      0x0000000000004000

#ifdef OSM_C_FILTER
int count_outer(Rel_t * relation);
#endif

#endif // OSM_LOAD_REL_H
