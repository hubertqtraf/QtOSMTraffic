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

#include "about.h"
#include "ui_about.h"

#include "tr_geo_object.h"

#include <qfile.h>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    QString path = QString(__FILE__);
    path.replace("about.cpp", "LICENSE");
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
            return;
    QByteArray data=file.readAll();
    ui->textEdit->setPlainText(data);
    file.close();
}

About::~About()
{
    delete ui;
}
