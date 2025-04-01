/******************************************************************
 *
 * @short	point of interest class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoi
 * superclass:	TrGeoPoint
 * modul:	tr_geo_poi.cc	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2023-2025
 *
 * beginning:	03.2023
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



#include <math.h>

#include "tr_geo_point.h"
#include "tr_map_poi.h"
#include "tr_geo_poly.h"

#include "tr_prof_class_def.h"

#define SELECT_SIZE 6

#include <QtGui/qstatictext.h>
// switch escape codes in 'name'
// TODO: do this in name objects?
#include <QtGui/qtextdocument.h> // QTextDocument

TrMapPoi::TrMapPoi()
	: TrGeoPoint()
	, m_poi_flags(0)
	, m_poi_data(0)
{
}

TrMapPoi::~TrMapPoi()
{
}

QDebug operator<<(QDebug dbg, const TrMapPoi & poi)
{
	return dbg << poi.getXmlName() << poi.m_name << " (" << TR_COOR(poi.m_pt) << ")"
		<< HEX << " flag: " << poi.m_poi_flags << " data: " << poi.m_poi_data;
}

QString TrMapPoi::getXmlName() const
{
	return "map_poi";
}

uint64_t TrMapPoi::getPoiTypeFlags() const
{
	return m_poi_flags;
}

void TrMapPoi::setPoiTypeFlags(const uint64_t flags)
{
	m_poi_flags = flags;
}

void TrMapPoi::setPoiNumData(const uint64_t data)
{
	m_poi_data = data;
}

uint64_t TrMapPoi::getPoiNumData() const
{
	return m_poi_data;
}

void TrMapPoi::setPoiName(const QString & name)
{
	m_name = name;
}

QString TrMapPoi::getPoiName()
{
	return m_name;
}


void TrMapPoi::setLayerShowMask(uint64_t mask)
{
	//TR_MSG << HEX << mask;
	m_inst_mask &= ~(TR_MASK_DRAW);
	if((m_poi_flags & (TYPE_POI_P_ALPINE)) && (mask & 0x80))
	{
		m_inst_mask |= TR_MASK_DRAW;
		return;
	}
	if((m_poi_flags & (TYPE_ADMIN /*| TYPE_POI_P_CHURCH | TYPE_POI_P_FOOD*/)) && (mask & 0x02))
		m_inst_mask |= TR_MASK_DRAW;
	if((m_poi_flags & (TYPE_PUBLIC /*| TYPE_POI_P_PARKING | TYPE_POI_P_CHURCH | TYPE_POI_P_FOOD*/)) && (mask & 0x04))
	{
		m_inst_mask |= TR_MASK_DRAW;
		return;
	}
	if((m_poi_flags & (TYPE_NATURAL /*| TYPE_POI_N_PEAK | TYPE_POI_N_TREE | TYPE_POI_N_CAVE*/)) && (mask & 0x08))
		m_inst_mask |= TR_MASK_DRAW;
	if((m_poi_flags & (TYPE_BUILDING /*| TYPE_RESTRICT*/)) && (mask & 0x10))
		m_inst_mask |= TR_MASK_DRAW;
	if((m_poi_flags & (TYPE_POWER)) && (mask & 0x20))
		m_inst_mask |= TR_MASK_DRAW;
	if((m_poi_flags & (TYPE_ROAD | TYPE_RAIL | TYPE_STREAM)) && (mask & 0x40))
		m_inst_mask |= TR_MASK_DRAW;
}

bool TrMapPoi::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	TrMapList * list = dynamic_cast<TrMapList *>(base);
	if(list == nullptr)
		return false;

	// TODO: wrong flag to select the base parameter
	//if(ctrl & TR_INIT_GEOMETRY)
	//	return false;

	// just a test - name should be not a text
	if(m_name.size() > 80)
	{
		TR_MSG << "long POI name: " << m_name.size() << " - "  << m_name;
		return false;
	}

	if(m_poi_flags & TYPE_POI_N_PEAK)
        {
                if(((m_poi_data & 0x00000000000000ffff) == 0x00000000000000ffff) &&
			(m_name.size() == 0))
		{
			TR_MSG << "peak " << m_geo_id << ": " << "no info, ignore";
			return false;
		}
	}

	if(m_poi_flags & TYPE_ADMIN)
	{
		if(m_poi_flags & TYPE_POI_A_VILL)
			m_geo_active_pen = list->getObjectPen(1);
	}
	if(m_poi_flags & TYPE_PUBLIC)
	{
		if(m_name.size())
			m_geo_active_pen = list->getObjectPen(2);
		// TODO: set an another color
		if(m_poi_flags & TYPE_POI_P_PARKING)
			m_geo_active_pen = list->getObjectPen(4);
	}

	if(m_poi_flags & TYPE_NATURAL)
		m_geo_active_pen = list->getObjectPen(3);

	if(m_poi_flags & (TYPE_BUILDING | TYPE_RESTRICT))
		m_geo_active_pen = list->getObjectPen(4);

	if(m_poi_flags & BUILDING_POWER)
		//if(m_poi_flags & TYPE_POWER)
		m_geo_active_pen = list->getObjectPen(8);

	if(m_poi_flags & (TYPE_ROAD | TYPE_RAIL | TYPE_STREAM))
		m_geo_active_pen = list->getObjectPen(6);

	if(m_poi_flags & TYPE_POI_P_ALPINE)
		m_geo_active_pen = list->getObjectPen(7);

	return true;
}

void TrMapPoi::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	if(!(m_inst_mask & TR_MASK_DRAW))
		return;

	if(this->clip(zoom_ref))
		return;

	if(getActivePen() == nullptr)
	{
		//TR_WRN << "getActivePen() == nullptr";
		return;
	}

	if(m_inst_mask & TR_MASK_SELECTED)
		drawSelect(zoom_ref, p, mode);
	// TODO: brush?
	TrPoint screen = getPoint();
	zoom_ref.setMovePoint(&screen.x,&screen.y);
	bool text = true;
	p->setPen(*getActivePen());

	if(m_poi_flags & TYPE_POI_P_ALPINE)
	{
		p->fillRect(static_cast <int>(screen.x-3),
		static_cast <int>(screen.y-6), 12, 6, QBrush(getActivePen()->color()));
	}

	p->setBrush(Qt::NoBrush);
	if(m_poi_flags & TYPE_POI_N_PEAK)
	{
		uint64_t alt = m_poi_data & 0x00000000000000ffff;
		QString text;
		if(alt != 0x00000000000000ffff)
			text = m_name + " (" + QString::number(alt) + ")";
		else
			text = m_name;
		if(!text.size())
		{
			// no name + no alt -> nonsense
			TR_WRN << "peak " << m_geo_id << ": " << "no info, ignore";
			return;
		}
		p->drawLine(static_cast <int>(screen.x-4),
			static_cast <int>(screen.y+6),
			static_cast <int>(screen.x),
			static_cast <int>(screen.y));
		p->drawLine(static_cast <int>(screen.x+4),
			static_cast <int>(screen.y+6),
			static_cast <int>(screen.x),
			static_cast <int>(screen.y));
		// TODO: check point draw...
		if(TR_MASK_POINTS_NUM & s_mask)
			p->drawStaticText(static_cast <int>(screen.x),
                              static_cast <int>(screen.y+7), QStaticText(text));
		return;
	}
	if(m_poi_flags & TYPE_POI_P_PARKING)
	{
		p->fillRect(static_cast <int>(screen.x-5),
			static_cast <int>(screen.y-4), 10, 10, QBrush(getActivePen()->color()));
		return;
	}
	if(m_poi_flags & TYPE_POI_P_FUEL)
	{
		p->fillRect(static_cast <int>(screen.x-3),
			static_cast <int>(screen.y-3), 4, 10, QBrush(getActivePen()->color()));
		// TODO: switch the text -> option
		//p->drawStaticText(screen.x+4, screen.y, QStaticText(m_name));
		return;
	}
	// TODO: rework power, red rectangle looks bad...
	if(((m_poi_flags & (TYPE_BUILDING | BUILDING_POWER)) == (TYPE_BUILDING | BUILDING_POWER)))
	//if(m_poi_flags & TYPE_POWER)
	{
		// TODO: add more infomation before to draw
		p->fillRect(static_cast <int>(screen.x-5),
			static_cast <int>(screen.y-7),
			10, 15, QBrush(getActivePen()->color()));
		return;
	}

	if(m_poi_flags & TYPE_PUBLIC)
	{
		//text = false;
		if(m_poi_flags & TYPE_POI_P_GUEST)
			text = true;
		if(m_poi_flags & TYPE_POI_P_FOOD)
			text = true;
		// false: TYPE_POI_P_CHURCH, TYPE_POI_P_MONEY, TYPE_POI_P_SHOP, TYPE_POI_P_TOUR
	}

	if(m_poi_flags & (TYPE_ROAD | TYPE_RAIL | TYPE_STREAM))
	{
		if((m_poi_flags & 0x000000000000000f) == 7)
			p->drawEllipse(static_cast <int>(screen.x-3),
				static_cast <int>(screen.y-10), 6, 20);
		if((m_poi_flags & 0x000000000000000f) == 1)
		{
			p->drawLine(static_cast <int>(screen.x-4),
				static_cast <int>(screen.y-4),
				static_cast <int>(screen.x+4),
				static_cast <int>(screen.y+4));
			p->drawLine(static_cast <int>(screen.x-4),
				static_cast <int>(screen.y+4),
				static_cast <int>(screen.x+4),
				static_cast <int>(screen.y-4));
		}
		// TODO: switch for station/bus stop
	}

	if(m_name.size())
	{
		if(m_poi_flags & TYPE_POI_N_TREE)
			p->drawEllipse(static_cast <int>(screen.x-7),
				static_cast <int>(screen.y-6), 4, 12);
		else
			p->drawEllipse(static_cast <int>(screen.x-3),
				static_cast <int>(screen.y-3), 6, 6);
		if((TR_MASK_POINTS_NUM & s_mask) && text)
			p->drawStaticText(static_cast <int>(screen.x),
				static_cast <int>(screen.y), QStaticText(m_name));
	}
}

// TODO: check hard coded values or move (gui?)
void TrMapPoi::drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrPoint screen = getPoint();
	zoom_ref.setMovePoint(&screen.x,&screen.y);

	QPainter::CompositionMode comp = p->compositionMode();
	p->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	p->setPen(QColor(0xff, 0xff, 0xff));
	p->setBrush(Qt::NoBrush);
	p->drawEllipse(screen.x-7, screen.y-6, 12, 12);
	// Default: CompositionMode_SourceOver
	p->setCompositionMode(comp);
}

bool TrMapPoi::setSurroundingRect()
{
	// TODO: set size of the symbol...
	return TrGeoPoint::setSurroundingRect();
}

#ifdef TR_SERIALIZATION
QString TrMapPoi::getXmlDescription()
{
	return QString();
}

uint64_t TrMapPoi::readXmlDescription(QXmlStreamReader & xml_in)
{
	uint64_t id = TR_NO_VALUE;

	QString name = readXmlHeader(xml_in);

	while(!xml_in.atEnd())
	{
		QString ref;

		xml_in.readNext();
		if(readDefStartElement(xml_in, ref))
		{
			//TR_MSG << ref;
			if(ref == "geo_point")
			{
				id = TrGeoPoint::readXmlPointData(xml_in, "geo_point");
			}
			if(ref == "poi")
			{
				QXmlStreamAttributes attr = xml_in.attributes();
				QString txt = attr.value("", "name").toString();
				QTextDocument doUnEscape;
				doUnEscape.setHtml(txt);
				m_name = doUnEscape.toPlainText();

				// TODO: check 'ok'
				bool ok;
				m_poi_flags = attr.value("", "flags").toULong(&ok, 16);
				m_poi_data  = attr.value("", "data").toULong(&ok, 16);
				//TR_INF << m_name << HEX << m_poi_flags << m_poi_data;
			}
		}
		else if(xml_in.isEndElement())
		{
			//TR_MSG << xml_in.name();

			if(xml_in.name().toString() == "geo_point")
			{
			}
			if(xml_in.name().toString() == getXmlName())
			{
				return id;
			}
		}
	}
	return id;
}

void TrMapPoi::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());
	xml_out.writeStartElement("geo_point");
	writeXmlPointPart(xml_out, id);
	xml_out.writeEndElement();
	xml_out.writeStartElement("poi");
	// add Html escape code if needed
	xml_out.writeAttribute("name", QVariant(m_name).toString().toHtmlEscaped());
	QString flags = "0x" + QString::number(m_poi_flags, 16);
	xml_out.writeAttribute("flags", QVariant(flags).toString());
	QString data = "0x" + QString::number(m_poi_data, 16);
	xml_out.writeAttribute("data", QVariant(data).toString());
	xml_out.writeEndElement();
	xml_out.writeEndElement();
}
#endif
