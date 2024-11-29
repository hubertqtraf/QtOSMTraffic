/******************************************************************
 *
 * @short	base class of trafalgar geometric selectable objects 
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoObject
 * superclass:	---
 * modul:	trgeo_object.cc	geometric selectable object	
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2009-2022
 *
 * beginning:	04.2009
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


#include "tr_geo_object.h"

#include <QtCore/qdebug.h>

uint64_t TrGeoObject::s_mask = 0;

TrGeoObject::TrGeoObject() 
	: m_geo_active_pen(nullptr)
	, disabled_pen(nullptr)
	, select_id(-1)
	, m_flags(0)
{
	surroundingRect[0] = surroundingRect[1] = surroundingRect[2] = surroundingRect[3] = 0.0;

	m_inst_mask = TR_MASK_EXIST;
}

TrGeoObject::TrGeoObject (const TrGeoObject& other)
	: m_geo_active_pen(other.m_geo_active_pen)
	, disabled_pen(other.disabled_pen)
	, select_id(-1)
{
	surroundingRect[0] = other.surroundingRect[0];
	surroundingRect[1] = other.surroundingRect[1];
	surroundingRect[2] = other.surroundingRect[2];
	surroundingRect[3] = other.surroundingRect[3];
	m_inst_mask = other.m_inst_mask;
}

TrGeoObject::~TrGeoObject()
{
}

QDebug operator<<(QDebug dbg, const TrGeoObject& obj)
{
	return dbg << "base class";
}

QString TrGeoObject::getName() const
{
	return QString("");
}

bool TrGeoObject::setName(const QString & name)
{
	return false;
}

QString TrGeoObject::getXmlName() const
{
	return "tr_object";
}

void TrGeoObject::setGlobelFlags(uint64_t flags)
{
	TR_MSG << HEX << s_mask << " > " << flags;
	s_mask = flags;
}

uint64_t TrGeoObject::getGlobelFlags()
{
	return s_mask;
}

void TrGeoObject::setMask(uint64_t bit_mask)
{
	m_inst_mask |= bit_mask;
}

void TrGeoObject::removeMask(uint64_t bit_mask)
{
	m_inst_mask &= ~(bit_mask);
}

uint64_t TrGeoObject::checkMask(uint64_t bit_mask)
{
	return (bit_mask & m_inst_mask);
}

bool TrGeoObject::addPen(const QString & group, int idx, const QPen & pen)
{
	TR_WRN << "do not use";
	return false;
}

void TrGeoObject::setColorGroup(const QString & group, QMap<int, QColor> & colors)
{
	for(QMap<int, QColor>::const_iterator ii = colors.constBegin();
		ii != colors.constEnd(); ++ii)
	{
		addPen(group, ii.key(), ii.value());
		appendObjectBrush(ii.key(), QBrush(ii.value()));
	}
}

bool TrGeoObject::appendObjectBrush(int idx, QBrush brush)
{
	return false;
}

void TrGeoObject::setActivePen(QPen * pen)
{
	if(pen == nullptr)
	{
        TR_WRN << "pen == nullptr" << this << *this;
		return;
	}
	m_geo_active_pen = pen;
}

void TrGeoObject::setActiveBrush(QBrush * brush)
{
}


QPen * TrGeoObject::getActivePen()
{
	return m_geo_active_pen;
}

// virtual default
void TrGeoObject::setNameList(TrGeoObject * list)
{
}

void TrGeoObject::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
    TR_INF << "bla";
}

void TrGeoObject::resize(const TrZoomMap & zoom_ref, int x, int y)
{
}

bool TrGeoObject::setPosition(double x, double y, int idx)
{
	return true;
}

uint64_t TrGeoObject::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	if((surroundingRect[0] < inside.x) && (surroundingRect[2] > inside.x) &&
			(surroundingRect[1] < inside.y) && (surroundingRect[3] > inside.y))
	{
		return 0;
	}
	return TR_NO_VALUE;
}

uint64_t TrGeoObject::editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids)
{
	return TR_NO_VALUE;
}

void TrGeoObject::drawSelect(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
}

void TrGeoObject::drawSurroundingRect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrPoint screen1;
	TrPoint screen2;

	if(m_inst_mask & TR_MASK_SELECTED)
	{
		screen1.x = surroundingRect[0];
		screen1.y = surroundingRect[1];
		screen2.x = surroundingRect[2];
		screen2.y = surroundingRect[3];
		zoom_ref.setMovePoint(&screen1.x,&screen1.y);
		zoom_ref.setMovePoint(&screen2.x,&screen2.y);
		QPainter::CompositionMode comp = p->compositionMode();
		p->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
		if(mode & TR_OBJ_DRAW_ERR)
			p->setPen(QColor(0x00, 0xff, 0xff));
		else
			p->setPen(QColor(0xff, 0xff, 0xff));
		p->setBrush(Qt::NoBrush);	
		p->drawRect(QRectF(screen1.x,screen1.y,
			screen2.x-screen1.x,screen2.y-screen1.y));
		// Default: CompositionMode_SourceOver
		p->setCompositionMode(comp);
	}
}

void TrGeoObject::disactivatePoints()
{
}

double TrGeoObject::getSurroundRectVal(int n)
{
	// TODO: check limit
	if((n<0) || (n>3))
		return (-200.0 * TR_COOR_FACTOR);
	return(surroundingRect[n]);
}

void TrGeoObject::updateSurroundRect(TrGeoObject & other, bool first)
{
	double rect[4];

	rect[0] = other.getSurroundRectVal(0);
	rect[1] = other.getSurroundRectVal(1);
	rect[2] = other.getSurroundRectVal(2);
	rect[3] = other.getSurroundRectVal(3);
	updateSurroundRect(rect, first);
}

void TrGeoObject::updateSurroundRect(double rect[4], bool first)
{
	if(first)
	{
		surroundingRect[0] = rect[0];
		surroundingRect[1] = rect[1];
		surroundingRect[2] = rect[2];
		surroundingRect[3] = rect[3];
	}
	else
	{
		if(surroundingRect[0] > rect[0])
			surroundingRect[0] = rect[0];
		if(surroundingRect[2] < rect[2])
			surroundingRect[2] = rect[2];
		if(surroundingRect[1] > rect[1])
			surroundingRect[1] = rect[1];
		if(surroundingRect[3] < rect[3])
			surroundingRect[3] = rect[3];
	}
}

bool TrGeoObject::clip(const TrZoomMap & zoom_ref)
{
	TrPoint screen1;
	TrPoint screen2;

	// TODO: workaround, clip of second line
	screen1.x = surroundingRect[0] - 100.0;
	screen1.y = surroundingRect[1] - 100.0;

	zoom_ref.setMovePoint(&screen1.x,&screen1.y);
	screen2.x = surroundingRect[2] + 100.0;
	screen2.y = surroundingRect[3] + 100.0;
	zoom_ref.setMovePoint(&screen2.x,&screen2.y);

	if(screen2.x < 0)
		return true;

	if(screen1.x > zoom_ref.getScreenWidth())
		return true;

	if(screen1.y < 0)
		return true;

	if(screen2.y > zoom_ref.getScreenHeight())
		return true;
	return false;
}

void TrGeoObject::setActBrush(QBrush * brush)	// Default of virtual 
{
}

uint16_t TrGeoObject::getType() const
{
	return m_flags & 0x000000000000ffff;
}

void TrGeoObject::setType(uint16_t type)
{
	m_flags &= 0xffffffffffff0000;
	m_flags |= type;
}

void TrGeoObject::setLayerShowMask(uint64_t mask)
{
}

void TrGeoObject::clear()
{
}

bool TrGeoObject::exportGeoJson(QJsonObject & geojson, uint64_t mode)
{
	return false;
}

bool TrGeoObject::importGeoJson(const QJsonObject & geojson, uint64_t mode)
{
	return false;
}

// TODO: create real function ...
uint64_t TrGeoObject::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TR_NO_VALUE;
}

#ifdef TR_SERIALIZATION
bool TrGeoObject::readDefStartElement(QXmlStreamReader & xml_in, QString & ref)
{
	if(xml_in.isStartElement())
	{
        ref = xml_in.name().toString();
		return true;
	}
	if(xml_in.isCharacters())
	{
		return false;
	}
	if(xml_in.isComment())
	{
		TR_WRN << xml_in.tokenString() << ": " << xml_in.text();
		return false;
	}
	return false;
}

const QString TrGeoObject::readXmlHeader(QXmlStreamReader & xml_in)
{
    QString ref;

	while(!xml_in.atEnd())
	{
        if(readDefStartElement(xml_in, ref))
		{
			if(ref == getXmlName())
                return ref;
		}
	}
    return ref;
}
#endif

void TrGeoObject::abortOnLine(QXmlStreamReader & xml_in, const QString & text)
{
	xml_in.raiseError("Error at line: " +
			QString::number(xml_in.lineNumber()) +
			", " + text);
}

void TrGeoObject::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());
	xml_out.writeEndElement();
}


