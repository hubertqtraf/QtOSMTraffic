/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrDocument
 * superclass:	TrGeoObject
 * modul:	tr_document.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * (C)		Schmid Hubert 2012-2022
 *
 * history:
 *  file: tr_map_world -> tr_document
 *  dir:  view -> lib/gui
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



#include "tr_document.h"
#include <tr_map_net.h>
#include <tr_map_net_road.h>
#include <tr_name_element.h>

#include <QtCore/qfile.h>

TrDocument::TrDocument(const QString & name, QObject * parent)
	: QObject(parent)
	, TrGeoObject()
	, m_name("")
	, m_fname(name)
	, m_is_dirty(false)
	, m_is_loaded(false)
{
	// TODO check
	m_name = QString("world");
	// TODO: add to layer?
	m_name_map.setObjClass("name");
}

TrDocument::TrDocument(QObject *parent)
	: QObject(parent)
	, TrGeoObject()
	, m_name("")
	, m_fname("")
	, m_is_dirty(false)
	, m_is_loaded(false)
{
	m_name = QString("world");
	m_name_map.setObjClass("name");
}

TrDocument::~TrDocument()
{
}

void TrDocument::clean()
{
	m_name = "";
	m_fname = "";
	m_map_stack.clear("");
	resetNameList();
	m_is_loaded = false;
	surroundingRect[0] = surroundingRect[1] = surroundingRect[2] = surroundingRect[3] = 0.0;
}

QString TrDocument::getName() const
{
	return m_name;
}

bool TrDocument::setName(const QString & name)
{
	m_name = name;
	return true;
}

const QString & TrDocument::getFileName()
{
	return m_fname;
}

void TrDocument::setFileName(const QString & fname)
{
	m_fname = fname;
}

uint64_t TrDocument::getSettings() const
{
	return TrGeoObject::s_mask; //m_settings;
}

const QStringList TrDocument::getLayerNames()
{
	// TODO: 'draw' not neeted -> fix it
	QVector<QString> list = m_map_stack.getLayerList("draw");
	// TODO: change QVector<QString> to QStringList, remove copy loop
	QStringList slist;
	for (int i = 0; i < list.size(); ++i)
		slist.append(list[i]);
	return slist;
}

void TrDocument::setSelectionLayer(const QString & layer)
{
	m_selection_layer = layer;
}

const QString & TrDocument::getSelectionLayer() const
{
	return m_selection_layer;
}

// TODO: remove -> layer?
TrMapList & TrDocument::getNameList()
{
	return m_name_map;
}

void TrDocument::resetNameList()
{
	m_name_map.clear();
}

bool TrDocument::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	m_map_stack.setNameList(&m_name_map);
	return m_map_stack.init(zoom_ref, ctrl);
}

bool TrDocument::setLayerItemData(const QMap<QString, uint64_t> & layers)
{
	for (auto i = layers.cbegin(), end = layers.cend(); i != end; ++i)
	{
		TrGeoObject * obj = m_map_stack.getMapObject(i.key());
		if(obj != nullptr)
		{
			obj->setLayerShowMask(i.value());
		}
	}
	return true;
}

void TrDocument::createEmtyLayers(const QStringList & layer_names)
{
	TR_MSG << layer_names;
	for (int i = 0; i < layer_names.size(); ++i)
	{
		TrMapNet * net = new TrMapNet();
		net->setName(layer_names[i]);

		net->init();
		m_map_stack.addLayer(layer_names[i], net);
	}
}

void TrDocument::appendListMembers(QStringList & list)
{
	m_map_stack.appendListMembers(list);
	//TR_MSG << list;
}

void TrDocument::addLayerType(const QList<QString> & type_list)
{
	for (int i = 0; i < type_list.size(); ++i)
	{
		m_map_stack.addOrderType(type_list[i]);
	}
}

// using on import
void TrDocument::addMapLayerObjectByName(const QString layer_name, TrGeoObject * obj)
{
	m_map_stack.addLayer(layer_name, obj);
}

void TrDocument::addOrderByType(const QString & type, const QList<QString> & layer_list)
{
	for (int i = 0; i < layer_list.size(); ++i)
	{
		m_map_stack.appendOrderElement(type, layer_list[i]);
	}
}

void TrDocument::setFunctionOrder(const QStringList func, const QString & type)
{
	m_map_stack.setFunctionOrder(func, type);
}

TrGeoObject * TrDocument::getLayerObjectByName(const QString & name)
{
	return m_map_stack.getMapObject(name);
}

TrGeoObject * TrDocument::getLayerObjectBySelection()
{
	return m_map_stack.getMapObject(m_selection_layer);
}

uint64_t TrDocument::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	// select network object, like 'road, rail...'
	TrGeoObject * obj = m_map_stack.getMapObject(m_selection_layer);

	if(obj != nullptr)
		return(obj->findSelect(zoom_ref, inside, pos));

	return TR_NO_VALUE;
}

uint64_t TrDocument::editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids)
{
	// select network object, like 'road, rail...'
	TrGeoObject * obj = m_map_stack.getMapObject(m_selection_layer);

	if(obj != nullptr)
		return(obj->editElement(zoom_ref, set, ids));

	return TR_NO_VALUE;
}

bool TrDocument::save(const QString & filename)
{
	TR_MSG << filename;

	QFile saveFile(filename);

	if(saveFile.open(QIODevice::ReadWrite|QIODevice::Truncate)==false)
		return true;

	QXmlStreamWriter doc(&saveFile);
	doc.setAutoFormatting(true);
	doc.writeStartDocument("1.0");
	doc.writeStartElement("trafalgar");
	doc.writeAttribute("doc","road_description");
	doc.writeAttribute("app","tr_view");
	doc.writeAttribute("version","0.2");
	doc.writeAttribute("protocol","4.0");

#ifdef TR_SERIALIZATION
	writeXmlDescription(doc, 0);
#endif

	doc.writeEndElement();		// cont
	doc.writeEndElement();		// trafalgar
	saveFile.close();
	return true;
}

bool TrDocument::setSurroundingRect()
{
	if(m_map_stack.setSurroundingRect() == false)
	{
		if(m_rect.size() == 4)
		{
			surroundingRect[0] = m_rect[0];
			surroundingRect[1] = m_rect[1];
			surroundingRect[2] = m_rect[2];
			surroundingRect[3] = m_rect[3];
			TR_MSG << m_rect;
			return true;
		}
		return false;
	}
    surroundingRect[0] = m_map_stack.getSurroundRectVal(0);
    surroundingRect[1] = m_map_stack.getSurroundRectVal(1);
    surroundingRect[2] = m_map_stack.getSurroundRectVal(2);
    surroundingRect[3] = m_map_stack.getSurroundRectVal(3);

	return true;
}

QVector<double> TrDocument::getSurroundingVecRect()
{
	return m_map_stack.getSurroundingVecRect();
}

void TrDocument::setSurroundingVecRect(const QVector<double> & rect)
{
	m_rect = rect;
	m_map_stack.setSurroundingVecRect(rect);
}

void TrDocument::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
    m_map_stack.draw(zoom_ref, p, mode);
}

int TrDocument::checkFileHeader(const QXmlStreamAttributes & attrs)
{
	QStringView doc_type = attrs.value("", "doc");

	if(doc_type.toString() == "road_description")
	{
		// TODO version check...
		return 0;
	}
	if(doc_type.toString() == "profile")
	{
		return 1;
	}
	return -1;
}

#ifdef TR_SERIALIZATION
uint64_t TrDocument::readXmlDescription(QXmlStreamReader & xml_in)
{
	xml_in.readNext();

	int progress = 0;

	while(!xml_in.atEnd())
	{
		if(xml_in.isComment())
		{
			TR_WRN << xml_in.tokenString() << ": " << xml_in.text();
		}
		else if(xml_in.isCharacters())
		{
		}
		else if(xml_in.isStartElement())
		{
			QXmlStreamAttributes attr = xml_in.attributes();

			//TR_MSG << xml_in.name() << attr.value("", "name");

			progress += 9;
			emit valueChanged(progress);

			TR_MSG << xml_in.name();

			if(xml_in.name() == "map_net")
			{
				QString layerName = attr.value("", "name").toString();

				if(m_map_stack.checkLayerName(layerName))
				{
					TR_ERR << "layer error: [" << layerName << "]";
				}
				else
				{
					// TODO: create a factory for net objects?
					TrMapNet * net = nullptr;
					if(layerName == "road")
					{
						TrMapNetRoad * net_road = new TrMapNetRoad;
						net = dynamic_cast<TrMapNet *>(net_road);
					}
					else
						net = new TrMapNet;

					if(net != nullptr)
					{
						if(net->readXmlDescription(xml_in) != 0)
						{
							TR_ERR << "XML error";
							return TR_NO_VALUE;
						}
						else
						{
							m_map_stack.addLayer(layerName, net);
							if(progress <= 80)
								progress += 20;
							emit valueChanged(progress);
						}
					}
				}
			}
			if(xml_in.name() == "map_list")
			{
				TrMapList * named_list = new TrMapList();

				if(named_list->readXmlDescription(xml_in) != 0)
				{
					TR_ERR << "XML error";
                    return TR_NO_VALUE;
				}

				// TODO: remove 'm_name_map' object, use layer...
				if(QString::compare(named_list->getObjClass(), "name") == 0)
				{
					uint64_t id = 0;
					TrGeoObject * obj = nullptr;
					// copy the list
					while((obj = named_list->getNextMapObject(id)) != nullptr)
					{
						TrNameElement * elem = dynamic_cast<TrNameElement *>(obj);
						if(elem != nullptr)
						{
							//TR_INF << "name:" << id << elem->getName();
							m_name_map.appendObject(elem, id);
						}
					}
				}

				if(QString::compare(named_list->getObjClass(), "poi") == 0)
				{
					m_map_stack.addLayer("poi", named_list);
				}
			}
		}
		else if(xml_in.isEndElement())
		{
			if(xml_in.name() == "map_world")
			{
				TR_INF << "end map_world";
				xml_in.readNext();
				return 0;
			}
			else if(xml_in.name() == "map_net")
			{
			}
			else if(xml_in.name() == "map_list")
			{
			}
		}
		xml_in.readNext();
	}
    return TR_NO_VALUE;
}

void TrDocument::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement("map_world");

	m_map_stack.writeXmlDescription(xml_out, id);
	m_name_map.writeXmlDescription(xml_out, id);

	xml_out.writeEndElement();
}
#endif

