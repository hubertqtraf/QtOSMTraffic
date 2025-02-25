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

#ifndef TRDISPOPTIONDIALOG_H
#define TRDISPOPTIONDIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class TrDispOptionDialog;
}

class TrDispOptionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TrDispOptionDialog(QWidget *parent = nullptr);
	~TrDispOptionDialog();

	void setLayerList(const QStringList &profile, const QStringList &doc);
	void setLayerItemList(const QString &layer, const QStringList &items);

	const QStringList &getViewList() const;

	QTableWidget *getTable(const QString &tab_name);
	uint64_t initFlags(QTableWidget *table);
	void getLayerMasks(QMap<QString, uint64_t> &layers);
	uint64_t getLayerMask();
	const QStringList getLayerActList();

private slots:
	void on_tableWidget_cellDoubleClicked(int row, int column);

private:
	Ui::TrDispOptionDialog *ui;
	QTableWidget * m_world_table;

	QStringList m_checkedLayerList;
	void createLayerTab(const QString &tab_name);
	void addCheckLine(QTableWidget * table, const QString &name, Qt::CheckState mode, int row);
};

#endif // TRDISPOPTIONDIALOG_H
