#include "trnodedock.h"
#include "ui_trnodedock.h"

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
    if(node == nullptr)
    {
        ui->lineEdit_ID->setText("---");
        ui->lineEdit_X->setText("---");
        ui->lineEdit_Y->setText("---");
        return;
    }
    ui->lineEdit_ID->setText(QString::number(node->getGeoId()));
    TrPoint pt = node->getPoint();
    ui->lineEdit_X->setText(QString().setNum(pt.x/TR_COOR_FACTOR, 'g', 11));
    ui->lineEdit_Y->setText(QString().setNum(pt.y/TR_COOR_FACTOR, 'g', 11));
}
