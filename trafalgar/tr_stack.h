/******************************************************************
 *
 * @short	Stack to control the layers
 *
 * project:	Trafalgar lib
 *
 * class:	TrStack
 * superclass:	TrGeoObject
 * modul:	tr_stack.h
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

#ifndef TR_STACK
#define TR_STACK

#include <stdint.h>

#include "tr_layer.h"

class TrStack : public TrGeoObject
{
private:
	uint32_t m_flags;
	QMap<QString, TrLayer*> m_layerMap;
	QMap<QString, QVector<TrGeoObject *>> m_order;

public:
	TrStack();
	virtual ~TrStack();

	// TODO: check function needed/usefull?
	void clear();

	void clear(const QString & name);

	void printData();

	QVector<QString> getLayerList(const QString & mode);

	bool addLayer(const QString & name, TrGeoObject * layer);

	TrGeoObject * getMapObject(const QString & name);

	void appendListMembers(QStringList & list);

	const QStringList getTypeStrings(const QString & type) const;

	void setFunctionOrder(const QStringList func, const QString & type);

	bool addOrderType(const QString & type);

	bool addOrderTypeStdList();

	bool resetOrderList(const QString & name);

	bool appendOrderElement(const QString & type, const QString & name);

	bool checkLayerName(const QString & name);

	const QString findFirstExt();

	void setFirstExt(const QString & name);

	void setNameList(TrGeoObject * name_list);

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	virtual bool setSurroundingRect();

	QVector<double> getSurroundingVecRect();

	void setSurroundingVecRect(const QVector<double> & rect);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
};

#endif //TR_STACK

