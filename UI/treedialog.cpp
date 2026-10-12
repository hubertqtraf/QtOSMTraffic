#include "treedialog.h"
#include "ui_treedialog.h"

TreeDialog::TreeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TreeDialog)
{
	ui->setupUi(this);
}

TreeDialog::~TreeDialog()
{
	delete ui;
}
