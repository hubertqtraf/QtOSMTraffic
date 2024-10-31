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

#include "profiledialog.h"
#include "profiledialog.h"
#include "tr_set_model.h"
#include "ui_profiledialog.h"

#include <QDataWidgetMapper>
#include <QDomDocument>
#include <qfile.h>

#include <tr_defs.h>

ProfileDialog::ProfileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileDialog)
    , m_profile(nullptr)
{
    m_profile = new TrSetModel(parent);
    ui->setupUi(this);
}

void ProfileDialog::read(QString & filename)
{
    if(filename == "")
    {
        //TODO:  __FILE__ macro at release mode?
        QString path = QString(__FILE__);
        filename = path.replace("profiledialog.cpp", "tr_default_profile.xml");
    }
    if(m_profile->setDataByFile(filename))
    {
        m_mapper = new QDataWidgetMapper(this);
        ui->profileTree->setModel(m_profile);
        m_mapper->setModel(m_profile);
    }
}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

// filter: trafalgar class (netroad, net, face), "" -> all elements
bool ProfileDialog::readSection(QDomNode & n, QMap<int, QString> & string_map, TrMapCol & col_map, const QString &filter)
{
    if (!n.isElement())
        return false;

    QDomElement e = n.toElement();
    QDomNodeList nl = e.elementsByTagName("value");

    for(int i = 0; i<nl.length(); i++)
    {
        QDomNode n_item = nl.item(i);
        TrSetItem item = TrSetItem(n_item, 0);

        QString name;
        QColor color;
        QString type_name;

        bool has_name = item.setColName(0, name);
        bool has_color = item.setColor(0, color);
        item.getType(0, type_name);
        int row_index = item.getIndex(0);
        //TR_MSG << name << row_index << type_name << filter;

        if(row_index >= 0)
        {
            if(string_map.contains(row_index))
            {
                //TR_MSG << "double use: " << row_index;
            }
            else
            {
                if(has_name)
                {
                    if(filter.size())
                    {
                        if(type_name == filter)
                            string_map[row_index] = name;
                    }
                    else
                        string_map[row_index] = name;
                }
                if(has_color)
                    col_map[row_index] = color;
            }
        }//QList<T> QMap::values() const
    }
    return true;
}



bool ProfileDialog::getElementList(const QString & sec_name, QMap<int, QString> & string_map, TrMapCol & col_map, const QString &filter)
{
    if(m_profile == nullptr)
    {
        return false;
        //TR_MSG << "nullptr";
    }

    string_map.clear();
    //col_map.clear();

    //readSections();
    QDomNode n = m_profile->getDocument()->firstChild();

    // ingore header
    n = n.nextSibling();

    if (!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            QDomNodeList nl = e.elementsByTagName("section");

            //TR_MSG << e.tagName() << nl.length();

            for(int i = 0; i<nl.length(); i++)
            {
                QDomNode n_sec = nl.item(i);
                QDomNamedNodeMap attributeMap = n_sec.attributes();
                QDomNode attribute = attributeMap.namedItem("name");

                //QDomNode filter = attributeMap.namedItem("type");

                if(attribute.nodeValue() == sec_name)
                {
                    //TR_MSG << sec_name;
                    readSection(n_sec, string_map, col_map, filter);
                }
                //TR_MSG << "start" << attribute.nodeValue() << ", " << nl.length();
            }
        }
    }
    return true;
}


QStringList ProfileDialog::getElemStringList(const QString & sec_name, const QString &filter)
{
    QMap<int, QString> string_map;
    TrMapCol col_map;

    if(getElementList(sec_name, string_map, col_map, filter) == true)
    {
        return string_map.values();
    }
    return QList<QString>();
}

ProfileDialog::TrMapCol ProfileDialog::getElemColorMap(const QString & sec_name)
{
    QMap<int, QString> string_map;
    TrMapCol col_map;

    if(getElementList(sec_name, string_map, col_map, "") == true)
    {
        return col_map;
    }
    return TrMapCol();
}



void ProfileDialog::on_treeView_doubleClicked(const QModelIndex &index)
{

}
