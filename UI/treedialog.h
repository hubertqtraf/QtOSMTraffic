#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QDialog>

namespace Ui {
class TreeDialog;
}

class TreeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TreeDialog(QWidget *parent = nullptr);
	~TreeDialog();

private slots:
	void on_treeSelect_activated(int index);

private:
	Ui::TreeDialog *ui;
};

#endif // TREEDIALOG_H
