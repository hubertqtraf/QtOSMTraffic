/******************************************************************
 *
 * @short	crossing link on road links on traffic lights, etc
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapTransverse
 * superclass:	TrMapLink
 * modul:	tr_map_transverse	geometric selectable object
 * @version	0.1
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2025-2025
 *
 * beginning:	03.2025
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


#ifndef TR_MAP_TRANSVERSE_H
#define TR_MAP_TRANSVERSE_H

#include "tr_map_link.h"

class TrMapTransverse : public TrMapLink
{
public:
	TrMapTransverse();

	bool setSurroundingRect();

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

#ifdef TR_SERIALIZATION
	QString getXmlDescription();

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_TRANSVERSE_H
