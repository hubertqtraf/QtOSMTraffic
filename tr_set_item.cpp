/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrSetItem
 * superclass:	---
 * modul:	tr_set_item.cc 
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


#include "tr_set_item.h"

#include <QtWidgets/qcolordialog.h>
#include <QtCore/qdebug.h>

// only for logger
#include "tr_geo_object.h"


TrSetItem::TrSetItem(QDomNode &node, int row, TrSetItem *parent)
{
	domNode = node;
	// Record the item's location within its parent.
	rowNumber = row;
	parentItem = parent;
}

TrSetItem::~TrSetItem()
{
	QHash<int, TrSetItem*>::iterator it;
	for (it = childItems.begin(); it != childItems.end(); ++it)
		delete it.value();
}

QDomNode TrSetItem::node() const
{
	return domNode;
}

TrSetItem * TrSetItem::parent()
{
	return parentItem;
}

TrSetItem * TrSetItem::child(int i)
{
	if (childItems.contains(i))
		return childItems[i];

	if (i >= 0 && i < domNode.childNodes().count()) 
	{
		QDomNode childNode = domNode.childNodes().item(i);
		TrSetItem *childItem = new TrSetItem(childNode, i, this);
		childItems[i] = childItem;
		return childItem;
	}
    return nullptr;
}

int TrSetItem::row()
{
	return rowNumber;
}

bool TrSetItem::getAtt(const QString & att_name, QDomNode & attribute)
{
	QDomNamedNodeMap attributeMap = domNode.attributes();

	for (int i = 0; i < attributeMap.count(); ++i)
	{
		attribute = attributeMap.item(i);

		if(attribute.nodeName() == att_name)
			return true;
	}
	return false;
}

bool TrSetItem::getColorValue(QString & col_name, QColor & color)
{
	if((col_name.size() != 7) && (col_name.size() != 9))
	{
		TR_WRN << "col_name.size != " << col_name.size();
		return false;
	}

	//TODO: option for Qt6 -> color.fromString(col_name);
	// rgb: #c8ffff, rgb with alpha cannel: #80c8ffff

	if(!col_name.startsWith("#"))
	{
		TR_WRN << "'#' missing" << col_name;
		return false;
	}

	bool OK = false;
	QString str = col_name.section('#', 1, 1);
	if(str.size() == 8)
	{
		QString alpha_str = str;
		alpha_str.truncate(2);
		int alpha = alpha_str.toInt(&OK, 16);
		if(OK)
			color.setAlpha(alpha);
		//TR_INF << alpha_str << alpha;
	}
	str = str.right(6);
	//TR_INF << str;

    uint32_t ncol = static_cast <uint32_t>(str.toInt(&OK, 16));
	if(!OK)
	{
		TR_WRN << "'toInt' error " << col_name;
		return false;
	}
	color.setBlue(ncol & 0x00000ff);
	color.setGreen((ncol >> 8) & 0x00000ff);
	color.setRed((ncol >> 16) & 0x00000ff);

	return true;
}

int TrSetItem::getIndex(int column)
{
	QDomNode attribute;

	if(getAtt("index", attribute) == true)
	{
		QString col_val = attribute.nodeValue();

        return(col_val.toInt(nullptr, 16));
	}
	return -1;
}

bool TrSetItem::getType(int column, QString & col_name)
{
	QDomNode attribute;

	if(getAtt("type", attribute) == true)
	{
		col_name = attribute.nodeValue();
		return true;
	}
	return false;
}

bool TrSetItem::setColName(int column, QString & col_name)
{
	QDomNode attribute;

	if(getAtt("name", attribute) == true)
	{
		col_name = attribute.nodeValue();

		// TR_MSG << col_name;
		return true;
	}
	return false;
}

bool TrSetItem::setColor(int column, QColor & color)
{
	QString xcol;
	QDomNode attribute;

	if(getAtt("rgb", attribute) == true)
	{
		xcol = attribute.nodeValue();

		return getColorValue(xcol, color);
	}
	return false;
}

bool TrSetItem::setData(int column, const QVariant &value)
{
	QString xcol;
	QDomNode col_att;

	QDomNode attribute;

	if (getAtt("rgb", attribute) == true)
	{
		xcol = attribute.nodeValue();
		col_att = attribute;

		QColor color;

		if(getColorValue(xcol, color) == false)
		{
			return false;
		}
        color = QColorDialog::getColor(color, nullptr, "Layer", QColorDialog::ShowAlphaChannel);

		if(!color.isValid())
		{
			TR_WRN << "!QColor::isValid";
                	return false;
		}

		QString number;
		if(color.alpha() == 255)
			number = color.name(QColor::HexRgb);
		else
			number = color.name(QColor::HexArgb);
		col_att.setNodeValue(number);

		//TR_INF << "# alpha " << number;

		return true;
	}
	return false;
}

