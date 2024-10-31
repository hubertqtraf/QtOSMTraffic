#include "trworldlayer.h"
#include "ui_trworldlayer.h"

TrWorldLayer::TrWorldLayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrWorldLayer)
{
    ui->setupUi(this);
}

TrWorldLayer::~TrWorldLayer()
{
    delete ui;
}
