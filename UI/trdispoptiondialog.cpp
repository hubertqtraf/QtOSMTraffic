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

#include "trdispoptiondialog.h"
#include "ui_trdispoptiondialog.h"

#include <QTableWidget>
#include <tr_defs.h>

TrDispOptionDialog::TrDispOptionDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::TrDispOptionDialog)
	, m_world_table(nullptr)
{
	ui->setupUi(this);
}

TrDispOptionDialog::~TrDispOptionDialog()
{
	delete ui;
}

void TrDispOptionDialog::setLayerList(const QStringList & profile, const QStringList & doc)
{
	QMap<QString, bool> layer_map;

	for (int i = 0; i < doc.size(); ++i)
		layer_map[doc[i]] = false;
	for (int i = 0; i < profile.size(); ++i)
		layer_map[profile[i]] = true;
	m_checkedLayerList.clear();
	for (auto i = layer_map.cbegin(), end = layer_map.cend(); i != end; ++i)
	{
		if(i.value() == true)
			m_checkedLayerList.append(i.key());
	}
	//TR_INF << "list: " << m_checkedLayerList;

	// after a reload of an other OSM file, use the existing tabs
	if(ui->tabWidget->count() <= 1)
	{
		for (int i = 0; i < m_checkedLayerList.size(); ++i)
		{
			createLayerTab(m_checkedLayerList[i]);
		}
	}

	QTableWidget * world = getTable("World");
	if(world != nullptr)
	{
		world->setRowCount(m_checkedLayerList.size());
		world->setColumnCount(2);
	}

	ui->tabWidget->setCurrentIndex(0);
	QWidget * w = ui->tabWidget->currentWidget();
	QObjectList widgetList = w->children();
	for (int i = 0; i < widgetList.size(); ++i)
	{
		QTableWidget * ww = dynamic_cast<QTableWidget *>(widgetList[i]);
		if(ww != nullptr)
		{
			m_world_table = ww;
			m_world_table->setColumnCount(2);
			m_world_table->setRowCount(m_checkedLayerList.size());

			for (int i = 0; i < m_checkedLayerList.size(); ++i)
				addCheckLine(m_world_table, m_checkedLayerList[i], Qt::Checked, i);
		}
	}
}

const QStringList & TrDispOptionDialog::getViewList() const
{
	return m_checkedLayerList;
}

void TrDispOptionDialog::setLayerItemList(const QString & layer, const QStringList & items)
{
	QTableWidget * table = nullptr;
	for (int i = 0; i < ui->tabWidget->count(); ++i)
	{
		table = getTable(layer);
		if(table != nullptr)
		{
			table->setRowCount(items.size());
			table->setColumnCount(2);
			for (int j = 0; j < items.size(); ++j)
				addCheckLine(table, items[j], Qt::Checked, j);
		}
	}
}


QTableWidget * TrDispOptionDialog::getTable(const QString & tab_name)
{
	for (int i = 0; i < ui->tabWidget->count(); ++i)
	{
		QWidget * page = ui->tabWidget->widget(i);
		if(page == nullptr)
			return nullptr;
		if(ui->tabWidget->tabText(i) == tab_name)
		{
			QLayout * layout = page->layout();
			if(layout == nullptr)
				return nullptr;
			if(layout->count() > 0)
			{
				QWidget *w = layout->itemAt(0)->widget();
				QTableWidget * table = dynamic_cast<QTableWidget *>(w);
				return table;
			}
		}
	}
	return nullptr;
}


void TrDispOptionDialog::createLayerTab(const QString & tab_name)
{
	QWidget * page = new QWidget();

	ui->tabWidget->addTab(page, tab_name);
	QVBoxLayout * layoutV = new QVBoxLayout(page);

	QTableWidget * table = new QTableWidget(this);
	layoutV->addWidget(table);
}

void TrDispOptionDialog::addCheckLine(QTableWidget * table, const QString & name, Qt::CheckState mode, int row)
{
	QTableWidgetItem *nameItem = new QTableWidgetItem(name);
	table->setItem(row, 0, nameItem);
	QTableWidgetItem *visibleItem = new QTableWidgetItem();
	visibleItem->data(Qt::CheckStateRole);
	visibleItem->setCheckState(mode);
	table->setItem(row, 1, visibleItem);
	// TODO: used for limit value...
	// restore the brush of the cell
	// m_cellBackground.setColor(nameItem->background().color());
}

uint64_t TrDispOptionDialog::initFlags(QTableWidget * table)
{
	uint64_t ret = 0;
	if(table == nullptr)
		return ret;
	for(int i = 0; i < table->rowCount(); i++)
	{
		Qt::CheckState state = table->item(i, 1)->checkState();
		if(state == Qt::Checked)
			ret |= (1 << i);
	}
	return ret;
}

void TrDispOptionDialog::getLayerMasks(QMap<QString, uint64_t> & layers)
{
	for(int i=0; i < m_checkedLayerList.size(); i++)
		layers[m_checkedLayerList[i]] = 0;
	QTableWidget * table = nullptr;
	for (auto ii = layers.begin(), end = layers.end(); ii != end; ++ii)
	{
		table = getTable(ii.key());
		if(table != nullptr)
		{
			ii.value() = initFlags(table);
		}
	}
}

uint64_t TrDispOptionDialog::getLayerMask()
{
	if(m_world_table != nullptr)
	{
		return initFlags(m_world_table);
	}
	return 0;
}

// TODO: rework, more checks, etc.
const QStringList TrDispOptionDialog::getLayerActList()
{
	QStringList list;
	if(m_world_table == nullptr)
		return list;
	for(int i=0; i < m_checkedLayerList.size(); i++)
	{
		Qt::CheckState state = m_world_table->item(i, 1)->checkState();
		if(state == Qt::Checked)
			list.append(m_checkedLayerList[i]);
	}
	return list;
}


void TrDispOptionDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
	TR_INF;
}
