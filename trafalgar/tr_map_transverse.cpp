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


#include "tr_map_transverse.h"

TrMapTransverse::TrMapTransverse()
    : TrMapLink()
{

}

bool TrMapTransverse::setSurroundingRect()
{
    return false;
}

bool TrMapTransverse::init(const TrZoomMap &zoom_ref, uint64_t ctrl, TrGeoObject *base)
{
    return false;
}

void TrMapTransverse::draw(const TrZoomMap &zoom_ref, QPainter *p, uint8_t mode)
{

}

#ifdef TR_SERIALIZATION
QString TrMapTransverse::getXmlDescription()
{

}

uint64_t TrMapTransverse::readXmlDescription(QXmlStreamReader &xml_in)
{

}

void TrMapTransverse::writeXmlDescription(QXmlStreamWriter &xml_out, uint64_t id)
{

}
#endif
