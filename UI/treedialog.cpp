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
#include <tr_geo_object.h>

#include "treedialog.h"
#include "ui_treedialog.h"


TreeElement::TreeElement()
	: flur_nr(0)
	, pic_nr(0)
	, road{""}
	, etc{""}

	, coor_x(0.0)
	, coor_y(0.0)

	, rel_flags(0x0000000000000000)
	, cond_flags(0x0000000000000000)
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
	QString map;

	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	for (auto i = m_tree_map.cbegin(), end = m_tree_map.cend(); i != end; ++i)
	{
		QMap<int, TreeElement> nr_map = i.value();
		for (auto j = nr_map.cbegin(), end = nr_map.cend(); j != end; ++j)
		{
			out << "<tree map=\"" << i.key() << "\" " << "local_id=\""  << j.key() << "\"" << ">\n";
			saveTree(out,i.key(), j.key());
			out << "<tree/>\n";
		}
	}
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

void TreeDialog::saveTree(QTextStream &out, const QString &map, int local_id)
{
	TreeElement * elem = getTreeNodeData(map, QString::number(local_id));
	if(elem == nullptr)
		return;
	out << createXmlPair(TAG, BUND_POS_MAP, map) << "\n";
	out << createXmlInt(TAG, BUND_POS_TREE_NR , local_id) << "\n";
	out << createXmlInt(TAG, BUND_POS_FLUR, m_tree_map[map][local_id].flur_nr) << "\n";
	out << createXmlInt(TAG, BUND_POS_PIC_NR, m_tree_map[map][local_id].pic_nr) << "\n";
	out << createXmlPair(TAG, BUND_POS_ROAD, m_tree_map[map][local_id].road) << "\n";
	out << createXmlPair(TAG, BUND_POS_ETC, m_tree_map[map][local_id].etc) << "\n";
	out << createXmlInt(TAG, BUND_POS_ACT, ui->act->currentIndex()) << "\n";
	out << createXmlPair(TAG, BUND_POS_OSM_ID, ui->OSM_ID->text()) << "\n";

	out << createXmlDouble(TAG, BUND_POS_COOR_X, m_tree_map[map][local_id].coor_x) << "\n";
	out << createXmlDouble(TAG, BUND_POS_COOR_Y, m_tree_map[map][local_id].coor_y) << "\n";

	out << createXmlPair(TAG, BUND_DESC_TYPE, ui->type->text()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_HEIGHT, ui->treeHeight->currentIndex()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_SINGLE, ui->singleTree->checkState()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_AGE, ui->age->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA, ui->diameter->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_COUNT, ui->trunkCount->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA_MIN, ui->diameterMin->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA_MAX, ui->diameterMax->value()) << "\n";

	QString rel_flags = "0x" + QString::number(m_tree_map[map][local_id].rel_flags, 16);
	out << createXmlPair(TAG, BUND_REL_FLAGS, rel_flags) << "\n";

	QString cond_flags = "0x" + QString::number(m_tree_map[map][local_id].cond_flags, 16);
	out << createXmlPair(TAG, BUND_COND_FLAGS, cond_flags) << "\n";
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
					m_tree_map[local_map][local_id].flur_nr = getID(value);
				}
				if(key == BUND_POS_PIC_NR)
				{
					m_tree_map[local_map][local_id].pic_nr = getID(value);
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
					ui->act->setCurrentIndex(getID(value));
				}
				if(key == BUND_POS_OSM_ID)
				{
					ui->OSM_ID->setText(value);
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
					ui->type->setText(value);
				}
				if(key == BUND_DESC_HEIGHT)
				{
					ui->treeHeight->setCurrentIndex(getID(value));
				}
				if(key == BUND_DESC_SINGLE)
				{
					ui->singleTree->setCheckState(getCheckState(value));
				}
				if(key == BUND_DESC_AGE)
				{
					ui->age->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA)
				{
					ui->diameter->setValue(getID(value));
				}
				if(key == BUND_DESC_COUNT)
				{
					ui->trunkCount->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA_MIN)
				{
					ui->diameterMin->setValue(getID(value));
				}
				if(key == BUND_DESC_DIA_MAX)
				{
					ui->diameterMax->setValue(getID(value));
				}

				if(key == BUND_COND_FLAGS)
				{
					uint64_t flags = getFlags(value);
					m_tree_map[local_map][local_id].cond_flags = flags;
				}
				if(key == BUND_REL_FLAGS)
				{
					uint64_t flags = getFlags(value);
					m_tree_map[local_map][local_id].rel_flags = flags;
				}
				//TR_INF << key << value;
			}
		}
		if(xml.isEndElement())
		{
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
							 const QString &x, const QString &y)
{
	QStringList list = name.split(u' ');
	Qt::CheckState osm = Qt::Checked;

	ui->tree_tab->setCurrentIndex(0);

	int t_id = 0;

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
	if(getTreeNodeData(ui->map->text(), ui->treeNr->text()) == nullptr)
			return;
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

QString TreeDialog::createXmlInt(const QString &tag, const QString &name, int value)
{
	return createXmlPair(tag, name, QString::number(value));
}

QString TreeDialog::createXmlDouble(const QString &tag, const QString &name, double value)
{
	return createXmlPair(tag, name, QString::number(value, 'g', 10));
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


int TreeDialog::getID(const QString & value)
{
	bool ok;
	int num = value.toInt(&ok);
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
	QString hex = value.last(value.size()-2);
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
	}
	if(std == QDialogButtonBox::Open)
	{
	}
}

void TreeDialog::on_buttonBox_accepted()
{
	// copy data from gui to db
	ui->map->text();
	QString nr = QString::number(ui->treeNr->value());
	TreeElement * elem = getTreeNodeData(ui->map->text(), nr);
	if((elem == nullptr) || (m_ok_clicked == false))
		return;
	m_tree_map[ui->map->text()][ui->treeNr->value()].flur_nr = ui->flurNr->value();
	m_tree_map[ui->map->text()][ui->treeNr->value()].pic_nr = ui->picNr->value();
	m_tree_map[ui->map->text()][ui->treeNr->value()].road = ui->road->text();
	m_tree_map[ui->map->text()][ui->treeNr->value()].etc = ui->etc->text();

	m_tree_map[ui->map->text()][ui->treeNr->value()].coor_x = getCoor(ui->CoorX->text());
	m_tree_map[ui->map->text()][ui->treeNr->value()].coor_y = getCoor(ui->CoorY->text());

	//m_tree_map[ui->map->text()][ui->treeNr->value()].rel_flags = relFlags();
	m_tree_map[ui->map->text()][ui->treeNr->value()].cond_flags = condFlags();

	//ui->tree_tab->setCurrentIndex(2);

	m_ok_clicked = false;
}

void TreeDialog::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
	if(item == nullptr)
		return;
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
	ui->flurNr->setValue(m_tree_map[base][id].flur_nr);
	ui->picNr->setValue(m_tree_map[base][id].pic_nr);
	ui->road->setText(m_tree_map[base][id].road);
	ui->etc->setText(m_tree_map[base][id].etc);

	ui->CoorX->setText(QString::number(m_tree_map[base][id].coor_x, 'g', 10));
	ui->CoorY->setText(QString::number(m_tree_map[base][id].coor_y, 'g', 10));
	//ui->flurNr->setValue(m_tree->flur_nr);

	//relFlagsToGui(m_tree_map[base][id].rel_flags);
	condFlagsToGui(m_tree_map[base][id].cond_flags);

	ui->tree_tab->setCurrentIndex(1);
}
