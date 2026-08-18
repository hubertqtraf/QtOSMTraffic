/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2026-2026
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


#include "tiledialog.h"
#include "ui_tiledialog.h"

#include <QFileDialog>

TileDialog::TileDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TileDialog),
	m_tile(nullptr)
{
	ui->setupUi(this);
}

TileDialog::~TileDialog()
{
	delete ui;
}

void TileDialog::setTile(TileWidget * tile)
{
	m_tile = tile;
}

void TileDialog::setRect(QVector<double> rect)
{
	if(m_tile == nullptr)
		return;
	m_tile->setRect(rect);
	if(rect.size() < 3)
		return;
	ui->lon1->setValue(rect[0]);
	ui->lon2->setValue(rect[1]);
	ui->lat1->setValue(rect[2]);
	ui->lat2->setValue(rect[3]);
	ui->x1->setValue(m_tile->lon2TileX(rect[0]));
	ui->x2->setValue(m_tile->lon2TileX(rect[1]));
	ui->y1->setValue(m_tile->lat2TileY(rect[2]));
	ui->y2->setValue(m_tile->lat2TileY(rect[3]));
}

void TileDialog::on_BaseDirSelect_clicked()
{
	QFileDialog dialog;
#if QT_VERSION < 0x060000
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);
#else
	dialog.setFileMode(QFileDialog::Directory);
#endif
	QString oldPath = ui->BaseDir->text();
	if(!oldPath.size())
		oldPath = ".";
	//TR_INF << oldPath;
	dialog.setDirectory(oldPath);
	dialog.exec();
	ui->BaseDir->clear();
	ui->BaseDir->insert(dialog.directory().path());
}


void TileDialog::on_pushButton_clicked()
{
	if(m_tile == nullptr)
		return;
	if(!ui->BaseDir->text().size())
	{
		// TODO: box
		return;
	}
	QFile file(ui->BaseDir->text() + "/index_leaflet.html");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);

	QString script = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<base target=\"_top\"><meta charset=\"utf-8\">\n";
	script.append("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<title>Quick Start - Leaflet</title>\n");
	script.append("<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"docs/images/favicon.ico\" />\n");
	script.append("<link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.css\" integrity=\"sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=\" crossorigin=\"\"/>\n");
	script.append("<script src=\"https://unpkg.com/leaflet@1.9.4/dist/leaflet.js\" integrity=\"sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=\" crossorigin=\"\"></script>\n");
	script.append("<style>\nhtml, body {\nheight: 100%;\nmargin: 0;\n}\nleaflet-container {\nheight: 400px;\nwidth: 600px;\nmax-width: 100%;\nmax-height: 100%;\n}\n</style>\n");
	script.append("</head>\n<body>\n<div id=\"map\" style=\"width: 600px; height: 400px;\"></div>\n<script>\n");

	double y_pos = ((ui->lat2->value() - ui->lat1->value()) / 2.0) + ui->lat1->value();
	double x_pos = ((ui->lon2->value() - ui->lon1->value()) / 2.0) + ui->lon1->value();

	out << script;
	out << "var map = L.map('map').setView({lon: " << x_pos << ", lat:" << y_pos << "}," << m_tile->getLevel()  << ");\n";
	out << "L.tileLayer('file:///" << ui->BaseDir->text() << "/{z}/{x}/{y}.png', {\n";
	out << "maxZoom: 19,\n";
	out << "attribution: '&copy; <a href=\"http://www.openstreetmap.org/copyright\">OpenStreetMap</a>'}).addTo(map);\n";

	out << "</script>\n</body>\n</html>";
}


void TileDialog::on_buttonBox_accepted()
{
	if(m_tile == nullptr)
		return;
	m_tile->m_active = true;
	m_tile->setLevel(ui->levelValue->value());
	QVector<double> rect = m_tile->getRect();
	if(rect.size() < 4)
		return;
	//TR_INF << rect;
	m_tile->setBasePath(ui->BaseDir->text());
	m_tile->m_move = true;

	m_tile->recalcExtRect(m_tile->lon2TileX(rect[0]), m_tile->lat2TileY(rect[2]));
}

void TileDialog::on_buttonBox_rejected()
{
	if(m_tile == nullptr)
		return;
	m_tile->m_move = true;
	m_tile->m_active = false;
}

void TileDialog::on_levelValue_valueChanged(int level)
{
	m_tile->setLevel(level);
	ui->x1->setValue(m_tile->lon2TileX(ui->lon1->value()));
	ui->x2->setValue(m_tile->lon2TileX(ui->lon2->value()));
	ui->y1->setValue(m_tile->lat2TileY(ui->lat1->value()));
	ui->y2->setValue(m_tile->lat2TileY(ui->lat2->value()));
}
