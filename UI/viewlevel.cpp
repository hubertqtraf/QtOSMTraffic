/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2026
 ******************************************************************/

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "viewlevel.h"

#include <tr_geo_object.h>

#include <QFile>

ViewLevel::ViewLevel(QObject *parent)
	: QAbstractItemModel(parent)
	, m_rootItem(nullptr)
{
}

QVariant ViewLevel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return tr("Base");

		case 1:
			return tr("Item");

		case 2:
			return tr("Show");

		default:
			return QVariant();
		}
	}
	return QVariant();
}

bool ViewLevel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
	if (value != headerData(section, orientation, role))
	{
		// FIXME: Implement me!
		emit headerDataChanged(orientation, section, section);
		return true;
	}
	return false;
}

QModelIndex ViewLevel::index(int row, int column, const QModelIndex &parent) const
{
	if(!hasIndex(row, column, parent))
		return QModelIndex();

	TrSetItem *parentItem;

	if(!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<TrSetItem*>(parent.internalPointer());

	TrSetItem *childItem = parentItem->child(row);
	if(childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex ViewLevel::parent(const QModelIndex &index) const
{
	if(!index.isValid())
		return QModelIndex();

	TrSetItem *childItem = static_cast<TrSetItem*>(index.internalPointer());
	TrSetItem *parentItem = childItem->parent();

	if(!parentItem || parentItem == m_rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int ViewLevel::rowCount(const QModelIndex &parent) const
{
	if(parent.column() > 0)
		return 0;

	TrSetItem *parentItem;

	if(!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<TrSetItem*>(parent.internalPointer());;
	return parentItem->node().childNodes().count();
}

int ViewLevel::columnCount(const QModelIndex &parent) const
{
	//if (!parent.isValid())
	//	return 0;

	return 3;
}

QVariant ViewLevel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
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
			return "";
			}

		case 2:
			return node.nodeName();

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
		return QVariant();

	default:
		return QVariant();
	}

}

bool ViewLevel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (data(index, role) != value)
	{
		// FIXME: Implement me!
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

Qt::ItemFlags ViewLevel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool ViewLevel::setDataByFile(const QString & fname)
{
	QFile file;
	file.setFileName(fname);
	if(!file.exists())
		return false;
	if(file.open(QIODevice::ReadOnly) == false)
	{
		TR_WRN << "unable to load [" << fname << "]";
		return false;
	}

	m_domDocument.setContent(&file);
	m_rootItem = new TrSetItem(m_domDocument, 0);
	//m_rootItem = new QDomNode();
	file.close();
	return true;
}

QDomDocument * ViewLevel::getDocument()
{
	return &m_domDocument;
}