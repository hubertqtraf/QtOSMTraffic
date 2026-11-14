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

#include <QFile>
#include <QFileDialog>
#include <tr_geo_object.h>

#include "treedialog.h"
#include "ui_treedialog.h"


TreeElement::TreeElement()
	: flur_nr("")
	, pic_nr("")
	, road{""}
	, etc{""}

	, act(0)
	, osm_id(0)
	, osm_inc()
	, coor_x(0.0)
	, coor_y(0.0)

	, tree_type("")
	, height(0)
	, single(0)
	, age(0)
	, dia(0)
	, stem_count(0)
	, dia_min(0)
	, dia_max(0)

	, rel_flags(0x0000000000000000)
	, rel_other("")

	, cond_flags(0x0000000000000000)
	, root_other("")
	, stem_other("")
	, crown_limit("")
	, crown_other("")

	, all()
{
}

TreeDialog::TreeDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::TreeDialog)
	, m_ok_clicked(false)
{
	ui->setupUi(this);

	ui->treeWidget->setColumnCount(1);
}

TreeDialog::~TreeDialog()
{
	delete ui;
}

void TreeDialog::save(const QString &fname)
{
	//QString map;

	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out << "<" << TREE_LIST << ">\n";
	for (auto i = m_tree_map.cbegin(), end = m_tree_map.cend(); i != end; ++i)
	{
		QMap<int, TreeElement> nr_map = i.value();
		for (auto j = nr_map.cbegin(), end = nr_map.cend(); j != end; ++j)
		{
			out << "<tree map=\"" << i.key() << "\" " << "local_id=\""  << j.key() << "\"" << ">\n";
			TreeElement * elem = getTreeNodeData(i.key(), QString::number(j.key()));
			if(elem == nullptr)
				return;
			out << createXmlPair(TAG, BUND_POS_MAP, i.key()) << "\n";
			out << createXmlInt(TAG, BUND_POS_TREE_NR, j.key()) << "\n";
			saveTree(out, *elem);
			//saveTree(out,i.key(), j.key());
			out << "</tree>\n";
		}
	}
	out << "</" << TREE_LIST << ">\n";
	/*QTreeWidgetItemIterator it(ui->treeWidget);
	while (*it)
	{
		if((*it)->childCount())
		{
			map = (*it)->text(0);
			//out << "--->" << (*it)->text(0) << " - " << (*it)->childCount() << "\n";
		}
		else
		{
			ui->map->setText(map);
			ui->treeNr->setValue(getID((*it)->text(0)));
			out << "<tree map=\"" << map << "\" " << "local_id=\""  << (*it)->text(0) << "\"" << ">\n";
			saveTree(out, map, 0);
			out << "<tree/>\n";
		}
		++it;
	}*/
}

void TreeDialog::elementToGui(const TreeElement & te)
{
	ui->flurNr->setText(te.flur_nr);
	ui->picNr->setText(te.pic_nr);
	ui->road->setText(te.road);
	ui->etc->setText(te.etc);

	ui->act->setCurrentIndex(te.act);
	if(te.osm_inc == 0)
		ui->inOSM->setCheckState(Qt::Unchecked);
	else
		ui->inOSM->setCheckState(Qt::Checked);
	ui->OSM_ID->setText(QString::number(te.osm_id, 'g', 10));
	ui->CoorX->setText(QString::number(te.coor_x, 'g', 10));
	ui->CoorY->setText(QString::number(te.coor_y, 'g', 10));

	ui->type->setText(te.tree_type);
	ui->treeHeight->setCurrentIndex(te.height);
	if(te.single == 0)
		ui->singleTree->setCheckState(Qt::Unchecked);
	else
		ui->singleTree->setCheckState(Qt::Checked);
	ui->age->setValue(te.age);
	ui->diameter->setValue(te.dia);
	ui->trunkCount->setValue(te.stem_count);
	ui->diameterMin->setValue(te.dia_min);
	ui->diameterMax->setValue(te.dia_max);

	relFlagsToGui(te.rel_flags);
	ui->rel_misc->setText(te.rel_other);
	condFlagsToGui(te.cond_flags);
	ui->cond_roor_misc->setText(te.root_other);
	ui->cond_stem_misc->setText(te.stem_other);
	ui->cond_crown_misc->setText(te.crown_other);
	ui->cond_limit->setText(te.crown_limit);

	ui->impression->setCurrentIndex(te.all);
}

void TreeDialog::guiToElement(TreeElement & te)
{
	te.flur_nr = ui->flurNr->text();
	te.pic_nr = ui->picNr->text();
	te.road = ui->road->text();
	te.etc = ui->etc->text();

	te.act = ui->act->currentIndex();
	te.osm_inc = ui->inOSM->checkState();
	te.osm_id = getID(ui->OSM_ID->text());
	te.coor_x = getCoor(ui->CoorX->text());
	te.coor_y = getCoor(ui->CoorY->text());

	te.tree_type = ui->type->text();
	te.height = ui->treeHeight->currentIndex();
	te.single = ui->singleTree->checkState();
	te.age = ui->age->value();
	te.dia = ui->diameter->value();
	te.stem_count = ui->trunkCount->value();
	te.dia_min = ui->diameterMin->value();
	te.dia_max = ui->diameterMax->value();

	te.rel_flags = relFlags();
	te.rel_other = ui->rel_misc->text();
	te.cond_flags = condFlags();
	te.root_other = ui->cond_roor_misc->text();
	te.stem_other = ui->cond_stem_misc->text();
	te.crown_other = ui->cond_crown_misc->text();
	te.crown_limit = ui->cond_limit->text();

	te.all = ui->impression->currentIndex();
}

void TreeDialog::guiToTreelist(const QString &base, int id)
{
	guiToElement(m_tree_map[base][id]);
}

void TreeDialog::saveTree(QTextStream &out, const TreeElement & te)
{
	//TreeElement * elem = getTreeNodeData(map, QString::number(local_id));
	//if(elem == nullptr)
	//		return;
	//out << createXmlPair(TAG, BUND_POS_MAP, map) << "\n";
	//out << createXmlInt(TAG, BUND_POS_TREE_NR , local_id) << "\n";
	out << createXmlPair(TAG, BUND_POS_FLUR, te.flur_nr) << "\n";
	out << createXmlPair(TAG, BUND_POS_PIC_NR, te.pic_nr) << "\n";
	out << createXmlPair(TAG, BUND_POS_ROAD, te.road) << "\n";
	out << createXmlPair(TAG, BUND_POS_ETC, te.etc) << "\n";
	out << createXmlInt(TAG, BUND_POS_ACT, te.act) << "\n";
	out << createXmlInt(TAG, BUND_POS_OSM_INC, te.osm_inc) << "\n";
	out << createXmlInt(TAG, BUND_POS_OSM_ID, te.osm_id) << "\n";

	out << createXmlDouble(TAG, BUND_POS_COOR_X, te.coor_x) << "\n";
	out << createXmlDouble(TAG, BUND_POS_COOR_Y, te.coor_y) << "\n";

	out << createXmlPair(TAG, BUND_DESC_TYPE, te.tree_type) << "\n";
	out << createXmlInt(TAG, BUND_DESC_HEIGHT, te.height) << "\n";
	out << createXmlInt(TAG, BUND_DESC_SINGLE, te.single) << "\n";
	out << createXmlInt(TAG, BUND_DESC_AGE, te.age) << "\n";
	out << createXmlDouble(TAG, BUND_DESC_DIA, te.dia ) << "\n";
	out << createXmlInt(TAG, BUND_DESC_COUNT, te.stem_count) << "\n";
	out << createXmlDouble(TAG, BUND_DESC_DIA_MIN, te.dia_min) << "\n";
	out << createXmlDouble(TAG, BUND_DESC_DIA_MAX, te.dia_max) << "\n";

	QString rel_flags = "0x" + QString::number(te.rel_flags, 16);
	out << createXmlPair(TAG, BUND_REL_FLAGS, rel_flags) << "\n";
	out << createXmlPair(TAG, BUND_REL_OTHER, te.rel_other) << "\n";

	QString cond_flags = "0x" + QString::number(te.cond_flags, 16);
	out << createXmlPair(TAG, BUND_COND_FLAGS, cond_flags) << "\n";
	out << createXmlPair(TAG, BUND_COND_ROOT_OTHER, te.root_other) << "\n";
	out << createXmlPair(TAG, BUND_COND_STEM_OTHER, te.stem_other) << "\n";
	out << createXmlPair(TAG, BUND_COND_CROWN_OTHER, te.crown_other) << "\n";
	out << createXmlPair(TAG, BUND_COND_CROWN_LIMIT, te.crown_limit) << "\n";

	out << createXmlInt(TAG, BUND_ALL, te.all) << "\n";
}

QTreeWidgetItem * TreeDialog::getTreeItem(QTreeWidgetItem * base, const QString &node)
{
	int childCount = base->childCount();
	for (int i = 0; i < childCount; ++i)
	{
		QTreeWidgetItem *child = base->child(i);
		QString test = child->data(0, Qt::DisplayRole).toString();
		if(test == node)
			return child;
	}
	return nullptr;
}

QTreeWidgetItem * TreeDialog::getBaseTreeItem(QTreeWidgetItem * base, const QString &node)
{
	QList<QTreeWidgetItem *> ilist = ui->treeWidget->findItems(node, Qt::MatchExactly);
	if(ilist.size() == 1)
	{
		if(ilist.size() == 1)
			return ilist[0];
	}
	return nullptr;
}

void TreeDialog::addTreeNodeData(const QString &page, const QString &nr)
{
	TreeElement tree_element;
	int index = getID(nr);

	if(m_tree_map.contains(page))
	{
		m_tree_map[page][index] = tree_element;
	}
	else
	{
		QMap<int, TreeElement> tree;
		tree[index] = tree_element;
		m_tree_map[page] = tree;
	}
}

TreeElement * TreeDialog::getTreeNodeData(const QString &page, const QString &nr)
{
	if(page == "")
		return nullptr;
	QMap<int, TreeElement> tree = m_tree_map[page];
	int index = getID(nr);
	if(index == 0)
		return nullptr;
	return &(tree[index]);
}

void TreeDialog::addTreeNode(const QString &page, const QString &nr)
{
	ui->treeWidget->clearSelection();
	//ui->treeWidget->setSelectionModel();

	if(m_tree_map.contains(page))
	{
		QTreeWidgetItem * imap = getBaseTreeItem(ui->treeWidget->topLevelItem(0), page);
		if(imap != nullptr)
		{
			QTreeWidgetItem * inr = getTreeItem(imap, nr);
			if(inr == nullptr)
			{
				QStringList list;
				list.append(nr);
				QTreeWidgetItem *jnr = new QTreeWidgetItem(imap, list, Qt::DisplayRole);
				jnr->setSelected(true);
				imap->setExpanded(true);

				addTreeNodeData(page, nr);
			}
		}
		return;
	}
	QStringList list;
	list.append(page);
	QTreeWidgetItem *jmap = new QTreeWidgetItem(ui->treeWidget, list, Qt::DisplayRole);
	list.clear();
	list.append(nr);
	QTreeWidgetItem *inr = new QTreeWidgetItem(jmap, list, Qt::DisplayRole);
	inr->setSelected(true);
	jmap->setExpanded(true);

	addTreeNodeData(page, nr);

	ui->map->setText(page);
	ui->treeNr->setValue(getID(nr));
}


void TreeDialog::read(const QString &fname)
{
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QXmlStreamReader xml;
	xml.setDevice(&file);

	QString local_map;
	QString local_nr;
	int local_id = 0;
	//TreeElement * element = nullptr;

	while (!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement())
		{
			if(xml.name().toString() == TREE_LIST)
			{
			}
			if(xml.name().toString() == TREE)
			{
				QXmlStreamAttributes attrs = xml.attributes();
				//QString ref = attrs.value("map").toString() + "_" +
				//		attrs.value("local_id").toString();
				if(attrs.value("map").toString() != "")
				{
					local_map = attrs.value("map").toString();
					local_nr = attrs.value("local_id").toString();
					local_id = getID(local_nr);
					addTreeNode(local_map, local_nr);
				}
			}
			if(xml.name().toString() == TAG)
			{
				QXmlStreamAttributes attrs = xml.attributes();
				QString key = attrs.value("k").toString();
				QString value = attrs.value("v").toString();

				if(key == BUND_POS_MAP)
				{
					ui->map->setText(value);
				}
				if(key == BUND_POS_TREE_NR)
				{
					ui->treeNr->setValue(getID(value));
				}
				if(key == BUND_POS_FLUR)
				{
					m_tree_map[local_map][local_id].flur_nr = value;
				}
				if(key == BUND_POS_PIC_NR)
				{
					m_tree_map[local_map][local_id].pic_nr = value;
				}
				if(key == BUND_POS_ROAD)
				{
					m_tree_map[local_map][local_id].road = value;
					//ui->road->setText(value);
				}
				if(key == BUND_POS_ETC)
				{
					m_tree_map[local_map][local_id].etc = value;
				}
				if(key == BUND_POS_ACT)
				{
					//ui->act->setCurrentIndex(getID(value));
					m_tree_map[local_map][local_id].act = getID(value);
				}
				if(key == BUND_POS_OSM_INC)
				{
					m_tree_map[local_map][local_id].osm_inc = getID(value);
				}
				if(key == BUND_POS_OSM_ID)
				{
					//ui->OSM_ID->setText(value);
					m_tree_map[local_map][local_id].osm_id = getID(value);
				}

				if(key == BUND_POS_COOR_X)
				{
					m_tree_map[local_map][local_id].coor_x = getCoor(value);
				}
				if(key == BUND_POS_COOR_Y)
				{
					m_tree_map[local_map][local_id].coor_y = getCoor(value);
				}

				if(key == BUND_DESC_TYPE)
				{
					m_tree_map[local_map][local_id].tree_type = value;
					//ui->type->setText(value);
				}
				if(key == BUND_DESC_HEIGHT)
				{
					m_tree_map[local_map][local_id].height = getCoor(value);
					//ui->treeHeight->setCurrentIndex(getID(value));
				}
				if(key == BUND_DESC_SINGLE)
				{
					m_tree_map[local_map][local_id].single = getID(value);
					//ui->singleTree->setCheckState(getCheckState(value));
				}
				if(key == BUND_DESC_AGE)
				{
					m_tree_map[local_map][local_id].age = getID(value);
					//ui->age->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA)
				{
					m_tree_map[local_map][local_id].dia = getCoor(value);
					//ui->diameter->setValue(getID(value));
				}
				if(key == BUND_DESC_COUNT)
				{
					m_tree_map[local_map][local_id].stem_count = getID(value);
					//ui->trunkCount->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA_MIN)
				{
					m_tree_map[local_map][local_id].dia_min = getCoor(value);
					//ui->diameterMin->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA_MAX)
				{
					m_tree_map[local_map][local_id].dia_max = getCoor(value);
					//ui->diameterMax->setValue(getID(value));
				}

				if(key == BUND_REL_FLAGS)
				{
					uint64_t flags = getFlags(value);
					m_tree_map[local_map][local_id].rel_flags = flags;
				}
				if(key == BUND_REL_OTHER)
				{
					m_tree_map[local_map][local_id].rel_other = value;
				}
				if(key == BUND_COND_FLAGS)
				{
					uint64_t flags = getFlags(value);
					m_tree_map[local_map][local_id].cond_flags = flags;
				}
				if(key == BUND_COND_ROOT_OTHER)
				{
					m_tree_map[local_map][local_id].root_other = value;
				}
				if(key == BUND_COND_STEM_OTHER)
				{
					m_tree_map[local_map][local_id].stem_other = value;
				}
				if(key == BUND_COND_CROWN_OTHER)
				{
					m_tree_map[local_map][local_id].crown_other = value;
				}
				if(key == BUND_COND_CROWN_LIMIT)
				{
					m_tree_map[local_map][local_id].crown_limit = value;
				}
				if(key == BUND_ALL)
				{
					m_tree_map[local_map][local_id].all = getID(value);
				}
				//TR_INF << key << value;
			}
		}
		if(xml.isEndElement())
		{
			if(xml.name().toString() == TREE_LIST)
			{
			}
			if(xml.name().toString() == TREE)
			{
				//int index = getID(local_nr);
				local_map = "";
				local_nr = "";
				local_id = 0;
				//element = nullptr;
			}
		}
	}
	if (xml.hasError())
	{
		//... do error handling
	}
}

void TreeDialog::setNodeData(const QString &name, const QString &osm_id,
							 const QString &x, const QString &y, const QString &data)
{
	QStringList list = name.split(u' ');
	Qt::CheckState osm = Qt::Checked;

	int poi_data = getID(data);
	poi_data &= 0x000000000000ffff;

	TreeElement tree;
	elementToGui(tree);

	ui->tree_tab->setCurrentIndex(0);

	int t_id = 0;
	ui->map->setText("");
	ui->treeNr->setValue(0);

	if(list.size() >= 3)
	{
		if(list[0] == "ott")
		{
			ui->map->setText(list[1]);
			osm = Qt::Unchecked;
			t_id = getID(list.last());
			ui->treeNr->setValue(t_id);
			addTreeNode(list[1], list.last());
		}
	}
	//else
	//	ui->map->setText(name);
	ui->inOSM->setCheckState(osm);
	ui->OSM_ID->setText(osm_id);
	ui->CoorX->setText(x);
	ui->CoorY->setText(y);
	if(poi_data)
		ui->diameterMax->setValue(poi_data/10.0);
	if(getTreeNodeData(ui->map->text(), ui->treeNr->text()) == nullptr)
			return;
	m_tree_map[ui->map->text()][t_id].osm_id = getCoor(ui->OSM_ID->text());
	m_tree_map[ui->map->text()][t_id].coor_x = getCoor(ui->CoorX->text());
	m_tree_map[ui->map->text()][t_id].coor_y = getCoor(ui->CoorY->text());
}

/*void TreeDialog::setCoorData(const QString &x, const QString &y)
{
	ui->CoorX->setText(x);
	ui->CoorY->setText(y);
}*/

void TreeDialog::on_treeSelect_activated(int index)
{
	QString select = ui->treeSelect->currentText();
	ui->type->setText(select);
}

QString TreeDialog::createXmlPair(const QString &tag, const QString &name, const QString &value)
{
	QString ret = "<" + tag + " k=\"" + name + "\" " + "v=\"" + value + "\"/>";
	return ret;
}

QString TreeDialog::createXmlInt(const QString &tag, const QString &name, int64_t value)
{
	return createXmlPair(tag, name, QString::number(value));
}

QString TreeDialog::createXmlDouble(const QString &tag, const QString &name, double value)
{
	return createXmlPair(tag, name, QString::number(value, 'g', 10));
}

uint64_t TreeDialog::relFlags()
{
	uint64_t flags = 0x0000000000000000;

	if(ui->rel_roots_1->isChecked())
		flags |= BUND_REL_ROOT_1;
	if(ui->rel_roots_2->isChecked())
		flags |= BUND_REL_ROOT_2;
	if(ui->rel_roots_3->isChecked())
		flags |= BUND_REL_ROOT_3;
	if(ui->rel_roots_4->isChecked())
		flags |= BUND_REL_ROOT_4;
	if(ui->rel_roots_5->isChecked())
		flags |= BUND_REL_ROOT_5;
	if(ui->rel_roots_6->isChecked())
		flags |= BUND_REL_ROOT_6;
	if(ui->rel_roots_7->isChecked())
		flags |= BUND_REL_ROOT_7;
	if(ui->rel_roots_8->isChecked())
		flags |= BUND_REL_ROOT_8;
	if(ui->rel_planting_1->isChecked())
		flags |= BUND_REL_PLANT_1;
	if(ui->rel_planting_2->isChecked())
		flags |= BUND_REL_PLANT_2;
	if(ui->rel_planting_3->isChecked())
		flags |= BUND_REL_PLANT_3;
	return flags;
}

void TreeDialog::relFlagsToGui(uint64_t flags)
{
	if(flags & BUND_REL_ROOT_1)
		ui->rel_roots_1->setCheckState(Qt::Checked);
	else
		ui->rel_roots_1->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_2)
		ui->rel_roots_2->setCheckState(Qt::Checked);
	else
		ui->rel_roots_2->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_3)
		ui->rel_roots_3->setCheckState(Qt::Checked);
	else
		ui->rel_roots_3->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_4)
		ui->rel_roots_4->setCheckState(Qt::Checked);
	else
		ui->rel_roots_4->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_5)
		ui->rel_roots_5->setCheckState(Qt::Checked);
	else
		ui->rel_roots_5->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_6)
		ui->rel_roots_6->setCheckState(Qt::Checked);
	else
		ui->rel_roots_6->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_7)
		ui->rel_roots_7->setCheckState(Qt::Checked);
	else
		ui->rel_roots_7->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_ROOT_8)
		ui->rel_roots_8->setCheckState(Qt::Checked);
	else
		ui->rel_roots_8->setCheckState(Qt::Unchecked);

	if(flags & BUND_REL_PLANT_1)
		ui->rel_planting_1->setCheckState(Qt::Checked);
	else
		ui->rel_planting_1->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_PLANT_2)
		ui->rel_planting_2->setCheckState(Qt::Checked);
	else
		ui->rel_planting_2->setCheckState(Qt::Unchecked);
	if(flags & BUND_REL_PLANT_3)
		ui->rel_planting_3->setCheckState(Qt::Checked);
	else
		ui->rel_planting_3->setCheckState(Qt::Unchecked);
}


uint64_t TreeDialog::condFlags()
{
	uint64_t flags = 0x0000000000000000;

	if(ui->cond_root_1->isChecked())
		flags |= BUND_COND_ROOT_1;
	if(ui->cond_root_2->isChecked())
		flags |= BUND_COND_ROOT_2;
	if(ui->cond_root_3->isChecked())
		flags |= BUND_COND_ROOT_3;
	if(ui->cond_root_4->isChecked())
		flags |= BUND_COND_ROOT_4;

	if(ui->cond_stem_1->isChecked())
		flags |= BUND_COND_STEM_1;
	if(ui->cond_stem_2->isChecked())
		flags |= BUND_COND_STEM_2;
	if(ui->cond_stem_3->isChecked())
		flags |= BUND_COND_STEM_3;
	if(ui->cond_stem_4->isChecked())
		flags |= BUND_COND_STEM_4;
	if(ui->cond_stem_5->isChecked())
		flags |= BUND_COND_STEM_5;
	if(ui->cond_stem_6->isChecked())
		flags |= BUND_COND_STEM_6;
	if(ui->cond_stem_7->isChecked())
		flags |= BUND_COND_STEM_7;
	if(ui->cond_stem_8->isChecked())
		flags |= BUND_COND_STEM_8;

	if(ui->cond_crown_1->isChecked())
		flags |= BUND_COND_CROWN_1;
	if(ui->cond_crown_2->isChecked())
		flags |= BUND_COND_CROWN_2;
	if(ui->cond_crown_3->isChecked())
		flags |= BUND_COND_CROWN_3;
	if(ui->cond_crown_4->isChecked())
		flags |= BUND_COND_CROWN_4;
	if(ui->cond_crown_5->isChecked())
		flags |= BUND_COND_CROWN_5;
	if(ui->cond_crown_6->isChecked())
		flags |= BUND_COND_CROWN_6;
	if(ui->cond_crown_7->isChecked())
		flags |= BUND_COND_CROWN_7;

	return flags;
}

void TreeDialog::condFlagsToGui(uint64_t flags)
{
	if(flags & BUND_COND_ROOT_1)
		ui->cond_root_1->setCheckState(Qt::Checked);
	else
		ui->cond_root_1->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_ROOT_2)
		ui->cond_root_2->setCheckState(Qt::Checked);
	else
		ui->cond_root_2->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_ROOT_3)
		ui->cond_root_3->setCheckState(Qt::Checked);
	else
		ui->cond_root_3->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_ROOT_4)
		ui->cond_root_4->setCheckState(Qt::Checked);
	else
		ui->cond_root_4->setCheckState(Qt::Unchecked);

	if(flags & BUND_COND_STEM_1)
		ui->cond_stem_1->setCheckState(Qt::Checked);
	else
		ui->cond_stem_1->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_2)
		ui->cond_stem_2->setCheckState(Qt::Checked);
	else
		ui->cond_stem_2->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_3)
		ui->cond_stem_3->setCheckState(Qt::Checked);
	else
		ui->cond_stem_3->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_4)
		ui->cond_stem_4->setCheckState(Qt::Checked);
	else
		ui->cond_stem_4->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_5)
		ui->cond_stem_5->setCheckState(Qt::Checked);
	else
		ui->cond_stem_5->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_6)
		ui->cond_stem_6->setCheckState(Qt::Checked);
	else
		ui->cond_stem_6->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_7)
		ui->cond_stem_7->setCheckState(Qt::Checked);
	else
		ui->cond_stem_7->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_STEM_8)
		ui->cond_stem_8->setCheckState(Qt::Checked);
	else
		ui->cond_stem_8->setCheckState(Qt::Unchecked);

	if(flags & BUND_COND_CROWN_1)
		ui->cond_crown_1->setCheckState(Qt::Checked);
	else
		ui->cond_crown_1->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_2)
		ui->cond_crown_2->setCheckState(Qt::Checked);
	else
		ui->cond_crown_2->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_3)
		ui->cond_crown_3->setCheckState(Qt::Checked);
	else
		ui->cond_crown_3->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_4)
		ui->cond_crown_4->setCheckState(Qt::Checked);
	else
		ui->cond_crown_4->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_5)
		ui->cond_crown_5->setCheckState(Qt::Checked);
	else
		ui->cond_crown_5->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_6)
		ui->cond_crown_6->setCheckState(Qt::Checked);
	else
		ui->cond_crown_6->setCheckState(Qt::Unchecked);
	if(flags & BUND_COND_CROWN_7)
		ui->cond_crown_7->setCheckState(Qt::Checked);
	else
		ui->cond_crown_7->setCheckState(Qt::Unchecked);
}


int64_t TreeDialog::getID(const QString & value)
{
	bool ok;
	int64_t num = value.toLongLong(&ok);
	if (!ok)
	{
		//cout << "Conversion failed" <<endl;
		return 0;
	}
	return num;
}

uint64_t TreeDialog::getFlags(const QString &value)
{
	bool ok;
	if(!value.startsWith("0x"))
		return 0;
	QString hex = value.mid(2);
	uint64_t flags = hex.toULongLong(&ok, 16);
	if (!ok)
	{
		//cout << "Conversion failed" <<endl;
		return 0;
	}
	return flags;
}

double TreeDialog::getCoor(const QString &value)
{
	bool ok;
	double num = value.toDouble(&ok);
	if (!ok)
	{
		//cout << "Conversion failed" <<endl;
		return 0;
	}
	return num;
}

Qt::CheckState TreeDialog::getCheckState(const QString & value)
{
	if(value == "2")
		return Qt::Checked;
	return Qt::Unchecked;
}

void TreeDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	enum QDialogButtonBox::StandardButton std = ui->buttonBox->standardButton(button);
	if(std == QDialogButtonBox::Ok)
	{
		m_ok_clicked = true;
		accept();
	}
	if(std == QDialogButtonBox::Save)
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Tree File"),
					".", tr("Tree File (*.xml)"));
		if(fileName.isEmpty())
		{
			// Warnig...
			return;
		}
		save(fileName);
	}
	if(std == QDialogButtonBox::Open)
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open Tree File"),
					".", tr("Tree File (*.xml)"));
		if(fileName.isEmpty())
		{
			// Warnig...
			return;
		}
		read(fileName);
	}
}

void TreeDialog::on_buttonBox_accepted()
{
	// copy data from gui to db
	QString nr = QString::number(ui->treeNr->value());

	if(ui->map->text().size() && nr.size())
		addTreeNode(ui->map->text(), nr);

	TreeElement * elem = getTreeNodeData(ui->map->text(), nr);
	if((elem == nullptr) || (m_ok_clicked == false))
		return;

	guiToTreelist(ui->map->text(), ui->treeNr->value());

	m_ok_clicked = false;
}

void TreeDialog::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
	if(item == nullptr)
		return;
	TreeElement tree;
	elementToGui(tree);
	if(item->childCount() == 0)
		;//return;
	QTreeWidgetItem * parent = item->parent();
	if(parent == nullptr)
		return;
	QString base = parent->data(column, Qt::DisplayRole).toString();
	QVariant text = item->data(column, Qt::DisplayRole);
	ui->map->setText(base);
	ui->treeNr->setValue(getID(text.toString()));
	TreeElement * elem = getTreeNodeData(base, text.toString());
	if(elem == nullptr)
		return;
	int id = getID(text.toString());

	elementToGui(m_tree_map[base][id]);
}
