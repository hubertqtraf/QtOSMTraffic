/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrSetModel
 * superclass:	QAbstractItemModel
 * modul:	tr_set_model.h 
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



#ifndef TR_SET_MODEL_H
#define TR_SET_MODEL_H

#include <QtWidgets/qabstractitemdelegate.h>
#include <QtXml/qdom.h>

#include <QtCore/qfile.h>

#include "tr_set_item.h"

class TrSetModel : public QAbstractItemModel 
{
	Q_OBJECT
private:
	QDomDocument domDocument;
	TrSetItem *rootItem;

	TrSetItem * getItem(const QModelIndex &index) const;

public:
    explicit TrSetModel(QObject *parent = nullptr);

	bool setDataByFile(const QString & fname);

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

	bool setData(const QModelIndex &index, const QVariant &value,
		int role = Qt::EditRole) Q_DECL_OVERRIDE;
	bool setHeaderData(int section, Qt::Orientation orientation,
		const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

	QDomDocument * getDocument();

public slots:
};

#endif	// TR_SET_MODEL_H

