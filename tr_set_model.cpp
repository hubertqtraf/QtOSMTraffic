/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrSetModel
 * superclass:	QAbstractItemModel
 * modul:	tr_set_model.cc 
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


#include "tr_set_model.h"

#include <stdio.h>

TrSetModel::TrSetModel(QObject *parent)
	: QAbstractItemModel(parent)
	, rootItem(nullptr)
{
}

bool TrSetModel::setDataByFile(const QString & fname)
{
	QFile file;
	file.setFileName(fname);
	if(!file.exists())
		return false;
	file.open(QIODevice::ReadOnly);

	domDocument.setContent(&file);
	rootItem = new TrSetItem(domDocument, 0);
	file.close();
	return true;
}

int TrSetModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 3;
}

TrSetItem * TrSetModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) 
	{
		TrSetItem * item = static_cast<TrSetItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QVariant TrSetModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	TrSetItem * item = static_cast<TrSetItem*>(index.internalPointer());

	switch(role)
	{
	case Qt::DisplayRole:
		{
		QDomNode node = item->node();
		QStringList attributes;
		QDomNamedNodeMap attributeMap = node.attributes();

		switch (index.column()) 
		{
		case 0:
			return node.nodeName();

		case 1:
			{
			QString col_name;

			if(item->setColName(1, col_name))
			{
				return col_name;
			}

			//setPalette(QPalette(QColor(222,0,0)));
			//setAutoFillBackground(true);
			for (int i = 0; i < attributeMap.count(); ++i) 
			{
				QDomNode attribute = attributeMap.item(i);
				attributes << attribute.nodeName() + "=\""
				+attribute.nodeValue() + '"';
			}
			return attributes.join(' ');
			}
			//return "";

		case 2:
			return node.nodeValue().split("\n").join(' ');

		default:
			return QVariant();
		}
		}

	case Qt::DecorationRole:
		if(index.column() == 2)
		{
			QColor pal;
			if(item->setColor(1, pal))
				return pal;
			else
				return QVariant();
		}
		//return QVariant();
	
	default:
		return QVariant();
	}
}

Qt::ItemFlags TrSetModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		// 5.15 'return 0': deprecated: Use default constructor instead [-Wdeprecated-declarations]
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant TrSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole) 
	{
		switch (section) 
		{
		case 0:
			return tr("Item");

		case 1:
			return tr("Name");

		case 2:
			return tr("Value");

		default:
			return QVariant();
		}
	}

	return QVariant();
}

QModelIndex TrSetModel::index(int row, int column, const QModelIndex &parent) const
{
	if(!hasIndex(row, column, parent))
		return QModelIndex();

	TrSetItem *parentItem;

	if(!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TrSetItem*>(parent.internalPointer());

	TrSetItem *childItem = parentItem->child(row);
	if(childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TrSetModel::parent(const QModelIndex &child) const
{
	if(!child.isValid())
		return QModelIndex();

	TrSetItem *childItem = static_cast<TrSetItem*>(child.internalPointer());
	TrSetItem *parentItem = childItem->parent();

	if(!parentItem || parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TrSetModel::rowCount(const QModelIndex &parent) const
{
	if(parent.column() > 0)
		return 0;

	TrSetItem *parentItem;

	if(!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TrSetItem*>(parent.internalPointer());

	return parentItem->node().childNodes().count();
}


bool TrSetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(role != Qt::EditRole)
		return false;

	TrSetItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if(result)
		emit dataChanged(index, index);

	return result;
}

bool TrSetModel::setHeaderData(int section, Qt::Orientation orientation,
	const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if(result)
		emit headerDataChanged(orientation, section, section);

	return result;
}

QDomDocument * TrSetModel::getDocument()
{
	return &domDocument;
}

