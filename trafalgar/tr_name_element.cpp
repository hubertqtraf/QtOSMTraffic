/******************************************************************
 *
 * @short	Name Map TODO: to implement/use
 *
 * project:	Trafalgar lib
 *
 * class:	TrNameList
 * superclass:	TrGeoObject
 * modul:	tr_name_list.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	03.2022
 *
 * @author	Schmid Hubert (C)2022 - 2023
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

#include "tr_name_element.h"


#include <QtCore/qdebug.h>

#include <math.h>

TrNameElement::TrNameElement()
	: TrGeoObject()
	, m_flags(0)
	, m_number(0)
{
}

TrNameElement::~TrNameElement()
{
}

QString TrNameElement::getXmlName() const
{
	return "name";
}

bool TrNameElement::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	return false;
}

void TrNameElement::drawOnPolygon(QPainter * p, const QPolygon & poly)
{
    QFont font = p->font();
    QFontMetrics fm(font);
    // angle could be negative
    double deg = 180.0 / M_PI;

    for(int i=1; i< poly.size(); i++)
    {
        if(i == 2)
            return;
        double dx = poly.at(i-1).x() - poly.at(i).x();
        double dy = poly.at(i-1).y() - poly.at(i).y();
        double angle = atan2(dy, dx) * deg;
        if(angle < 0.0)
                angle += 360.0;
        bool ret = false;
        if((angle > 90.0) && (angle <= 180.0))
        {
            angle += 180.0;
            ret = true;
        }
        if((angle > 180.0) && ((angle <= 270.0)))
        {
            angle -= 180.0;
            ret = true;
        }
        p->save();
        p->translate(static_cast <int>(poly.at(i).x()), static_cast <int>(poly.at(i).y()));
        p->rotate(angle);
        QRect rect = fm.boundingRect(m_name);
        int dt = abs(static_cast<int>(dy));
        if(abs(dx) > abs(dy))
                dt = abs(static_cast<int>(dx));
        if((rect.width() + rect.height()) < dt)
        {
            if(!ret)
                p->drawText(rect.height(), -3, m_name);
            else
                p->drawText(-(rect.width() + rect.height()), (rect.height()/2)+3, m_name);
        }
        p->restore();
    }
}

bool TrNameElement::setName(const QString & name)
{
	m_name = name;
	return true;
}

QString TrNameElement::getName() const
{
	return m_name;
}

QString & TrNameElement::getNameRef()
{
	return m_name;
}

void TrNameElement::setNameNumber(uint32_t number)
{
	m_number = number;
}

void TrNameElement::increaseNumber()
{
	m_number++;
}

bool TrNameElement::decreaseNumber()
{
	if(m_number == 0)
	{
		TR_ERR << "decreasing";
		return false;
	}
	m_number--;
	if(m_number == 0)
		return false;
        return true;
}

bool TrNameElement::setSurroundingRect()
{
	bool ret = false;

	return ret;
}

#ifdef TR_SERIALIZATION
uint64_t TrNameElement::readXmlDescription(QXmlStreamReader & xml_in)
{
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
			if(xml_in.name() == "name")
			{
				QXmlStreamAttributes attr = xml_in.attributes();
				m_name = attr.value("", "name").toString();
				m_number = attr.value("", "number").toLong();
				//TR_MSG << "name:" << m_name << " " << attr.value("", "id").toLong();
				xml_in.readNext();
				return attr.value("", "id").toLong();
			}
		}
		xml_in.readNext();
	}
	return TR_NO_VALUE;
}

void TrNameElement::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	xml_out.writeStartElement(getXmlName());

	xml_out.writeAttribute("id", QVariant((qulonglong)id).toString());

	xml_out.writeAttribute("name", QVariant(m_name).toString());

	xml_out.writeAttribute("number", QVariant((qulonglong)m_number).toString());

	xml_out.writeEndElement();
}
#endif


