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


#ifndef TILEDIALOG_H
#define TILEDIALOG_H

#include <QDialog>

namespace Ui {
class TileDialog;
}

class TileDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TileDialog(QWidget *parent = nullptr);
	~TileDialog();

private:
	Ui::TileDialog *ui;
};

#endif // TILEDIALOG_H
