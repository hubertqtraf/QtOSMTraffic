/******************************************************************
 *
 * @short	face class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrMapFace
 * superclass:	TrGeoObject
 * modul:	tr_map_face.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * @author	Schmid Hubert (C)2012-2024
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



// area -> polygon + attributes

#ifndef TR_MAP_FACE_H
#define TR_MAP_FACE_H

// use 'QPainterPath'
// void addPolygon(const QPolygonF &polygon)

#include "tr_geo_poly.h"

#include <QtCore/qjsonobject.h>

class TrMapFace : public TrGeoObject
{
private:
	TrGeoPolygon * m_pline;
	uint16_t m_f_type;

	QBrush * m_actBrush;

protected:

public:
	TrMapFace();
	TrMapFace(QString, long id);
	virtual ~TrMapFace();

	friend QDebug operator<<(QDebug dbg, const TrMapFace& poi);

	virtual void setLayerShowMask(uint64_t mask);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	void appendPolyPoint(TrPoint pt);

	void appendPolygon(uint8_t flags);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	bool setSurroundingRect();

	void setActiveBrush(QBrush * brush);

	virtual uint16_t getDrawType();

	virtual void setDrawType(uint16_t type);

#ifdef TR_SERIALIZATION
	virtual bool exportGeoJson(QJsonObject & geojson, uint64_t mode);

	bool importArrayJson(const QJsonArray & arrjson, uint64_t mode);

	virtual bool importGeoJson(const QJsonObject & geojson, uint64_t mode);

	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif  // TR_MAP_FACE_H

