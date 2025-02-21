/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024
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

#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include "profile.h"
#include "tr_set_model.h"

#include <QAbstractButton>
#include <QDataWidgetMapper>
#include <QDialog>

// standard file for profile data: "tr_default_profile.xml"

namespace Ui {
class ProfileDialog;
}

class ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QMap<int, QString> TrMapString;
    typedef QMap<int, QColor> TrMapCol;

    explicit ProfileDialog(QWidget *parent = nullptr);
    ~ProfileDialog();

    void read(QString &filename);

    QStringList getElemStringList(const QString &sec_name, const QString &filter = "");
    ProfileDialog::TrMapCol getElemColorMap(const QString &sec_name);

private slots:
    //void on_treeView_doubleClicked(const QModelIndex &index);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ProfileDialog *ui;
    TrSetModel * m_profile;
    QDataWidgetMapper * m_mapper;
    QString m_filename;

    bool getElementList(const QString &sec_name, QMap<int, QString> &string_map, TrMapCol & col_map, const QString &filter);
    bool readSection(QDomNode &n, QMap<int, QString> &string_map, TrMapCol & col_map, const QString &filter);
};

#endif // PROFILEDIALOG_H
