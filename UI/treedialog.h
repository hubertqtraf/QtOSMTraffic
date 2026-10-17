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


#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QAbstractButton>
#include <QDialog>

#define TAG "tag"

#define BUND_POS_MAP "BUND:pos:map"

namespace Ui {
class TreeDialog;
}

class TreeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TreeDialog(QWidget *parent = nullptr);
	~TreeDialog();

	void save(const QString &fname);
	void read(const QString &fname);

private slots:
	void on_treeSelect_activated(int index);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_buttonBox_accepted();

private:
	Ui::TreeDialog *ui;

	QString createXmlPair(const QString &tag, const QString &name, const QString &value);
	QString createXmlInt(const QString &tag, const QString &name, int value);
};

#endif // TREEDIALOG_H
