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


TreeDialog::TreeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TreeDialog)
{
	ui->setupUi(this);

	ui->treeWidget->setColumnCount(2);
	//for (int i = 0; i < 2; ++i)
	//	new QTreeWidgetItem(ui->treeWidget, { tr("item: %1").arg(i) });
}

TreeDialog::~TreeDialog()
{
	delete ui;
}

void TreeDialog::save(const QString &fname)
{
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << "<tree>\n";
	saveTree(out, "xxx", 0);
	out << "<tree/>";
}

void TreeDialog::saveTree(QTextStream &out, const QString &map, int local_id)
{
	out << createXmlPair(TAG, BUND_POS_MAP, ui->map->text()) << "\n";
	out << createXmlInt(TAG, BUND_POS_TREE_NR , ui->treeNr->value()) << "\n";
	out << createXmlInt(TAG, BUND_POS_FLUR, ui->flurNr->value()) << "\n";
	out << createXmlInt(TAG, BUND_POS_PIC_NR, ui->picNr->value()) << "\n";
	out << createXmlPair(TAG, BUND_POS_ROAD, ui->road->text()) << "\n";
	out << createXmlPair(TAG, BUND_POS_ETC, ui->etc->text()) << "\n";
	out << createXmlInt(TAG, BUND_POS_ACT, ui->act->currentIndex()) << "\n";
	out << createXmlPair(TAG, BUND_POS_OSM_ID, ui->OSM_ID->text()) << "\n";

	out << createXmlPair(TAG, BUND_DESC_TYPE, ui->type->text()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_HEIGHT, ui->treeHeight->currentIndex()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_SINGLE, ui->singleTree->checkState()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_AGE, ui->age->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA, ui->diameter->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_COUNT, ui->trunkCount->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA_MIN, ui->diameterMin->value()) << "\n";
	out << createXmlInt(TAG, BUND_DESC_DIA_MAX, ui->diameterMax->value()) << "\n";
}

void TreeDialog::read(const QString &fname)
{
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QXmlStreamReader xml;
	xml.setDevice(&file);

	while (!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement())
		{
			if(xml.name().toString() == TREE)
			{
				QXmlStreamAttributes attrs = xml.attributes();
				QString ref = attrs.value("map").toString() + "_" +
						attrs.value("local_id").toString();
				if(attrs.value("map").toString() != "")
				{
					QStringList list;
					list.append(attrs.value("map").toString());
					new QTreeWidgetItem(ui->treeWidget, list, Qt::DisplayRole);
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
					ui->flurNr->setValue(getID(value));
				}
				if(key == BUND_POS_PIC_NR)
				{
					ui->picNr->setValue(getID(value));
				}
				if(key == BUND_POS_ROAD)
				{
					ui->road->setText(value);
				}
				if(key == BUND_POS_ETC)
				{
					ui->etc->setText(value);
				}
				if(key == BUND_POS_ACT)
				{
					ui->act->setCurrentIndex(getID(value));
				}
				if(key == BUND_POS_OSM_ID)
				{
					ui->OSM_ID->setText(value);
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
				//TR_INF << key << value;
			}
		}
	}
	if (xml.hasError())
	{
		//... do error handling
	}
}

void TreeDialog::setNodeData(const QString &name, const QString &id)
{
	QStringList list = name.split(u' ');
	Qt::CheckState osm = Qt::Checked;

	if(list.size() >= 3)
	{
		if(list[0] == "ott")
		{
			ui->road->setText(list[1]);
			osm = Qt::Unchecked;
			ui->treeNr->setValue(getID(list.last()));
		}
	}
	else
		ui->road->setText(name);
	ui->inOSM->setCheckState(osm);
	ui->OSM_ID->setText(id);
}

void TreeDialog::setCoorData(const QString &x, const QString &y)
{
	ui->CoorX->setText(x);
	ui->CoorY->setText(y);
}

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
		accept();
	if(std == QDialogButtonBox::Save)
	{
	}
	if(std == QDialogButtonBox::Open)
	{
	}
}

void TreeDialog::on_buttonBox_accepted()
{
}

void TreeDialog::on_treeWidget_itemActivated(QTreeWidgetItem *item, int column)
{
	if(item == nullptr)
		return;
	QVariant text = item->data(column, Qt::DisplayRole);
	ui->map->setText(text.toString());
}
