/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024
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

#include "profile.h"

#include <QFile>

#include <tr_defs.h>

Profile::Profile(QObject *parent)
    : QAbstractItemModel(parent)
    , rootItem(nullptr)
{
}

bool Profile::setDataByFile(const QString & fname)
{
        QFile file;
        TR_INF << "profile: " << fname;
        file.setFileName(fname);
        if(!file.exists())
                return false;

        file.open(QIODevice::ReadOnly);

        domDocument.setContent(&file);
        rootItem = new TrSetItem(domDocument, 0);
        file.close();
        return true;
}


QVariant Profile::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
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

QModelIndex Profile::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex Profile::parent(const QModelIndex &index) const
{
    if(!index.isValid())
            return QModelIndex();

    TrSetItem *childItem = static_cast<TrSetItem*>(index.internalPointer());
    TrSetItem *parentItem = childItem->parent();

    if(!parentItem || parentItem == rootItem)
            return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int Profile::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;
    if(parent.column() > 0)
            return 0;

    TrSetItem *parentItem;

    if(!parent.isValid())
            parentItem = rootItem;
    else
            parentItem = static_cast<TrSetItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

int Profile::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        TR_INF << "PRARENT!!!!";
        return 3;
    }
    return 3;
}

QVariant Profile::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return QVariant();
}

QDomDocument & Profile::getDocument()
{
    return domDocument;
}

