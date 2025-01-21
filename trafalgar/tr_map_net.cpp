/******************************************************************
 *
 * @short	directed graph
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapNet
 * superclass:	TrGeoObject
 * modul:	tr_map_net.cc 
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2025
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


#include "tr_map_net.h"
#include "tr_map_node.h"
#include "tr_map_link_road.h"

// Segment: for debug output drawing used for cross point
#ifdef SEG_TEST
TrGeoSegment * TrMapNet::ms_seg_1 = new TrGeoSegment;
TrGeoSegment * TrMapNet::ms_seg_2 = new TrGeoSegment;
#endif

TrMapNet::TrMapNet()
	: TrGeoObject()
	, m_name("")
	, m_mask_cmp(TrGeoObject::s_mask)
	, m_link_list(nullptr)
	, m_node_map(nullptr)
	, m_primive_map(nullptr)
	, m_complex_map(nullptr)
{
}

TrMapNet::~TrMapNet()
{
	TR_MSG;

	manageList(&m_link_list, LIST_DELETE, "map_link");
	manageList(&m_node_map, LIST_DELETE, "geo_point");
	manageList(&m_primive_map, LIST_DELETE, "geo_primive");
	manageList(&m_complex_map, LIST_DELETE, "map_complex");
}

QString TrMapNet::getName() const
{
	return m_name;
}

bool TrMapNet::setName(const QString & name)
{
	m_name = name;
	return true;
}

void TrMapNet::setNameList(TrGeoObject * list)
{
}

bool TrMapNet::manageList(TrMapList ** list, bool del, const QString & name)
{
	if(del)
	{
		delete (*list);
		*list = nullptr;
		return true;
	}
	if(*list == nullptr)
	{
		*list = new TrMapList();
		(*list)->setObjClass(QString(name));
		(*list)->setMask(TR_MASK_DRAW);
		return true;
	}
	return false;
}

bool TrMapNet::init()
{
	// used with new object or import
	// TODO: set all lists - change?
	bool ret_link = manageList(&m_link_list, LIST_CREATE, "map_link");
	bool ret_node = manageList(&m_node_map, LIST_CREATE, "geo_point");
	bool ret_prim = manageList(&m_primive_map, LIST_CREATE, "geo_primive");
	bool ret_comp = manageList(&m_complex_map, LIST_CREATE, "map_complex");
	// TODO: check, needed?
	//m_node_map->removeMask(TR_MASK_DRAW);

	return (ret_link || ret_node || ret_prim || ret_comp);
}

void TrMapNet::setLayerShowMask(uint64_t mask)
{
	//TR_MSG << HEX << "net: " << mask;

	if(m_link_list != nullptr)
		m_link_list->setLayerShowMask(mask);
}

TrMapList * TrMapNet::getNetList(uint64_t set, bool create)
{
	uint64_t type = s_mask & (TR_MASK_SELECT_ALL);
	if(set)
		type = set;
	//TR_MSG << HEX << type;
	switch(type)
	{
	case TR_MASK_SELECT_LINK:
		if(create)
		{
			manageList(&m_link_list, LIST_CREATE, "map_link");
		}
		return m_link_list;

	case TR_MASK_SELECT_POINT:
		if(create)
		{
			manageList(&m_node_map, LIST_CREATE, "geo_point");
		}
		return m_node_map;

	case TR_MASK_SELECT_POLY:
		if(create)
		{
			manageList(&m_primive_map, LIST_CREATE, "geo_primive");
		}
		return m_primive_map;

	case TR_MASK_SELECT_COMPLEX:
		if(create)
		{
			manageList(&m_complex_map, LIST_CREATE, "map_complex");
		}
		return m_complex_map;

	default:
		break;
	}
	return nullptr;
}

// for new net (document) or import
void TrMapNet::appendLink(TrMapLink * link)
{
	if(m_link_list == nullptr)
		return;

	m_link_list->appendObject(link);
}

bool TrMapNet::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	//TR_INF << HEX << ctrl;
	// TODO: check the 'ctrl == 0' line

	if(ctrl == 0)
	{
		TR_ERR << "ctrl == 0";
		abort();
	}
	// create the list if not here
	this->init();

	if(ctrl != 0)
	{
		//TR_MSG << "other modes:" << ctrl;
		//return false;
	}

	if((m_primive_map != nullptr) && (ctrl & TR_INIT_COLORS))
	{
		m_primive_map->init(zoom_ref, ctrl, m_primive_map);
		m_default_pen.setColor(QColor(0x00, 0x55, 0x7f));
	}

	if(m_complex_map != nullptr)
	{
		m_complex_map->init(zoom_ref, ctrl, m_complex_map);
	}

	if(m_node_map != nullptr)
	{
		m_node_map->init(zoom_ref, ctrl, m_primive_map);
	}
	else
	{
		TR_WRN << "no node map object:" << m_name;
		return false;
	}

	// no nodes, no links
	if (m_link_list == nullptr)
	{
		return false;
	}

	m_link_list->init(zoom_ref, ctrl, m_link_list);

	createNodeInOut();

	return true;
}

void TrMapNet::draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	// draw raw or net_link_list

	// TODO: #2, edit: works anyway; view: works only lines are diabled
	//TR_MSG << m_name;
	//if(!(TR_MASK_DRAW & m_inst_mask))	// TODO: ??? -> do this by stack?
	//	return;

#ifdef SEG_TEST
	// debug output on request, one object per net
	p->setPen(QPen(QColor(0,0,155)));
	TrMapNet::ms_seg_1->setSurroundingRect();
	TrMapNet::ms_seg_2->setSurroundingRect();
	p->setPen(QPen(QColor(155,0,0)));
	TrMapNet::ms_seg_1->draw(zoom_ref, p, 0);
	TrMapNet::ms_seg_2->draw(zoom_ref, p, 0);
#endif

	if(m_link_list != nullptr)
	{
		m_link_list->draw(zoom_ref, p, mode);
	}

	if(m_primive_map != nullptr)
	{
		m_primive_map->draw(zoom_ref, p, mode);
	}

	// TODO draw m_complex_map?

	p->setPen(QPen(QColor(255,255,255)));

	if(m_node_map != nullptr)
	{
		m_node_map->draw(zoom_ref, p, mode);
	}
}


bool TrMapNet::setSurroundingRect()
{
	bool first = true;

	if(m_link_list != nullptr)
	{
		if(m_link_list->setSurroundingRect() == true)
		{
			updateSurroundRect(*m_link_list, first);
			first = false;
		}
	}
	if(m_primive_map != nullptr)
	{
		if(m_primive_map->setSurroundingRect() == true)
		{
			updateSurroundRect(*m_primive_map, first);
			first = false;
		}
	}
	if(m_complex_map != nullptr)
	{
		if(m_complex_map->setSurroundingRect() == true)
		{
			updateSurroundRect(*m_complex_map, first);
			first = false;
		}
	}
	if(m_node_map != nullptr)
	{
		if(m_node_map->setSurroundingRect() == true)
		{
			updateSurroundRect(*m_node_map, first);
			first = false;
		}
	}

	if(first == true)
	{
		//TR_WRN << "no list";
		return false;
	}
	TR_INF << "n:" << first <<
		QString::number(surroundingRect[0],'f', 2) <<
		QString::number(surroundingRect[1],'f', 2) <<
		QString::number(surroundingRect[2],'f', 2) <<
		QString::number(surroundingRect[3],'f', 2) << m_name;

	return true;
}

uint64_t TrMapNet::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos, uint64_t mask)
{
	//TR_MSG << inside.x << ", " << inside.y;

	TrMapList * select_list = nullptr;

	if(mask & TR_MASK_SELECT_LINK)
	{
		select_list = m_link_list;
	}
	else if(mask & TR_MASK_SELECT_POLY)
	{
		select_list = m_primive_map;
	}
	else if(mask & TR_MASK_SELECT_POINT)
	{
		select_list = m_node_map;
	}
	else if(mask & TR_MASK_SELECT_COMPLEX)
	{
		select_list = m_complex_map;
	}
	if(select_list != nullptr)
	{
		return select_list->findSelect(zoom_ref, inside, pos);
	}
	return TR_NO_VALUE;
}

uint64_t TrMapNet::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	return findSelect(zoom_ref, inside, pos, s_mask);
}

// TODO: remove...?
uint64_t TrMapNet::editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids)
{
	switch(ids.count())
	{
	case 0:
		TR_WRN << "emty list";
		break;

	case 1:
		TR_MSG << "only 1 element";
		break;

	case 2:
		TR_MSG << "use id";
		break;

	case 3:
		TR_MSG << "use two ids";
		break;

	default:
		TR_ERR << "format error";
		break;
	}

	return 0x1000000000000000;
}

TrGeoObject * TrMapNet::manageGap(const TrZoomMap & zoom_ref, TrMapLink & link, uint8_t mode,
		const TrPoint & pt, TrGeoObject * obj)
{
	TR_MSG;

	switch(mode)
	{
	case TR_NET_GAP_ADD:		//0x01
		if((m_primive_map != nullptr) && (link.getPolygon() == nullptr))
		{
			uint64_t id = m_primive_map->createMapNextId();
			TrGeoPolygon * poly = new TrGeoPolygon;

			if(id != TR_NO_VALUE)
			{
				// TODO, check...
				//poly->setPolyId(id);
				m_primive_map->appendObject(poly, id);
				link.manageGap(zoom_ref, mode, pt, poly);
			}
		}
		break;

	case TR_NET_GAP_REMOVE:		//0x02
		if((m_primive_map != nullptr) && (link.getPolygon() != nullptr))
		{
			uint64_t id = m_primive_map->findObjectId(link.getPolygon());
			if(id != TR_NO_VALUE)
			{
				link.manageGap(zoom_ref, mode, pt);
				m_primive_map->deleteObject(id);
			}
		}
		break;

	case TR_NET_GAP_REPLACE:	//0x04
		TR_WRN << "TODO, do not use!";
		break;

	case TR_NET_GAP_UPDATE:
		link.manageGap(zoom_ref, mode, pt);
		break;

	default:
		TR_WRN << "unused code!" << mode;
		break;
	}
	return nullptr;
}

// TODO: use group parm
bool TrMapNet::addPen(const QString & group, int idx, const QPen & pen)
{
	//TR_INF << group << idx;
	//abort();
	if(group == "base")
	{
		if(m_primive_map != nullptr)
			m_primive_map->appendObjectPen(idx, pen);
	}
	else
	{
		if(m_link_list != nullptr)
			m_link_list->appendObjectPen(idx, pen);
	}
	return true;
}

bool TrMapNet::appendObjectBrush(int idx, QBrush brush)
{
	// TODO: define the brush if needed
	//TR_INF << idx;
	return true;
}

void TrMapNet::setMask(uint64_t bit_mask)
{
	TrGeoObject::setMask(bit_mask);

	//TR_MSG << "set" << m_name << HEX << m_inst_mask << bit_mask;

	m_inst_mask |= bit_mask;

	// TODO: s_mast vs. m_inst_mask!!!
	if((m_name == "rail") || (m_name == "stream"))
	{
		// the 'more lines' option is not suitable on 'rail' or 'stream' type
		m_inst_mask &= ~(TR_MASK_MORE_LINES);
	}

	if(m_link_list != nullptr)
	{
		m_link_list->setMask(bit_mask);
	}
	if(m_node_map != nullptr)
	{
		m_node_map->setMask(bit_mask | TR_MASK_DRAW);
	}
	if(m_primive_map != nullptr)
	{
		m_primive_map->setMask(bit_mask);
	}
}

void TrMapNet::removeMask(uint64_t bit_mask)
{
	TrGeoObject::removeMask(bit_mask);

	//TR_MSG << "rem" << m_name << HEX << m_inst_mask << bit_mask;

	m_inst_mask &= ~(bit_mask);

	if(m_node_map != nullptr)
	{
		m_node_map->removeMask(bit_mask /*| TR_MASK_DRAW*/);
	}
	if(m_primive_map != nullptr)
	{
		m_primive_map->removeMask(bit_mask);
	}
}

bool TrMapNet::createNodeInOut()
{
	// TODO: check number of connections

	if(m_node_map == nullptr)
	{
		TR_WRN << "node_map == nullptr";
		return false;
	}

	if(m_link_list == nullptr)
	{
		TR_WRN << "m_link_list == nullptr";
		return false;
	}

	QMap<uint64_t, TrGeoObject *> & pt_map = m_node_map->getMap();

	QMap<uint64_t, TrGeoObject *>::const_iterator ii = pt_map.constBegin();
	while (ii != pt_map.constEnd())
	{
		TrMapNode * n = dynamic_cast<TrMapNode *>(ii.value());
		if(n != nullptr)
			n->resetIoOut();
		++ii;
	}

	for (size_t i = 0; i < m_link_list->objCount(); ++i)
	{
		TrMapLink * link = dynamic_cast<TrMapLink *>(m_link_list->getVecObject(i));

		if(link != nullptr)
		{
			int64_t nd_from = link->getNodeFrom();
			TrMapNode * n_from = dynamic_cast<TrMapNode *>(m_node_map->getMapObject(nd_from));
			if(n_from != nullptr)
			{
				n_from->addConnection(link, TR_NODE_OUT);
			}
			int64_t nd_to = link->getNodeTo();
			TrMapNode * n_to = dynamic_cast<TrMapNode *>(m_node_map->getMapObject(nd_to));
			if(n_to != nullptr)
			{
				n_to->addConnection(link, TR_NODE_IN);
			}
		}
	}
        return true;
}

#ifdef TR_SERIALIZATION
uint64_t TrMapNet::readXmlDescription(QXmlStreamReader & xml_in)
{
	QXmlStreamAttributes attr = xml_in.attributes();
	m_name.clear();
	m_name.append(attr.value("", "name").toString());

	//TR_MSG << xml_in.name() << " " << m_name;

	xml_in.readNext();

	while(!xml_in.atEnd())
	{
		// TR_MSG << xml_in->name();
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
			if(xml_in.name().toString() == "map_list")
			{
				TrMapList * tmp_list = new TrMapList();

				tmp_list->readXmlDescription(xml_in);

				if(QString::compare(tmp_list->getObjClass(), "map_link") == 0)
				{
					m_link_list = tmp_list;
				}
				if(QString::compare(tmp_list->getObjClass(), "geo_point") == 0)
				{
					m_node_map = tmp_list;
					m_node_map->removeMask(TR_MASK_DRAW);
				}
				if(QString::compare(tmp_list->getObjClass(), "geo_primive") == 0)
				{
					m_primive_map = tmp_list;
				}
				if(QString::compare(tmp_list->getObjClass(), "map_complex") == 0)
				{
					m_complex_map = tmp_list;
				}
			}
			else
			{
				TR_ERR << "unknown element:" << xml_in.name();
			}
			xml_in.readNext();
		}
		else if(xml_in.isEndElement())
		{
			// TODO: NODE! node_map -> 22.05.2017 ???

			//link_list = tmp_list;

			// TODO: move code to init?
			if(m_link_list != nullptr)
			{
				for (size_t i = 0; i < m_link_list->objCount(); ++i)
				{
					TrMapLink * link = dynamic_cast<TrMapLink *>(m_link_list->getVecObject(i));
					if(link != nullptr)
					{
						if(link->getXmlName() == "map_link")
						{
							link->setPrimiveById(m_primive_map);
							link->reSetNodes(m_node_map);
						}
					}
					TrMapLinkRoad * link_rd = dynamic_cast<TrMapLinkRoad *>(m_link_list->getVecObject(i));
					if(link_rd != nullptr)
					{
						if(link_rd->getXmlName() == "map_link_road")
						{
							link_rd->setPrimiveById(m_primive_map);
							link_rd->reSetNodes(m_node_map);
						}
					}
#ifdef TR_EDGE
					TrMapEdge * edge = dynamic_cast<TrMapEdge *>(m_link_list->getVecObject(i));
					if(edge != nullptr)
					{
						if(edge->getXmlName() == "map_edge")
						{
							edge->setComplexById(m_complex_map);
							edge->reSetNodes(m_node_map);
						}
					}
#endif
				}
			}

			xml_in.readNext();

			//TR_MSG << "end";

			return 0;
		}
	}
	return TR_NO_VALUE;
}

void TrMapNet::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	//TR_MSG << m_complex_map << m_primive_map;

	xml_out.writeStartElement("map_net");

	xml_out.writeAttribute("name", QVariant(m_name).toString());

	if(m_link_list != nullptr)
	{
		m_link_list->writeXmlDescription(xml_out, 0);
	}
	else
	{
		//TR_WRN << "nullptr";
	}
	if(m_node_map != nullptr)
	{
		m_node_map->writeXmlDescription(xml_out, 0);
	}
	else
	{
		//TR_WRN << "nullptr";
	}
	if(m_primive_map != nullptr)
	{
		m_primive_map->writeXmlDescription(xml_out, 0);
	}
	else
	{
		//TR_WRN << "nullptr";
	}
	if(m_complex_map != nullptr)
	{
		TR_MSG << "complex_map";
		m_complex_map->writeXmlDescription(xml_out, 0);
	}
	else
	{
		//TR_WRN << "nullptr";
	}
	xml_out.writeEndElement();
}
#endif
