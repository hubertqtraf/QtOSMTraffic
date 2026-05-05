/******************************************************************
 * project:     OSM Traffic
 *
 * (C)          Schmid Hubert 2026-2026
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


#include "nodestype.h"
#include "ui_nodestype.h"

NodesType::NodesType(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::NodesType)
{
	ui->setupUi(this);
}

NodesType::~NodesType()
{
	delete ui;
}

uint64_t NodesType::getMask()
{
	uint64_t ret = 0;

	if(ui->checkBox_raod->isChecked())
		ret |= 0x0000000000000001;
	if(ui->checkBox_standard->isChecked())
		ret |= 0x0000000000000002;
	if(ui->checkBox_line->isChecked())
		ret |= 0x0000000000000004;
	if(ui->checkBox_end->isChecked())
		ret |= 0x0000000000000008;
	return ret;
}
