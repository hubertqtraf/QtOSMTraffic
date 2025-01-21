/******************************************************************
 *
 * @short	layer element used by the stack class
 *
 * project:	Trafalgar lib
 *
 * class:	TrLayer
 * superclass:	TrGeoObject
 * modul:	tr_layer.cc 
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	10.2019
 *
 * (C)		Schmid Hubert (C)2019-2025
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

#include "tr_layer.h"


#include <QtCore/qdebug.h>


TrLayer::TrLayer()
	: TrGeoObject()
	, m_flags(0)
	, m_alias("")
	, m_element(nullptr)
{
	surroundingRect[0] = surroundingRect[1] = surroundingRect[2] = surroundingRect[3] = 0;
}

TrLayer::~TrLayer()
{
	TR_MSG;

	if(m_element != nullptr)
		delete m_element;
}

QDebug operator<<(QDebug dbg, const TrLayer& layer)
{
    return dbg << layer.m_element;
}

void TrLayer::clear()
{
	TR_WRN << "check";
}

void TrLayer::setElement(TrGeoObject * elem)
{
	m_element = elem;
}

uint32_t TrLayer::getFlags()
{
	return m_flags;
}

TrGeoObject * TrLayer::getElement()
{
	return m_element;
}

bool TrLayer::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	return false;
}

void TrLayer::setActExt(bool set)
{
	m_flags &= ~TR_LAYER_FIRST;

	if(set)
		m_flags |= TR_LAYER_FIRST;
}

bool TrLayer::setSurroundingRect()
{
	if(m_element == nullptr)
	{
		surroundingRect[0] = surroundingRect[1] =
			surroundingRect[2] = surroundingRect[3] = 0.0;
		return false;
	}
	if(m_element->setSurroundingRect())
	{
		updateSurroundRect(*m_element, true);
		return true;
	}
	return false;
}

#ifdef TR_SERIALIZATION
void TrLayer::writeXmlDescription(QXmlStreamWriter & xml_out, int64_t id)
{
	TR_MSG << "name or NULL: " << m_element->getName();

    if(m_element != nullptr)
	{
        if(m_element->getName() != nullptr)
			m_element->writeXmlDescription(xml_out, id);
	}
}
#endif

