/******************************************************************
 *
 * @short	face class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapFace
 * superclass:	TrGeoObject
 * modul:	tr_map_face.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2024
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


#include "tr_map_face.h"
#include "tr_map_list.h"


TrMapFace::TrMapFace()
	: m_pline(nullptr)
	, m_f_type(0xff)
	, m_actBrush(nullptr)
{
}

TrMapFace::TrMapFace(QString name, long id) 
	: TrGeoObject()
	, m_pline(nullptr)
	, m_f_type(0xff)
	, m_actBrush(nullptr)
{
	// polygon version
	//pline = new TrGeoPolygon("", 0); 
}

TrMapFace::~TrMapFace()
{
	if(m_pline)
	{
		delete(m_pline);
		m_pline = nullptr;
	}
}

QDebug operator<<(QDebug dbg, const TrMapFace & face)
{
	return dbg << face.getXmlName() << HEX << "class: " << face.getType() <<
		"type: " << face.m_f_type;
}

// TODO: m_f_type align with link - range is +1
void TrMapFace::setLayerShowMask(uint64_t mask)
{
	m_inst_mask &= ~(TR_MASK_DRAW);

	uint64_t class_mask = 1;
	if((m_f_type & 0x00ff) > 1)
		class_mask = (static_cast<uint64_t>((1 << (m_f_type  & 0x00ff))));
	//TR_MSG << HEX << m_f_type << m_f_class << (class_mask & mask) << class_mask << mask;
	if((class_mask & mask) == class_mask)
	{
		m_inst_mask |= TR_MASK_DRAW;
	}
}

bool TrMapFace::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	if(base == nullptr)
		return false;
	TrMapList * list = dynamic_cast<TrMapList *>(base);
	if(list == nullptr)
		return false;
	uint16_t type = (getType() & 0x00ff);
	QPen * pen = list->getObjectPen(type);//type & 0x001f);
	if(pen != nullptr)
	{
		setActivePen(pen);
	}
	else
	{
		//TR_WRN << "pen " << type << " is not in the list" << m_objPenMap.size();
	}
	return true;
}

void TrMapFace::setActiveBrush(QBrush * brush)
{
	m_actBrush = brush;
}

void TrMapFace::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	if(this->clip(zoom_ref))
		return;	

	if((m_actBrush == nullptr) || (m_geo_active_pen == nullptr))
	{
		if(getActivePen() == nullptr)
		{
			QPen * br = new QPen(QColor(228, 128, 128));
			setActivePen(br);
		}
		p->setBrush(QBrush(QColor(128,128,128)));
		m_pline->setActivePen(getActivePen());
	}
	else
	{
		m_pline->setActivePen(getActivePen());
		m_geo_active_pen->setWidth(2);
		p->setPen(*m_geo_active_pen);
		p->setBrush(*m_actBrush);
	}

	// TODO: polygon is not closed! append first point?
 	if(m_f_type & 0x4000)
		m_pline->draw(zoom_ref, p, 0x02);
	else
		m_pline->draw(zoom_ref, p, 0x00);

	if(m_inst_mask & TR_MASK_SELECTED)
		TrGeoObject::drawSurroundingRect(zoom_ref, p, 0);
}

void TrMapFace::appendPolyPoint(TrPoint pt)
{
	QVector<TrPoint> next_points;
	next_points.append(pt);
	m_pline->appendPoints(next_points);
}

void TrMapFace::appendPolygon(uint8_t flags)
{
	// TODO: array of polygons
	m_pline = new TrGeoPolygon();
	m_pline->setMask(TR_POLY_TYPE_DEF | TR_MASK_DRAW);
}

bool TrMapFace::setSurroundingRect()
{
	if(m_pline == nullptr)
		return false;

	bool first = true;
	if(m_pline->setSurroundingRect() == true)
	{
		updateSurroundRect(*m_pline, first);
		first = false;
		return true;
	}
	return false;
}

uint16_t TrMapFace::getDrawType()
{
	return m_f_type;
}

void TrMapFace::setDrawType(uint16_t type)
{
	m_f_type = type;
}

#ifdef TR_SERIALIZATION
bool TrMapFace::exportGeoJson(QJsonObject & geojson, uint64_t mode)
{
	//TR_MSG;

	QJsonObject properties;
	properties["class"] = m_f_class;
	properties["type"]  = m_f_type;
	geojson["properties"] = properties;

	/*if((m_f_type & 0x000f) > 12)
	{
		TR_WRN << "type: " << HEX << m_f_type;
		return false;
	}*/

	if(m_pline != nullptr)
	{
		QJsonObject properties;
		properties["class"] = m_f_class;
		properties["type"]  = m_f_type;
		geojson["properties"] = properties;

		return m_pline->exportGeoJson(geojson, mode);
	}
	return false;
}

bool TrMapFace::importArrayJson(const QJsonArray & arrjson, uint64_t mode)
{
	appendPolygon(0x00);
	setFaceClass(1);
	return m_pline->importArrayJson(arrjson, 1);
}

bool TrMapFace::importGeoJson(const QJsonObject & geojson, uint64_t mode)
{
	// MultiPolygon
	// LineString

	if(geojson.contains("type"))
	{
		if(geojson.value("type").toString() != "Feature")
			return false;
	}
	appendPolygon(0x00);
	if(geojson.contains("properties"))
	{
		QJsonObject properties = geojson["properties"].toObject();

		m_f_class = properties["class"].toInt();
		m_f_type  = properties["type"].toInt();
	}
	if(geojson.contains("geometry") && (m_pline != nullptr))
	{
		return m_pline->importGeoJson(geojson["geometry"].toObject(), 0);
	}
	return false;
}

uint64_t TrMapFace::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TR_NO_VALUE;
}

void TrMapFace::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	// TR_MSG << "face_class";

	// replaced by json
}
#endif
