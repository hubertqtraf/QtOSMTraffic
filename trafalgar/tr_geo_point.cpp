/******************************************************************
 *
 * @short	point class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoint
 * superclass:	TrGeoObject
 * modul:	tr_geo_point.cc	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2021-2025
 *
 * beginning:	05.2021
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
#include "tr_geo_poly.h"

#define SELECT_SIZE 6


TrGeoPoint::TrGeoPoint()
	: TrGeoObject()
	, m_view_opt(0x00)
	, stdPen(nullptr)
	, m_pt{0.0, 0.0}
	, m_pt_ref(nullptr)
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
	m_geo_id = -1;
}

TrGeoPoint::TrGeoPoint (const TrGeoPoint& other)
	: TrGeoObject(other)
	, m_view_opt(0x00)
	, stdPen(nullptr)
	, m_pt_ref(nullptr)
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
	m_geo_id = -1;

	m_pt = other.m_pt;
	if(other.m_pt_ref != nullptr)
	{
		m_pt_ref = new TrPointRef;
		*m_pt_ref = *other.m_pt_ref;
	}
}

TrGeoPoint::~TrGeoPoint()
{
	this->resetRef();
}

QDebug operator<<(QDebug dbg, const TrGeoPoint& point)
{
	if(point.m_pt_ref != nullptr)
		return dbg << point.getXmlName() << " Ref: " << TR_COOR_REF(point.m_pt_ref);
	return dbg << point.getXmlName() << " (" << TR_COOR(point.m_pt) << ")";
}

QString TrGeoPoint::getXmlName() const
{
	return "geo_point";
}

uint8_t TrGeoPoint::getViewOpt()
{
	return m_view_opt;
}

void TrGeoPoint::setViewOpt(uint8_t opt)
{
	m_view_opt = opt;
}

int64_t TrGeoPoint::getGeoId() const
{
	return m_geo_id;
}

void TrGeoPoint::setGeoId(int64_t gid)
{
	m_geo_id = gid;
}

void TrGeoPoint::setPoint(const TrPoint & point)
{
	m_pt = point;
}

TrPoint TrGeoPoint::getPoint() const
{
	return m_pt;
}

void TrGeoPoint::setPointRef(const TrPointRef & ref)
{
	if(m_pt_ref == nullptr)
	{
		m_pt_ref = new TrPointRef;
	}

	m_pt_ref->dist      = ref.dist;
	m_pt_ref->intercept = ref.intercept;
	m_pt_ref->id        = ref.id;
	m_pt_ref->type      = ref.type;
	m_pt_ref->no        = ref.no;
}

void TrGeoPoint::resetRef()
{
	delete m_pt_ref;
	m_pt_ref = nullptr;
}


TrPointRef * TrGeoPoint::getPointRef() const
{
	return m_pt_ref;
}

bool TrGeoPoint::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	TrMapList * plist = dynamic_cast<TrMapList*>(base);
	if(!plist)
		return false;

	//TR_INF << m_geo_active_pen << HEX << ctrl;
	if((m_geo_active_pen == nullptr) && (ctrl & TR_INIT_COLORS))
	{
		// '0x1003' -> 'marker_1'
		setActivePen(plist->getObjectPen(0x1003));
	}

	if(m_pt_ref == nullptr)
		return false;

	//TR_MSG << m_pt_ref->id << m_pt_ref->no;

	// TODO: use abstract function? -> primitive -> polygon/circle
	TrGeoPolygon * poly = dynamic_cast<TrGeoPolygon *>(plist->getMapObject(m_pt_ref->id));
	if(poly != nullptr)
	{
		if(size_t((m_pt_ref->no + 1)) > poly->getSize())
		{
			TR_WRN << "size: " << m_pt_ref->no << poly->getSize();
			return false;
		}
		TrPoint first = poly->getPoint(m_pt_ref->no);
		TrPoint second = poly->getPoint(m_pt_ref->no + 1);
		first.x += ((second.x - first.x) * m_pt_ref->dist);
		first.y += ((second.y - first.y) * m_pt_ref->dist);

		poly_add add = poly->getGeoSegmentData(m_pt_ref->no);
		TrGeoPolygon::calcParPoint(zoom_ref, first, add.sl, m_pt_ref->intercept);

        setPoint(first);
		return true;
	}
	else
		TR_WRN << "poly == nullptr, id: " << m_geo_id << ", " << m_pt_ref->id;
	return false;
}

void TrGeoPoint::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	if(getActivePen() == nullptr)
	{
		TR_WRN << "getActivePen() == nullptr";
		return;
	}
	p->setPen(*getActivePen());
	// TODO: brush?
	if(m_inst_mask & TR_MASK_SELECTED)
		drawSelect(zoom_ref, p, mode);
	drawElement(zoom_ref, p, 0x00);
}

void TrGeoPoint::drawText(const TrZoomMap & zoom_ref, QPainter * p, const QString & text)
{
	if(!(s_mask & TR_MASK_SHOW_POINTS))
		return;

	TrPoint screen;
	screen.x = m_pt.x;
	screen.y = m_pt.y;
	zoom_ref.setMovePoint(&screen.x, &screen.y);

	if(TR_MASK_POINTS_NUM & s_mask)
	{
        int pos_y = static_cast <int>(screen.y + DRAW_SIZE);
		//if(mode == 1)
        pos_y = static_cast <int>(screen.y + DRAW_SIZE + 12);      // TODO: hight of the text
        p->drawText(static_cast <int>(screen.x + DRAW_SIZE*2), pos_y, text + QString::number(m_geo_id));
	}
}

void TrGeoPoint::drawElement(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	/*if(this->checkMask(TR_MASK_SELECTED) == TR_MASK_SELECTED)
	{
		drawSelect(zoom_ref, p, 0);
	}*/
	if(!(s_mask & TR_MASK_SHOW_POINTS))
		return;

	TrPoint screen;
	screen.x = m_pt.x;
	screen.y = m_pt.y;
	zoom_ref.setMovePoint(&screen.x, &screen.y);

	if(m_pt_ref != nullptr)
	{
		p->drawLine(static_cast <int>(screen.x-SELECT_SIZE),
			static_cast <int>(screen.y+SELECT_SIZE),
			static_cast <int>(screen.x+SELECT_SIZE),
			static_cast <int>(screen.y-SELECT_SIZE));
		p->drawLine(static_cast <int>(screen.x+SELECT_SIZE),
			static_cast <int>(screen.y+SELECT_SIZE),
			static_cast <int>(screen.x-SELECT_SIZE),
			static_cast <int>(screen.y-SELECT_SIZE));
	}
	else
	{
		p->drawRect(static_cast <int>(screen.x-DRAW_SIZE),
			static_cast <int>(screen.y-DRAW_SIZE), DRAW_SIZE*2, DRAW_SIZE*2);
	}
}

// TODO: check hard coded values or move (gui?)
void TrGeoPoint::drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrPoint screen = getPoint();

	zoom_ref.setMovePoint(&screen.x,&screen.y);

	//zoom_ref.setPoint(&screen.x, &screen.y);
	QPainter::CompositionMode comp = p->compositionMode();
	p->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
	p->setPen(QColor(0xff, 0xff, 0xff));
	p->setBrush(Qt::NoBrush);
	if(mode)
	{
		p->drawEllipse(static_cast <int>(screen.x-7),
			static_cast <int>(screen.y-6), 12, 12);
	}
	else
	{
		p->drawRect(static_cast <int>(screen.x-5),
			static_cast <int>(screen.y-5), 10, 10);
	}
	// Default: CompositionMode_SourceOver
	p->setCompositionMode(comp);
}

bool TrGeoPoint::isCloseToPoint(const TrZoomMap & zoom_ref, const TrPoint & inside, const TrPoint & pos)
{
	double x1 = inside.x;
	double y1 = inside.y;

	zoom_ref.setPoint(&x1, &y1);

	double x2 = x1-SELECT_SIZE;
	double y2 = y1-SELECT_SIZE;
	x1 += SELECT_SIZE;
	y1 += SELECT_SIZE;

	zoom_ref.getPoint(&x1, &y1);
	zoom_ref.getPoint(&x2, &y2);

	if((x2 < pos.x) && (x1 > pos.x) && (y1 < pos.y) && (y2 > pos.y))
	{
		return true;
	}
	return false;
}

uint64_t TrGeoPoint::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	if(TrGeoPoint::isCloseToPoint(zoom_ref, inside, m_pt))
		return m_geo_id;

	return TR_NO_VALUE;
}


bool TrGeoPoint::setSurroundingRect()
{
	surroundingRect[0] = m_pt.x;
	surroundingRect[2] = m_pt.x;
	surroundingRect[1] = m_pt.y;
	surroundingRect[3] = m_pt.y;
	return true;
}

#ifdef TR_SERIALIZATION
QString TrGeoPoint::getXmlDescription()
{
	return QString();
}

uint64_t TrGeoPoint::readXmlDescription(QXmlStreamReader & xml_in)
{
	QStringRef name = readXmlHeader(xml_in);

	//TR_MSG << name;

	if(name != getXmlName())
	{
		TR_ERR;
		return TR_NO_VALUE;
	}
	return readXmlPointData(xml_in, getXmlName());
}

TrGeoPoint::readXmlPointData(QXmlStreamReader & xml_in, const QString & block)
{
	TrPointRef ref;

	while(!xml_in.atEnd())
	{
		if(xml_in.isComment())
		{
			TR_WRN << xml_in.tokenString() << ": " << xml_in.text();
			xml_in.readNext();
		}
		else if(xml_in.isCharacters())
		{
			xml_in.readNext();
		}
		else if(xml_in.isStartElement())
		{
			if(xml_in.name() == "point")
			{
				QXmlStreamAttributes attr = xml_in.attributes();

				m_pt.x = attr.value("", "lon").toDouble() * TR_COOR_FACTOR;
				m_pt.y = attr.value("", "lat").toDouble() * TR_COOR_FACTOR;
			}
			if(xml_in.name() == block)
			{
				QXmlStreamAttributes id_attr = xml_in.attributes();
				m_geo_id = id_attr.value("", "id").toLong();
				if(m_geo_id == 0)
					return 0;
			}
			if(xml_in.name() == "ref")
			{
				QXmlStreamAttributes attr = xml_in.attributes();
				ref.dist = attr.value("", "dist").toDouble();
				ref.intercept = attr.value("", "intercept").toInt();
				ref.id = attr.value("", "id").toLong();
				ref.type = attr.value("", "type").toInt();
				ref.no = attr.value("", "no").toInt();
			}
			xml_in.readNext();

		}
		else if(xml_in.isEndElement())
		{
			if(xml_in.name() == block)
			{
				xml_in.readNext();
				return m_geo_id;
			}
			else if(xml_in.name() == "ref")
			{
				//TR_MSG << m_geo_id << "close the tag -> set the ref";
				this->setPointRef(ref);
				xml_in.readNext();
			}
			xml_in.readNext();
		}
	}
	return m_geo_id;
}

void TrGeoPoint::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());
	writeXmlPointPart(xml_out, id);
	xml_out.writeEndElement();
}


void TrGeoPoint::writeXmlPointPart(QXmlStreamWriter & xml_out, uint64_t id)
{
	// TODO: use getXmlName()
	//xml_out.writeStartElement(getXmlName());

	xml_out.writeAttribute("id", QVariant((qulonglong)id).toString());

	xml_out.writeStartElement("point");

	if(m_pt_ref == nullptr)
	{
		xml_out.writeAttribute("lon", QVariant(m_pt.x/TR_COOR_FACTOR).toString());
		xml_out.writeAttribute("lat", QVariant(m_pt.y/TR_COOR_FACTOR).toString());
	}
	else
	{
		xml_out.writeStartElement("ref");

		xml_out.writeAttribute("dist", QVariant(m_pt_ref->dist).toString());
		xml_out.writeAttribute("intercept", QVariant(m_pt_ref->intercept).toString());
		xml_out.writeAttribute("id", QVariant((qulonglong)m_pt_ref->id).toString());
		xml_out.writeAttribute("type", QVariant(m_pt_ref->type).toString());
		xml_out.writeAttribute("no", QVariant((quint16)m_pt_ref->no).toString());

		xml_out.writeEndElement();
	}

	xml_out.writeEndElement();

	//xml_out.writeEndElement();
}
#endif

