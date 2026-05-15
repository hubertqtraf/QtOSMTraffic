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
 * @author	Schmid Hubert (C)2025-2026
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
	, m_poi1(nullptr)
	, m_poi2(nullptr)
	, m_pt_first{0.0,0.0}
	, m_pt_last{0.0,0.0}
{
}

TrMapPoi * TrMapTransverse::getPoi(bool dir)
{
	if(dir)
		return m_poi1;
	return m_poi2;
}

TrMapPoi * TrMapTransverse::getPoiType(TrMapList * list, int64_t nd)
{
	TrGeoObject * poi_obj = list->getObject(nd);
	if(poi_obj == nullptr)
		return nullptr;

	TrMapPoi * poi = dynamic_cast<TrMapPoi *>(poi_obj);
	if(poi == nullptr)
		return nullptr;
	if((poi->getPoiTypeFlags() & 0x000000000000000f) != 1)
		return nullptr;
	return poi;
}

bool TrMapTransverse::hasTransverse(TrGeoObject *obj, int64_t nd)
{
	if(obj == nullptr)
		return false;
	TrMapList * list = dynamic_cast<TrMapList *>(obj);
	if(list == nullptr)
		return false;
	if(list->getObject(nd) == nullptr)
		return false;
	//TR_INF << nd;
	return true;
}

void TrMapTransverse::setPoi(TrGeoObject *obj, TrMapLink * link)
{
	if(obj == nullptr)
		return;
	TrMapList * list = dynamic_cast<TrMapList *>(obj);
	if(list == nullptr)
		return;
	m_poi1 = this->getPoiType(list, link->getNodeFrom());
	m_poi2 = this->getPoiType(list, link->getNodeTo());
}

void TrMapTransverse::setLinkData(TrMapLink &link)
{
	setActivePen(link.getActivePen());
	if(link.getOneWay() & TR_LINK_DIR_BWD)
	{
		removeMask(TR_MASK_DRAW);
	}
	m_pt_last = link.getNodeFromRef()->getPoint();
	m_pt_first = link.getNodeToRef()->getPoint();
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
	if(!(m_inst_mask & TR_MASK_DRAW))
		return;
	if(getActivePen() == nullptr)
	{
		TR_WRN << "getActivePen() == nullptr";
		return;
	}
	QPolygon poly(2);
	QPen tra_pen = QPen(getActivePen()->color(), 7);
	p->setPen(tra_pen);
	if((m_poi1 != nullptr) && (m_poi2 != nullptr))
	{
		getTwoLine(zoom_ref, poly, m_poi1->getPoint(), m_poi2->getPoint());
		p->drawPolyline(poly);
		return;
	}
	if(m_poi1 != nullptr)
	{
		getTwoLine(zoom_ref, poly, m_poi1->getPoint(), m_pt_first);
		p->drawPolyline(poly);
	}
	if(m_poi2 != nullptr)
	{
		getTwoLine(zoom_ref, poly, m_pt_last, m_poi2->getPoint());
		p->drawPolyline(poly);
	}
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
