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
	, m_checkedState(Qt::Checked)
{
}

TrSetItem * ViewLevel::nodeForIndex(const QModelIndex &index) const
{
	if (index.isValid())
	{
		TrSetItem * item = static_cast<TrSetItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return m_rootItem;
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

	if(!index.isValid())
			return QVariant();

	if(role == Qt::CheckStateRole)
	{
		if (index.column() == 2)
		{
			TrSetItem *item = nodeForIndex(index);
			if(item == nullptr)
				return QVariant();
			return item->getState();
		}
	}
	else if (role == Qt::DisplayRole)
	{
		TrSetItem *item = nodeForIndex(index);

		if (index.column() == 0)
		{
			return item->node().nodeName();
		}
		if (index.column() == 1)
		{
			QString col_name;

			if(item->setColName(1, col_name))
			{
				return col_name;
			}
			return item->node().nodeName();
		}
	}
	return QVariant();
}

bool ViewLevel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role == Qt::CheckStateRole && index.column() == 2)
	{
		//TrSetItem *item = nodeForIndex(index);
		QTreeWidgetItem *check = static_cast<QTreeWidgetItem*>(index.internalPointer());
		bool boolVal = value.value<bool>();

		if(!boolVal)
		{
			check->setCheckState(2, Qt::Unchecked);
		}
		else
		{
			check->setCheckState(2, Qt::Checked);
		}
		emit dataChanged(index, index);
		return true;
	}
	emit dataChanged(index, index);
	return false;
}

Qt::ItemFlags ViewLevel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	//if((index.column() == 2) && index.parent().isValid())
	{
		flags |= Qt::ItemIsUserCheckable;
		return flags;
	}
	return QAbstractItemModel::flags(index);
	//return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
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