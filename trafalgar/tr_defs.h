/******************************************************************
 *
 * @short	base definitions of the trafagar lib
 *
 * project:	Trafalgar lib
 *
 * class:	---
 * superclass:	---
 * modul:	tr_defs.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2020
 *
 * @author	Schmid Hubert (C)2020-2021
 *
 * history:
 *
 ******************************************************************/

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


#ifndef TR_DEFS_H
#define TR_DEFS_H

#include <stdint.h>

#include <QtCore/qdebug.h>
#include <QtCore/qloggingcategory.h>

#define TR_MASK_SELECT_LINK    0x0000000000000100U
#define TR_MASK_SELECT_POLY    0x0000000000000200U
// TODO: TR_MASK_SELECT_POINT -> TR_MASK_SELECT_NODE
#define TR_MASK_SELECT_POINT   0x0000000000000400U
#define TR_MASK_SELECT_COMPLEX 0x0000000000000800U
#define TR_MASK_SELECT_POI     0x0000000000001000U
#define TR_MASK_SELECT_ALL     (TR_MASK_SELECT_LINK | TR_MASK_SELECT_POLY | TR_MASK_SELECT_POINT | TR_MASK_SELECT_COMPLEX | TR_MASK_SELECT_POI)

#define TR_MASK_CLASS_SHIFT    16
#define TR_MASK_CLASS_LEVEL    0x00000000007F0000U
#define TR_MASK_CLASS_FILTER   0x0000000000800000U
#define TR_MASK_SHOW_POINTS    0x0000000010000000U
#define TR_MASK_POINTS_NUM     0x0000000020000000U
#define TR_MASK_MORE_LINES     0x0000000100000000U
#define TR_MASK_SET_RAMPS      0x0000000200000000U
#define TR_MASK_SHOW_PARKING   0x0000000400000000U
#define TR_MASK_SHOW_CONSTRUCT 0x0000000800000000U
#define TR_MASK_MOVE_LINE      0x0000001000000000U
#define TR_MASK_SHOW_ROADNAME  0x0000002000000000U

// log defines
// how to redirect qDebug ... stackoverflow ... done :-)
#define TR_MSG qDebug()    << __func__ << "| " << __FILE__<< ", " << __LINE__ << "| "
#define TR_INF qInfo()     << __func__ << __FILE__ << __LINE__ << ", "
#define TR_WRN qWarning()  << " "
#define TR_ERR qCritical() << " "
// TODO: check qFatal()...
#define TR_FTL qFatal()    << __func__ << __FILE__ << __LINE__ << ", "

Q_DECLARE_LOGGING_CATEGORY(trafalgar)

#endif // TR_DEFS_H

