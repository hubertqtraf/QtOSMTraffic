/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024-2025
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


#include "trnodedock.h"
#include "ui_trnodedock.h"

#include <tr_map_poi.h>

TrNodeDock::TrNodeDock(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TrNodeDock)
{
	ui->setupUi(this);
}

TrNodeDock::~TrNodeDock()
{
	delete ui;
}

void TrNodeDock::setData(TrGeoPoint * node)
{
	TrMapPoi * poi = dynamic_cast<TrMapPoi *>(node);

	if(node == nullptr)
	{
		ui->label_type_2->setText("---");
		ui->lineEdit_ID->setText("---");
		ui->lineEdit_X->setText("---");
		ui->lineEdit_Y->setText("---");
		ui->lineEdit_text->setText("---");
		return;
	}
	if(poi != nullptr)
	{
		ui->label_type_2->setText("POI");
		ui->lineEdit_text->setText(poi->getPoiName());
	}
	else
	{
		ui->label_type_2->setText("Node");
		ui->lineEdit_text->setText("---");
	}
	ui->lineEdit_ID->setText(QString::number(node->getGeoId()));
	TrPoint pt = node->getPoint();
	ui->lineEdit_X->setText(QString().setNum(pt.x/TR_COOR_FACTOR, 'g', 11));
	ui->lineEdit_Y->setText(QString().setNum(pt.y/TR_COOR_FACTOR, 'g', 11));
}
