/******************************************************************
 *
 * @short	Stack to control the layers
 *
 * project:	Trafalgar lib
 *
 * class:	TrStack
 * superclass:	TrGeoObject
 * modul:	tr_layer.cc
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

#include "tr_stack.h"


#include <QtCore/qdebug.h>


TrStack::TrStack()
	: TrGeoObject()
{
	surroundingRect[0] = surroundingRect[1] = surroundingRect[2] = surroundingRect[3] = 0;
}

TrStack::~TrStack()
{
	this->clear("");
}


void TrStack::clear()
{
	TR_WRN << "check";
}

void TrStack::clear(const QString & name)
{
	// delete layer elements
	if(name == "")
	{
		QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();
		while (ii != m_layerMap.constEnd())
		{
			delete ii.value();
			++ii;
		}
		m_layerMap.clear();

        m_order.clear();
		return;
	}

	QMap<QString, TrLayer*>::iterator ii = m_layerMap.find(name);
	if(ii != m_layerMap.constEnd())
	{
		TrGeoObject * obj = ii.value()->getElement();

		QMap<QString, QVector<TrGeoObject *>> tmp_order = m_order;
		m_order.clear();

		TR_MSG << "old order: " << tmp_order;

		QMap<QString, QVector<TrGeoObject *>>::const_iterator ij = tmp_order.constBegin();
		while (ij != tmp_order.constEnd())
		{
			QVector<TrGeoObject *> v = ij.value();
			int n = v.size();
			for (int i = 0; i < v.size(); ++i)
			{
				if(obj == v[i])
					n = i;
			}
			if(n < v.size())
			{
				v.remove(n);
			}
			m_order[ij.key()] = v;
			++ij;
		}
		TR_MSG << "new order: " << m_order;
		delete ii.value();
		m_layerMap.erase(ii);
	}
}

void TrStack::printData()
{
	TR_MSG << m_layerMap;
	TR_MSG << m_order;
}	

// TODO: mismach m_order <> m_layerMap?
QVector<QString> TrStack::getLayerList(const QString & type)
{
	QVector<QString> test;

	if(m_order.contains(type))
	{
		QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();

		while (ii != m_layerMap.constEnd())
		{
			test.append(ii.key());
			++ii;
		}
		//QVector<TrGeoObject *> vec_obj = m_order[type];
	}
	//TR_MSG << type << test;
	return test;
}

bool TrStack::addOrderType(const QString & type)
{
	if(!m_order.contains(type))
	{
		QVector<TrGeoObject *> order_list;

		m_order[type] = order_list;
		return true;
	}
	return false;
}	

bool TrStack::resetOrderList(const QString & type)
{
	// TODO
	return false;
}

bool TrStack::appendOrderElement(const QString & type, const QString & name)
{
	if(!m_order.contains(type))
	{
		TR_WRN << "type not " << type << "(" << name << ")";
		return false;
	}
	if(!m_layerMap.contains(name))
	{
		TR_WRN << "name not " << name;
		return false;
	}
	TrGeoObject * obj = m_layerMap[name]->getElement();	

	m_order[type].append(obj);

	return true;
}

bool TrStack::addLayer(const QString & name, TrGeoObject * layer)
{
	TrLayer * new_layer = new TrLayer();
	new_layer->setElement(layer);

	// TODO: check name/layer
	m_layerMap.insert(name, new_layer);

	//edit_objects[name] = layer;

	// TODO: use paint order
	//m_paintList.push_back(layer);

	if(layer != nullptr)
	{
		layer->setName(name);
	}

	return true;
}

TrGeoObject * TrStack::getMapObject(const QString & name)
{
	if(!m_layerMap.contains(name))
		return nullptr;
	return m_layerMap[name]->getElement();
}

void TrStack::appendListMembers(QStringList & list)
{
	QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();

	while (ii != m_layerMap.constEnd())
	{
		list.append(ii.key());
		++ii;
	}
}

const QStringList TrStack::getTypeStrings(const QString & type) const
{
	QStringList list;

	if(!m_order.contains(type))
	{
		TR_ERR << "type error " << type;
		return list;
	}

	for (int i = 0; i < m_order[type].size(); ++i)
	{
		TrGeoObject * obj = m_order[type].at(i);
		if(obj == nullptr)
		{
			list.append("emty");
		}
		else
		{
			if(obj->getName() == "")
			{
				list.append("unknown");
			}
			else
			{
				list.append(obj->getName());
			}
		}
	}
	return list;
}

void TrStack::setFunctionOrder(const QStringList func, const QString & type)
{
	if(!m_order.contains(type))
	{
		TR_ERR << "type error " << type;
		return;
	}

	m_order[type].clear();

	for (int i = 0; i < func.size(); ++i)
	{
		if(m_layerMap.contains(func[i]))
		{
			TrLayer * layer = m_layerMap[func[i]];
			if(layer != nullptr)
			{
				if(layer->getElement() != nullptr)
				{
					m_order[type].append(layer->getElement());
					// TODO: double filter -> tr_view, remove if tr_view is changed
					if(type == "draw")
					{
						layer->getElement()->setMask(TR_MASK_DRAW);
					}
				}
			}
		}
	}
}

bool TrStack::checkLayerName(const QString & name)
{
	return m_layerMap.contains(name);
}

const QString TrStack::findFirstExt()
{
	QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();
	while (ii != m_layerMap.constEnd())
	{
		TrLayer * act = ii.value();
		
		TR_MSG << ii.key() << " " << act->getFlags();
		
		if((act->getFlags() & 0xff000000) == TR_LAYER_FIRST) 
			return ii.key();
		++ii;
	}
	return "";
}
        
void TrStack::setFirstExt(const QString & name)
{
	QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();
	while (ii != m_layerMap.constEnd())
	{
		if(ii.key() == name)
		{
			TR_MSG << "set first true, " << name;
			ii.value()->setActExt(true);
		}
		else
		{
			ii.value()->setActExt(false);
		}
		++ii;
	}
}

bool TrStack::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
    QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();
	while (ii != m_layerMap.constEnd())
	{
		// TR_MSG << ii.key();
		TrLayer * act = ii.value();

		if(act != nullptr)
		{
			if(act->getElement() != nullptr)
				act->getElement()->init(zoom_ref, ctrl);
		}
		++ii;
	}
	return true;
}

void TrStack::setNameList(TrGeoObject * name_list)
{
	QMap<QString, TrLayer*>::const_iterator ii = m_layerMap.constBegin();
	while (ii != m_layerMap.constEnd())
	{
		TrLayer * act = ii.value();

		if(act != nullptr)
		{
			if(act->getElement() != nullptr)
				act->getElement()->setNameList(name_list);
		}
		++ii;
	}
}

bool TrStack::setSurroundingRect()
{
	bool first = true;

	bool ret = false;
	// TODO: check -> replaced?
	/*surroundingRect[0] = 1.0;
	surroundingRect[1] = 2.0;
	surroundingRect[2] = 3.0;
	surroundingRect[3] = 4.0;*/

	// TR_MSG << "check layer/order" << " " << m_layerMap << " " << m_order;

	if(!m_order.contains("resize"))
	{
		TR_WRN << "m_order has no 'resize' element";
		return false;
	}
	if(m_order["resize"].size() < 1)
	{
		TR_WRN << "size: " << m_order["resize"].size();
		return false;
	}

	for (int i = 0; i < m_order["resize"].size(); ++i)
	{
		TrGeoObject * obj = m_order["resize"].at(i);

		if(obj == nullptr)
		{
			TR_WRN << "null ptr";
			return false;
		}

		if(obj->setSurroundingRect() == true)
		{
			if(first)
			{
				updateSurroundRect(*obj, true);
				first = false;
			}
			else
			{
				updateSurroundRect(*obj, false);
			}
			ret = true;
		}
		else
		{
			TR_MSG << "emty list";
		}
	}

	TR_MSG << "stack " << QString::number(surroundingRect[0],'f', 2) <<
		QString::number(surroundingRect[1],'f', 2) <<
		QString::number(surroundingRect[2],'f', 2) <<
		QString::number(surroundingRect[3],'f', 2) << 
		"edit_objects" << m_order["resize"].size();

	return ret;
}

QVector<double> TrStack::getSurroundingVecRect()
{
	QVector<double> rect;
	rect.append(getSurroundRectVal(0));
	rect.append(getSurroundRectVal(1));
	rect.append(getSurroundRectVal(2));
	rect.append(getSurroundRectVal(3));
	return rect;
}

void TrStack::setSurroundingVecRect(const QVector<double> & rect)
{
	if(rect.size() != 4)
	{
		TR_ERR << "rect.size != 4";
		return;
	}
	double val[4];
	val[0] = rect[0];
	val[1] = rect[1];
	val[2] = rect[2];
	val[3] = rect[3];
	updateSurroundRect(val, true);
}

void TrStack::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	//TR_MSG << "mode: " << mode;
	//printData();

	if(!m_order.contains("draw"))
	{
		TR_WRN << "missing layer with 'draw'";
		return;
    }

	for (int i = 0; i < m_order["draw"].size(); ++i)
    {
		TrGeoObject * obj = m_order["draw"].at(i);

		if(obj == nullptr)
		{
			TR_WRN << "null ptr";
			return;
		}
		else
		{
			obj->draw(zoom_ref, p, mode);
		}
	}
}

void TrStack::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	if(!m_order.contains("write"))
	{
		TR_WRN << "missing layer with 'write': " << m_order;
		return;
	}

	for (int i = 0; i < m_order["write"].size(); ++i)
	{
		TrGeoObject * obj = m_order["write"].at(i);

		if(obj == nullptr)
		{
			TR_WRN << "null ptr";
			return;
		}
		else
		{
			obj->writeXmlDescription(xml_out, 0);
		}
	}
}

