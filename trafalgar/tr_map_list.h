/******************************************************************
 *
 * @short	list/map class
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapList
 * superclass:	TrGeoObject
 * modul:	tr_map_list.h
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



// container for other objects -> nature...

#ifndef TR_MAP_LIST_H
#define TR_MAP_LIST_H

#include "tr_geo_object.h"

#include <stdint.h>

#include <QtCore/qmap.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>

class TrMapList : public TrGeoObject
{
private:
	typedef QMap<uint64_t, TrGeoObject *> TrMap;
	typedef QVector<TrGeoObject *> TrList;

	QString m_name;

	TrMap obj_map;

	TrList obj_list;

	QMap<int, QPen> m_objPenMap;

	QMap<int, QBrush> m_objBrushMap;

	QString m_obj_class;

public:
	TrMapList();

	virtual ~TrMapList();

	friend QDebug operator<<(QDebug dbg, const TrMapList& list);

	virtual QString getName() const;

	virtual bool setName(const QString & name);

	virtual QString getXmlName() const;

	void clear();

	size_t objCount();

	size_t objCountMap();

	TrGeoObject * getVecObject(size_t n);

	TrGeoObject * getMapObject(uint64_t id);

	TrGeoObject * getObject(uint64_t id);

	TrGeoObject * getNextMapObject(uint64_t & id);

	TrGeoObject * getNextMapObject(uint64_t & id, const TrZoomMap & zoom_ref);

	QMap<uint64_t, TrGeoObject *> & getMap();

	uint64_t findObjectId(TrGeoObject * obj);

	uint64_t createMapNextId();

	virtual void setMask(uint64_t bit_mask);

	virtual void removeMask(uint64_t bit_mask);

	virtual void setLayerShowMask(uint64_t mask);

	virtual void setActiveBrush(QBrush * brush);

	QString & getObjClass();

	void setObjClass(const QString & oc);

	void appendObject(TrGeoObject * list_obj);

	bool appendObject(TrGeoObject * list_obj, uint64_t key);

	bool deleteObject(const uint64_t key);

	bool addPen(const QString & group, int idx, const QPen & pen);

	bool appendObjectPen(int idx, QPen pen);

	virtual void setColorGroupx(const QString & group, QMap<int, QColor> & colors);

	QPen * getObjectPen(int idx);

	int countPens();

	virtual void setActivePen(QPen * pen);

	bool appendObjectBrush(int idx, QBrush brush);

	QBrush * getObjectBrush(int idx);

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode);

	bool setSurroundingRect();
#ifdef TR_SERIALIZATION
	virtual bool exportGeoJson(QJsonObject & geojson, uint64_t mode);

	virtual bool importGeoJson(const QJsonObject & geojson, uint64_t mode);

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif	// TR_MAP_LIST_H

