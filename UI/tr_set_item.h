/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrSetItem
 * superclass:	---
 * modul:	tr_set_item.h 
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	12.2016
 *
 * (C)		Schmid Hubert 2016-2022
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



#ifndef TR_SET_ITEM_H
#define TR_SET_ITEM_H

#include <QtXml/qdom.h>
#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

class TrSetItem
{
private:
	QDomNode domNode;
	QHash<int, TrSetItem*> childItems;
	TrSetItem *parentItem;
	int rowNumber;

	bool getAtt(const QString & att_name, QDomNode & attribute);
	bool getColorValue(QString & col_name, QColor & color);

public:
    TrSetItem(QDomNode &node, int row, TrSetItem *parent = nullptr);
	~TrSetItem();

	TrSetItem *child(int i);
	TrSetItem *parent();
	QDomNode node() const;
	int row();

	int getIndex(int column);

	bool getType(int column, QString & col_name);

	bool setColor(int column, QColor & color);

	bool setColName(int column, QString & col_name);

	bool setData(int column, const QVariant &value);
};

#endif	// TR_SET_ITEM_H

