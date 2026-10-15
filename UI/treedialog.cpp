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

void TreeDialog::on_treeSelect_activated(int index)
{
	QString select = ui->treeSelect->currentText();
	ui->treeName->setText(select);
}