/******************************************************************
 *
 * @short	layer element used by the stack class
 *
 * project:	Trafalgar lib
 *
 * class:	TrLayer
 * superclass:	TrGeoObject
 * modul:	tr_layer.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	10.2019
 *
 * @author	Schmid Hubert (C)2019-2020
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

#ifndef TR_LAYER
#define TR_LAYER

#include <stdint.h>

#include "tr_geo_object.h"

#define TR_LAYER_FIRST 0x80000000

class TrLayer : public TrGeoObject
{
//private:
public:
	uint32_t m_flags;
	QString m_alias;
	TrGeoObject * m_element;

public:
	TrLayer();
	virtual ~TrLayer();

    friend QDebug operator<<(QDebug dbg, const TrLayer& layer);

	// TODO: check fuction needed?
	void clear();

	void setElement(TrGeoObject * elem);

	uint32_t getFlags();

	TrGeoObject * getElement();

	void setActExt(bool set);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual bool setSurroundingRect();

#ifdef TR_SERIALIZATION
	void writeXmlDescription(QXmlStreamWriter & xml_out, int64_t id);
#endif
};

#endif //TR_LAYER

