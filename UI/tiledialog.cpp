/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2026-2026
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


#include "tiledialog.h"
#include "ui_tiledialog.h"

#include <QFileDialog>

TileDialog::TileDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TileDialog),
	m_tile(nullptr)
{
	ui->setupUi(this);
}

TileDialog::~TileDialog()
{
	delete ui;
}

void TileDialog::setTile(TileWidget * tile)
{
	m_tile = tile;
}

void TileDialog::setRect(QVector<double> rect)
{
	m_tile->setRect(rect);
}

void TileDialog::on_BaseDirSelect_clicked()
{
	QFileDialog dialog;
#if QT_VERSION < 0x060000
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);
#else
	dialog.setFileMode(QFileDialog::Directory);
#endif
	QString oldPath = ui->BaseDir->text();
	if(!oldPath.size())
		oldPath = ".";
	//TR_INF << oldPath;
	dialog.setDirectory(oldPath);
	dialog.exec();
	ui->BaseDir->clear();
	ui->BaseDir->insert(dialog.directory().path());
}


void TileDialog::on_buttonBox_accepted()
{
	if(m_tile == nullptr)
		return;
	m_tile->m_active = true;
	QVector<double> rect = m_tile->getRect();
	if(rect.size() < 4)
		return;
	//TR_INF << rect;
	m_tile->setBasePath(ui->BaseDir->text());
	m_tile->setLevel(ui->levelValue->value());
	m_tile->m_move = true;

	m_tile->recalcExtRect(m_tile->lon2TileX(rect[0]), m_tile->lat2TileY(rect[3]));
}
