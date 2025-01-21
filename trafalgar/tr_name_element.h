/******************************************************************
 *
 * @short	Name Map TODO: to implement/use
 *
 * project:	Trafalgar lib
 *
 * class:	TrNameList
 * superclass:	TrGeoObject
 * modul:	tr_name_list.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	03.2022
 *
 * @author	Schmid Hubert (C)2022 - 2025
 *
 * history:
 *
 ******************************************************************/

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

#ifndef TR_NAME_ELEMENT_H
#define TR_NAME_ELEMENT_H

#include <stdint.h>

#include "tr_geo_object.h"

class TrNameElement : public TrGeoObject
{
private:
	//QMap<QString, uint64_t> m_name_list;
	//TrMapList * m_name_map;

	uint32_t m_flags;
	uint32_t m_number;
	QString m_name;

public:
	TrNameElement();
	virtual ~TrNameElement();

	QString getXmlName() const;

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	void drawOnPolygon(QPainter * p, const QPolygon &poly);

	bool setName(const QString & name);

	QString getName() const;

	QString & getNameRef();

	void setNameNumber(uint32_t number);

	void increaseNumber();

	bool decreaseNumber();

	virtual bool setSurroundingRect();
#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif

};

#endif // TR_NAME_ELEMENT_H

