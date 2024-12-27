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
        return;
    ui->lineEdit_ID->setText(QString::number(node->getGeoId()));
    TrPoint pt = node->getPoint();
    ui->lineEdit_X->setText(QVariant(pt.x/TR_COOR_FACTOR).toString());
    ui->lineEdit_Y->setText(QVariant(pt.y/TR_COOR_FACTOR).toString());
}
