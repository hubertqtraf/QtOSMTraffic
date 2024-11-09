/******************************************************************
 *
 * @short	list/map class
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapList
 * superclass:	TrGeoObject
 * modul:	tr_map_list.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2021
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

#include "tr_map_list.h"

// TODO: remove??? -> create a new class like face_list...
//#ifdef TESTX
#include "tr_map_face.h"
// TODO: define without IO
#ifdef TESTX
#include "tr_factory.h"
#endif

#include <QtCore/qdebug.h>

#include <QtCore/qjsondocument.h>

TrMapList::TrMapList()
	: TrGeoObject()
	//, default_pen_idx(-1)
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
}

TrMapList::~TrMapList()
{
	this->clear();
}

QDebug operator<<(QDebug dbg, const TrMapList& list)
{
	return dbg << list.getXmlName() << "list:" << list.obj_list.size() <<
		"map:" << list.obj_map.size();
}

QString TrMapList::getName() const
{
	return m_name;
}

bool TrMapList::setName(const QString & name)
{
	m_name = name;
	return true;
}

QString TrMapList::getXmlName() const
{
	return "map_list";
}

size_t TrMapList::objCount()
{
    return static_cast<size_t>(obj_list.size());
}

size_t TrMapList::objCountMap()
{
    return static_cast<size_t>(obj_map.size());
}

TrGeoObject * TrMapList::getVecObject(size_t n)
{
    if((obj_list.size() < static_cast<int>(n)) || (static_cast<int>(n) < 0))
		return nullptr;
	return obj_list.at(n);
}

TrGeoObject * TrMapList::getMapObject(uint64_t id)
{
	if(obj_map.contains(id))
	{
		return obj_map[id];
	}
	return nullptr;
}

// TODO: id is not index! check maps on for loops: for(i=0;...
TrGeoObject * TrMapList::getObject(uint64_t id)
{
	if(obj_list.size() > 0)
		return getVecObject(id);
	return getMapObject(id);
}

TrGeoObject * TrMapList::getNextMapObject(uint64_t & id)
{
	TrMap::const_iterator ii;

	if(id == 0)
	{
		ii = obj_map.constBegin();
	}
	else
	{
		ii = obj_map.find(id);
		if(ii == obj_map.end())
		{
			TR_MSG << "not found";
			id = 0;
			return nullptr;
		}
		ii++;
	}
	if(ii == obj_map.end())
	{
		id = 0;
		return nullptr;
	}
	id = ii.key();
	return ii.value();
}

TrGeoObject * TrMapList::getNextMapObject(uint64_t & id, const TrZoomMap & zoom_ref)
{
	TrMap::const_iterator ii;

	if(id == 0)
	{
		ii = obj_map.constBegin();	
	}
	else
	{	
		ii = obj_map.find(id);
		if(ii == obj_map.end())
		{
			TR_MSG << "not found";
			id = 0;
			return nullptr;
		}
		ii++;
	}

	while (ii != obj_map.constEnd())
	{
		if(ii.value()->clip(zoom_ref) == false)
		{
			id = ii.key();
			return ii.value();
		}
		else
		{
			++ii;
		}
	}
	if(ii == obj_map.end())
	{
		id = 0;
		return nullptr;
	}
	else
	{
		id = ii.key();
		return ii.value();
	}
}

QMap<uint64_t, TrGeoObject *> & TrMapList::getMap()
{
	return obj_map;
}

uint64_t TrMapList::createMapNextId()
{
	uint64_t next_id = 0;

	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		//TR_MSG << "list A: " << ii.key() << (ii.key() - next_id);

		if((ii.key() - next_id) != 1)
		{
			//TR_MSG << "list B: " << ii.key() << (next_id + 1);
			return ++next_id;
		}
		next_id = ii.key();
	}
	//TR_MSG << "list C: " << (next_id + 1);
	return ++next_id;
}

uint64_t TrMapList::findObjectId(TrGeoObject * obj)
{
	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		if(ii.value() == obj)
			return ii.key();
	}
    for(int i = 0; i < obj_list.size(); ++i)
	{
		if(obj_list[i] == obj)
			return i;
	}
	return TR_NO_VALUE;
}

void TrMapList::setMask(uint64_t bit_mask)
{
	//TR_MSG << hex << bit_mask << mask;
	if(bit_mask & TR_MASK_DRAW)
	{
		m_inst_mask |= TR_MASK_DRAW;
	}
	m_inst_mask |= bit_mask;

	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		ii.value()->setMask(bit_mask);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		obj_list[i]->setMask(bit_mask);
	}
}

void TrMapList::removeMask(uint64_t bit_mask)
{
	//TR_MSG << hex << bit_mask << mask;
	if(bit_mask & TR_MASK_DRAW)
	{
		m_inst_mask &= ~(TR_MASK_DRAW);
	}
	m_inst_mask &= ~(bit_mask);

	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		ii.value()->removeMask(bit_mask);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		obj_list[i]->removeMask(bit_mask);
	}
}

void TrMapList::setLayerShowMask(uint64_t mask)
{
	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		ii.value()->setLayerShowMask(mask);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		obj_list[i]->setLayerShowMask(mask);
	}

	//TR_INF << HEX << mask;
}

void TrMapList::setActiveBrush(QBrush * brush)
{
	// TODO rework, type should be common -> class int in TrGeoObject -> see Link!

	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		// TODO use map?
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		TrMapFace * obj = dynamic_cast<TrMapFace*>(obj_list[i]);
		if(obj != nullptr)
		{
			uint16_t type = (obj->getType() & 0x00ff);
			QPen * pen = getObjectPen(type & 0x001f);
			if(pen != nullptr)
			{
				obj->setActiveBrush(getObjectBrush(type & 0x001f));
				obj->setActivePen(pen);
			}
			else
			{
				TR_WRN << "pen " << type << " is not in the list" << m_objPenMap.size();
			}
		}
	}
}



QString & TrMapList::getObjClass()
{
	return m_obj_class;
}

void TrMapList::setObjClass(const QString & oc)
{
	m_obj_class = oc;
}

void TrMapList::appendObject(TrGeoObject * list_obj)
{
	obj_list.append(list_obj);
}

bool TrMapList::appendObject(TrGeoObject * list_obj, uint64_t key)
{
	TrMap::const_iterator i = obj_map.find(key);

	if(i == obj_map.end())
	{
		obj_map[key] = list_obj;		
		return true;
	}

	return false;
}

bool TrMapList::deleteObject(const uint64_t key)
{
	if(obj_list.size())
	{
		//TR_MSG << key;
        if(key >= static_cast<uint64_t>(obj_list.size()))
			return false;
		obj_list.remove(key);
		return true;
	}
	if(obj_map.remove(key))
	{
		return true;
	}
	return false;
}

bool TrMapList::appendObjectPen(int idx, QPen pen)
{
	m_objPenMap[idx] = pen;
	if(!m_objPenMap.contains(idx))
		// append the pen
		return true;
	else
		// replace the pen
		return false;
}

int TrMapList::countPens()
{
	return m_objPenMap.size();
}

QPen * TrMapList::getObjectPen(int idx)
{
	//TR_MSG << idx;

	if(m_objPenMap.contains(idx))
	{
		return &(m_objPenMap[idx]);
	}
	return nullptr;
}

void TrMapList::setActivePen(QPen * pen)
{
	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		ii.value()->setActivePen(pen);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		obj_list[i]->setActivePen(pen);
	}
}

bool TrMapList::appendObjectBrush(int idx, QBrush brush)
{
	m_objBrushMap[idx] = brush;
	if(!m_objBrushMap.contains(idx))
		// append the brush
		return true;
	else
		// replace the brush
		return false;
}

QBrush * TrMapList::getObjectBrush(int idx)
{
	if(m_objBrushMap.contains(idx))
	{
		return &(m_objBrushMap[idx]);
	}
	return nullptr;
}

void TrMapList::setColorGroupx(const QString & group, QMap<int, QColor> & colors)
{
	// TODO: abstract function is not working, rework!
	//if(group != "base")
	if(group == "poi")
	{
		for(QMap<int, QColor>::const_iterator ii = colors.constBegin(); ii != colors.constEnd(); ++ii)
		{
			appendObjectPen(ii.key(), QPen(ii.value()));
		}
		TR_MSG << group << colors;
	}
}

bool TrMapList::addPen(const QString & group, int idx, const QPen & pen)
{
	//TR_MSG << group << idx;

	appendObjectPen(idx, pen);

	return true;
}

uint64_t TrMapList::findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos)
{
	if(obj_map.size())
	{
		TrMap::const_iterator ii;

		if(pos == TR_NO_VALUE)
		{
			ii = obj_map.constBegin();
		}
		else
		{
			if(!obj_map.contains(pos))
				return TR_NO_VALUE;
			ii = obj_map.find(pos);
			++ii;
		}
		while (ii != obj_map.constEnd())
		{
			if(TR_NO_VALUE != ii.value()->findSelect(zoom_ref, inside, pos))
			{
				//TR_MSG << "found" << ii.key();
				return (ii.key());
			}
			++ii;
		}
		return TR_NO_VALUE;
	}

	if(obj_list.size())
	{
		if(pos == TR_NO_VALUE)
		{
			pos = 0;
		}
		else
		{
			pos++;
		}
		if(pos > objCount())
			pos = 0;

		for (uint64_t i = pos; i < objCount(); ++i)
		{
			if(TR_NO_VALUE != obj_list[i]->findSelect(zoom_ref, inside, pos))
			{
				//TR_MSG << "found" << i;
				return i;
			}
		}
		return TR_NO_VALUE;
    }
	return TR_NO_VALUE;
}

bool TrMapList::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	// if base is 'nullptr' -> use member data/colour list
	if(base == nullptr)
		base = this;
	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		ii.value()->init(zoom_ref, ctrl, base);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
        obj_list[i]->init(zoom_ref, ctrl, this);
	}
	// TODO: rework remove?
	setActiveBrush(nullptr);
	return true;
}

void TrMapList::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	if(m_inst_mask & TR_MASK_DRAW)
	{
		for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
		{
			ii.value()->draw(zoom_ref, p, mode);
		}
		for (int i = 0; i < obj_list.size(); ++i)
		{
            obj_list[i]->draw(zoom_ref, p, mode);
		}
	}
}


bool TrMapList::setSurroundingRect()
{
	//TR_MSG << obj_list.size() << " - " << obj_map.count();

	if(obj_list.size() > 0)
	{
		bool first = true;
		for (int i = 0; i < obj_list.size(); ++i)
		{
			if(obj_list[i] != nullptr)
			{
				if(obj_list[i]->setSurroundingRect())
				{
					updateSurroundRect(*(obj_list[i]), first);
					if(first)
						first = false;
				}
			}
		}
		// needs at least one element with surround rectangle
		if(!first)
		{
			return true;
		}
	}

	if(obj_map.count() > 0)
	{
		bool first = true;

		for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
		{
			if(ii.value() != nullptr)
			{
				if(ii.value()->setSurroundingRect())
				{
					updateSurroundRect(*(ii.value()), first);

					if(first)
						first = false;
				}
			}
		}
		// needs at least one element with surround rectangle
		if(!first)
		{
			return true;
		}
	}
    //TR_MSG << "return false" << obj_list.size() << obj_map.count() <<
    //    getSurroundRectVal(0) << getSurroundRectVal(1) << getSurroundRectVal(2) << getSurroundRectVal(3);
	return false;
}

void TrMapList::clear()
{
	// TODO: clear the 'class' name?
    /*if(obj_map.count())
	{
		for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
		{
            // TODO: fix the crash -> tree times reloding -> memmory leak!
			delete ii.value();
		}
	}
	else
		TR_MSG << "emty list";

	for (int i = 0; i < obj_list.size(); ++i)
	{
		delete obj_list.at(i);		// TODO: clear?
    }*/
    obj_map.clear();
    obj_list.clear();
}

#ifdef TR_SERIALIZATION
bool TrMapList::importGeoJson(const QJsonObject & geojson, uint64_t mode)
{
	TR_MSG;

	QJsonArray featureArray = geojson["features"].toArray();

	QJsonDocument doc(geojson);

	TR_MSG << objCount();

	for(int i = 0; i < featureArray.size(); ++i)
	{
		QJsonObject featureObject = featureArray[i].toObject();
		if(featureObject.contains("type"))
		{
			//TR_MSG << featureObject.value("type").toString();
			if(featureObject.value("type").toString() != "Feature")
				return false;
		}
		if(featureObject.contains("geometry"))
		{
			QJsonObject geoObject = featureObject["geometry"].toObject();
			if(geoObject.contains("type"))
			{
				// split the polygons here as there is not a 'MultiPolygon' class existing
				// there is not infomation about class and type, all elements
				// have same the class and type inforation...
				if(geoObject.value("type").toString() == "MultiPolygon")
				{
					// TODO: check array[array]
					QJsonArray coordinatesArray = geoObject["coordinates"].toArray();
					for(int j = 0; j < coordinatesArray.size(); ++j)
					{
						QJsonArray testArray = coordinatesArray[j].toArray();
						for(int k = 0; k < testArray.size(); ++k)
						{
//#ifdef TESTX
							TrMapFace * face = new TrMapFace;
							//face->importGeoJson(coordinatesArray[j], 1);
							face->importArrayJson(testArray[k].toArray(), 1);
							appendObject(face);
//#endif
						}
					}
				}
				// use the 'LineString' element as standard face element with class and type
				if(geoObject.value("type").toString() == "LineString")
				{
//#ifdef TESTX
					TrMapFace * face = new TrMapFace;
					face->importGeoJson(featureObject, 0);
					appendObject(face);
//#endif
				}
			}
		}
	}
	setSurroundingRect();
	return true;
}

bool TrMapList::exportGeoJson(QJsonObject & geojson, uint64_t mode)
{
	//TR_MSG;

	QJsonArray tr_objects;

	for(TrMap::const_iterator ii = obj_map.constBegin(); ii !=  obj_map.constEnd(); ++ii)
	{
		QJsonObject tr_feature;
		ii.value()->exportGeoJson(tr_feature, mode);
		QJsonValue tr_value(tr_feature);
		tr_objects.append(tr_value);
	}
	for (int i = 0; i < obj_list.size(); ++i)
	{
		QJsonObject tr_feature;
		obj_list[i]->exportGeoJson(tr_feature, mode);
		QJsonValue tr_value(tr_feature);
		tr_objects.append(tr_value);
	}
	geojson["features"] = tr_objects;

	return true;
}

uint64_t TrMapList::readXmlDescription(QXmlStreamReader & xml_in)
{
	//int64_t id_check = TR_NO_VALUE;
#ifdef TESTX
	QXmlStreamAttributes attr = xml_in.attributes();

	QStringRef c_class = attr.value("", "class"); //.toInt();

	if(!c_class.isEmpty())
	{
		m_obj_class = c_class.toString();
	}

	uint64_t x = 0;
	x = attr.value("", "id").toInt();
	//TR_MSG << x;

	while(!xml_in.atEnd())
	{
		xml_in.readNext();

		QStringRef ref;
		if(readDefStartElement(xml_in, ref))
		{
			uint64_t id = TR_NO_VALUE; 

			TrGeoObject * obj = TrFactory::create(ref, xml_in, id);

			if(obj != nullptr)
			{
				if(id == TR_NO_VALUE)
				{
					appendObject(obj);
				}
				else
				{
					if(appendObject(obj, id) == false)
					{
						TR_ERR << "append error";
					}
				}
			}
			else
			{
				TR_ERR << "factory error: " << xml_in.name();
			}
		}
		else if(xml_in.isEndElement())
		{
			//return 0;
			return x;
                }
        }
#endif
        return TR_NO_VALUE;
}

void TrMapList::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	// TR_MSG << "writeXmlDescription";	

	if(obj_map.size() != 0)
	{
		xml_out.writeStartElement(getXmlName()); //"map_list");

		if(m_obj_class.length())
		{
			xml_out.writeAttribute("class", m_obj_class);
			xml_out.writeAttribute("key", "long");
		}

		TrMap::const_iterator i = obj_map.constBegin();

		while (i != obj_map.constEnd())
		{
			if(i.value() == nullptr)
			{
				TR_ERR << "value() == nullptr";
				return;
			}
			i.value()->writeXmlDescription(xml_out, i.key());
			++i;
		}
		xml_out.writeEndElement();
	}
	if(obj_list.size() != 0)
	{
		xml_out.writeStartElement(getXmlName()); //"map_list");

		xml_out.writeAttribute("n", QVariant(obj_list.size()).toString());
		if(m_obj_class.length())
		{
			xml_out.writeAttribute("class", m_obj_class);
		}
		if(id != 0)
		{
			xml_out.writeAttribute("id", QVariant((int)id).toString());
		}
		for (int i = 0; i < obj_list.size(); ++i)
		{
			obj_list[i]->writeXmlDescription(xml_out, 0);	
		}
		xml_out.writeEndElement();
	}
}
#endif

