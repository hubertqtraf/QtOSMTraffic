/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2026
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


#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QAbstractButton>
#include <QAbstractItemModel>
#include <QDialog>
#include <QTreeWidget>

#define TAG "tag"
#define TREE "tree"

#define BUND_POS_MAP "BUND:pos:map"
#define BUND_POS_TREE_NR "BUND:pos:tree_nr"
#define BUND_POS_FLUR "BUND:pos:flur_nr"
#define BUND_POS_PIC_NR "BUND:pos:pic_nr"
#define BUND_POS_ROAD "BUND:pos:road"
#define BUND_POS_ETC "BUND:pos:etc"
#define BUND_POS_ACT "BUND:pos:act"

#define BUND_POS_OSM_INC "BUND:pos:osm_inc"
#define BUND_POS_OSM_ID "BUND:pos:osm_id"
#define BUND_POS_COOR_X "BUND:pos:coor_x"
#define BUND_POS_COOR_Y "BUND:pos:coor_y"

#define BUND_DESC_TYPE "BUND:desc:type"
#define BUND_DESC_HEIGHT "BUND:desc:height"
#define BUND_DESC_SINGLE "BUND:desc:single"
#define BUND_DESC_AGE "BUND:desc:age"
#define BUND_DESC_DIA "BUND:desc:diameter"
#define BUND_DESC_COUNT "BUND:desc:count"
#define BUND_DESC_DIA_MIN "BUND:desc:diameter:min"
#define BUND_DESC_DIA_MAX "BUND:desc:diameter:max"

#define BUND_REL_FLAGS "BUND:relation:flags"
#define BUND_COND_FLAGS "BUND:cond:flags"

#define BUND_COND_ROOT_1  0x0000000000000001
#define BUND_COND_ROOT_2  0x0000000000000002
#define BUND_COND_ROOT_3  0x0000000000000004
#define BUND_COND_ROOT_4  0x0000000000000008

#define BUND_COND_STEM_1  0x0000000000010000
#define BUND_COND_STEM_2  0x0000000000020000
#define BUND_COND_STEM_3  0x0000000000040000
#define BUND_COND_STEM_4  0x0000000000080000
#define BUND_COND_STEM_5  0x0000000000100000
#define BUND_COND_STEM_6  0x0000000000200000
#define BUND_COND_STEM_7  0x0000000000400000
#define BUND_COND_STEM_8  0x0000000000800000

#define BUND_COND_CROWN_1 0x0000000100000000
#define BUND_COND_CROWN_2 0x0000000200000000
#define BUND_COND_CROWN_3 0x0000000400000000
#define BUND_COND_CROWN_4 0x0000000800000000
#define BUND_COND_CROWN_5 0x0000001000000000
#define BUND_COND_CROWN_6 0x0000002000000000
#define BUND_COND_CROWN_7 0x0000004000000000

class TreeElement
{
public:
	TreeElement();

	int flur_nr;
	int pic_nr;
	QString road;
	QString etc;

	int act;
	int osm_id;
	int osm_inc;

	double coor_x;
	double coor_y;

	QString tree_type;
	int height;
	int single;
	int age;
	int dia;
	int stem_count;
	int dia_min;
	int dia_max;

	uint64_t rel_flags;
	uint64_t cond_flags;
};

namespace Ui {
class TreeDialog;
}


class TreeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TreeDialog(QWidget *parent = nullptr);
	~TreeDialog();

	void save(const QString &fname);
	void saveTree(QTextStream &out, const QString &map, int local_id);
	void read(const QString &fname);

	void setNodeData(const QString &name, const QString &osm_id,
					 const QString &x, const QString &y);

	void addTreeNode(const QString &page, const QString &nr);

	QTreeWidgetItem *getTreeItem(QTreeWidgetItem *base, const QString &node);

	QTreeWidgetItem *getBaseTreeItem(QTreeWidgetItem *base, const QString &node);

private slots:
	void on_treeSelect_activated(int index);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_buttonBox_accepted();

	void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

private:
	Ui::TreeDialog *ui;

	bool m_ok_clicked;
	QMap<QString, QMap<int, TreeElement>> m_tree_map;
	TreeElement *getTreeNodeData(const QString &page, const QString &nr);

	QString createXmlPair(const QString &tag, const QString &name, const QString &value);
	QString createXmlInt(const QString &tag, const QString &name, int value);
	QString createXmlDouble(const QString &tag, const QString &name, double value);

	uint64_t condFlags();
	void condFlagsToGui(uint64_t flags);

	int getID(const QString &value);
	uint64_t getFlags(const QString &value);
	double getCoor(const QString &value);
	Qt::CheckState getCheckState(const QString & value);
	void addTreeNodeData(const QString &page, const QString &nr);
};

#endif // TREEDIALOG_H
