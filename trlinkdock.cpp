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


#include "trlinkdock.h"
#include "ui_trlinkdock.h"

TrLinkDock::TrLinkDock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrLinkDock)
{
    ui->setupUi(this);
}

TrLinkDock::~TrLinkDock()
{
    delete ui;
}

void TrLinkDock::setData(TrGeoObject* obj)
{
    if(obj == nullptr)
        return;	    
    TrMapLink * link = dynamic_cast<TrMapLink *>(obj); 
    if(link == nullptr)
    {
        ui->label_type_2->setText("---");
        ui->lineEdit_ID->setText("---");
        ui->lineEdit_Head->setText("---");
        ui->lineEdit_Tail->setText("---");
        ui->spinBox_Class->setValue(1);
        ui->spinBox_Lanes->setValue(1);
        return;
    }
    ui->label_type_2->setText("Link");
    ui->lineEdit_ID->setText(QString::number(link->getGeoId()));
    if((link->getNodeFromRef() == nullptr) || (link->getNodeToRef() == nullptr))
    {
        ui->lineEdit_Head->setText("error");
        ui->lineEdit_Tail->setText("error");	
    }
    else
    {    
    	ui->lineEdit_Head->setText(QString::number(link->getNodeFromRef()->getGeoId()));
    	ui->lineEdit_Tail->setText(QString::number(link->getNodeToRef()->getGeoId()));
    }
    ui->spinBox_Class->setValue(link->getRdClass() & 0x0f);
    TrMapLinkRoad * roadlink = dynamic_cast<TrMapLinkRoad *>(link);
    if(roadlink != nullptr)
    	ui->spinBox_Lanes->setValue(roadlink->getLanes());
}