/******************************************************************
 *
 * @short	base class of trafalgar geometric selectable objects 
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoObject
 * superclass:  ---
 * modul:	tr_geo_object.h	geometric selectable object	
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2009-2022
 *
 * beginning:	04.2009
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

#ifndef _tr_geo_object_h
#define _tr_geo_object_h

#include <QtCore/qglobal.h>
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	#define HEX Qt::hex
	#define DEC Qt::dec
#else
	#define HEX hex
	#define DEC dec
#endif

#include "tr_defs.h"
#include "tr_point.h"

#include "tr_zoom_map.h"

//#include <geo_base.h>
//#include <geo_lin.h>

#include <stdint.h>

#include <QtCore/qstring.h>
#include <QtGui/qpainter.h>
#include <QtCore/qxmlstream.h>

#define TR_MOUSE_PRESS 1
#define TR_MOUSE_RELEASE 2
#define TR_MOUSE_MOVE 3

#define TR_NO_VALUE   		0x1000000000000000U

#define TR_MASK_EXIST 		0x0000000000000001U
#define TR_MASK_DRAW  		0x0000000000000002U
#define TR_MASK_EXPANSION	0x0000000000000004U
// TODO: allways use 'TR_MASK_SELECTED' for all classes, not only assembly
#define TR_MASK_SELECTED	0x0000000000000008U

#define TR_MASK_DATA 		0x0000000000000010U

#define TR_OBJ_DRAW_ERR         0x02

// this flags are for the init function
#define TR_INIT_COLORS		0x0000000000000100U
#define TR_INIT_GEOMETRY	0x0000000000001000U

// TODO: replace 'init' to this?
// demandElement->updateGeometry();
// demandElement->updatePartialGeometry(this);

class TrGeoObject 
{
private:

protected:
	QPen * m_geo_active_pen;
	QPen * disabled_pen;
	int select_id;
	uint64_t m_flags;
	// alternative: std::array<double, 4> surroundingRectx;
	double surroundingRect[4];

	uint64_t m_inst_mask;
	// TODO: back to protected?
	//static uint64_t s_mask;

	void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);
	bool setPosition(double x, double y, int idx);
	void createSelectPoints(int n);

public:
	// TODO: only for link shift, but...
	static uint64_t s_mask;

	TrGeoObject();

	TrGeoObject (const TrGeoObject& other);

	virtual ~TrGeoObject();

	friend QDebug operator<<(QDebug dbg, const TrGeoObject& obj);

	virtual QString getName() const;

	virtual bool setName(const QString & name);

	virtual QString getXmlName() const;

	static void setGlobelFlags(uint64_t flags);

	static uint64_t getGlobelFlags();

	virtual void setMask(uint64_t bit_mask);

	virtual void removeMask(uint64_t bit_mask);

	virtual uint64_t checkMask(uint64_t bit_mask);

	virtual void setActivePen(QPen * pen);

	virtual void setActiveBrush(QBrush * brush);

	virtual void setColorGroup(const QString & group, QMap<int, QColor> & colors);
	// TODO, remove when obsolete...
	virtual bool addPen(const QString & group, int idx, const QPen & pen);

	virtual bool appendObjectBrush(int idx, QBrush brush);

	virtual QPen * getActivePen();

	void setSelectPen(QPen * act, QPen * dis);

	virtual uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	virtual uint64_t editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr) = 0;

	virtual void setNameList(TrGeoObject * list);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	virtual void resize(const TrZoomMap & zoom_ref, int x, int y);

	void drawSurroundingRect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode);

	void disactivatePoints();

	double getSurroundRectVal(int n);

	void updateSurroundRect(TrGeoObject & other, bool first);

	void updateSurroundRect(double rect[4], bool first);

	virtual bool setSurroundingRect() = 0;

	virtual bool clip(const TrZoomMap & zoom_ref);

	virtual void setActBrush(QBrush * brush);

	// TODO: rework
	virtual uint16_t getType() const;

	virtual void setType(uint16_t);

	virtual void setLayerShowMask(uint64_t mask);

	virtual void clear();

	virtual bool exportGeoJson(QJsonObject & geojson, uint64_t mode);

	virtual bool importGeoJson(const QJsonObject & geojson, uint64_t mode);
#ifdef TR_SERIALIZATION
	static bool readDefStartElement(QXmlStreamReader & xml_in, QString & ref);

	const QString readXmlHeader(QXmlStreamReader & xml_in);
#endif
	void abortOnLine(QXmlStreamReader & xml_in, const QString & text);

	// TODO: vrtual mode: replace by readXmlHeader
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);

};


#endif
