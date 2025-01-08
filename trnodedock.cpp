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
