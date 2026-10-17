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
	out << createXmlPair(TAG, BUND_POS_MAP, ui->map->text()) << "\n";
	out << createXmlInt(TAG, "BUND:pos:tree_nr", ui->treeNr->value()) << "\n";
	out << createXmlInt(TAG, "BUND:pos:flur_nr", ui->flurNr->value()) << "\n";
	out << createXmlInt(TAG, "BUND:pos:pic_nr", ui->picNr->value()) << "\n";
	out << createXmlPair(TAG, "BUND:pos:road", ui->road->text()) << "\n";
	out << createXmlPair(TAG, "BUND:pos:etc", ui->etc->text()) << "\n";
	out << createXmlPair(TAG, "BUND:pos:act", ui->act->currentText()) << "\n";
	// TODO: label etc.?
	out << createXmlInt(TAG, "BUND:pos:osm_id", ui->OSM_ID->value()) << "\n";

	out << createXmlPair(TAG, "BUND:desc:type", ui->type->text()) << "\n";
	out << createXmlPair(TAG, "BUND:desc:height", ui->treeHeight->currentText()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:single", ui->singleTree->checkState()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:age", ui->age->value()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:diameter", ui->diameter->value()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:count", ui->trunkCount->value()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:diameter:min", ui->diameterMin->value()) << "\n";
	out << createXmlInt(TAG, "BUND:desc:diameter:max", ui->diameterMax->value()) << "\n";
	out << "<tree/>";
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
			if(xml.name().toString() == TAG)
			{
				QXmlStreamAttributes attrs = xml.attributes();
				QString key = attrs.value("k").toString();
				QString value = attrs.value("v").toString();
				if(key == BUND_POS_MAP)
				{
					ui->map->setText(value);
				}
				TR_INF << key << value;
			}
		}
	}
	if (xml.hasError())
	{
		//... do error handling
	}
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
