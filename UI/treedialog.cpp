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
	out << createXmlPair("ttt", "test", "value") << "\n";
	out << createXmlInt("ttt", "test", -77) << "\n";
}

void TreeDialog::on_treeSelect_activated(int index)
{
	QString select = ui->treeSelect->currentText();
	ui->treeName->setText(select);
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
