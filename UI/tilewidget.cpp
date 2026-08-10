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

#include "tilewidget.h"

#include <qapplication.h>
#include <qdir.h>
#include <qpainter.h>


TileWidget::TileWidget(QWidget *parent)
	: QWidget{parent}
	, m_level(1)
	, m_x(0)
	, m_y(0)
	, m_start_x(0)
	, m_rect{400.0, 400.0, 400,0, 400,0}
	, m_move(false)
{
	m_zoom_ref.setScreenDimension(256,256);
	//TR_INF << "! " << size();
	resize(1,1);
}

void TileWidget::setDocument(TrDocument *doc)
{
	m_doc = doc;
}

int TileWidget::lon2TileX(double lon)
{
	return static_cast<int32_t>(std::floor((lon + 180.0) / 360.0 * (1 << m_level)));
}

int TileWidget::lat2TileY(double lat)
{
	// Convert latitude to radians
	double lat_rad = lat * M_PI / 180.0;
	return static_cast<int32_t>(std::floor((1.0 - std::log(std::tan(lat_rad) + (1.0 /
		std::cos(lat_rad))) / M_PI) / 2.0 * (1 << m_level)));
}

double TileWidget::tileX2Lon(int x)
{
	return static_cast<double>(x) / (1 << m_level) * 360.0 - 180.0;
}

double TileWidget::tileY2Lat(int y)
{
	double lat_rad = std::atan(std::sinh(M_PI * (1.0 - 2.0 * static_cast<double>(y) / (1 << m_level))));
	return lat_rad * 180.0 / M_PI;
}

void TileWidget::setBasePath(const QString &path)
{
	m_path = path;
}

bool TileWidget::createDir(const QString & path)
{
	QDir dir(path);
	if(dir.exists())
	{
		return true;
	}
	if(dir.mkdir(path) == false)
	{
		qWarning("Cannot craete directory");
		return false;
	}
	return true;
}

QString TileWidget::getPath(QVector<int> & data)
{
	QString path;

	int tile_x = 0;
	int tile_y = 0;
	//if(!getTileCoor(false, tile_x, tile_y))
	//        return path;

	// TODO: param for dir
	path = m_path +
				QString::number(m_level) + "/" +
				QString::number(tile_x) + "/" +
				//QString::number(tile_y-1) + ".png";
				QString::number(tile_y) + ".png";

	/*data.clear();
	data.append(m_level);
	data.append(tile_x);
	data.append(tile_y);*/
	TR_INF << path;

	return path;
}

bool TileWidget::resetX(int &x, int &y)
{
	int limit_y = lat2TileY(m_rect[3]); //m_tile->getRect().at(3));
	if(y <= limit_y)
	{
		TR_INF << "limit Y" << limit_y; //<< y;
		m_x++;
		x = m_x;
		m_y = lat2TileY(m_rect[2]);
		y = m_y;
		return true;
	}
	return false;
}

bool TileWidget::setLavelPathCoor(double lon, double lat)
{
	return setLavelPath(lon2TileX(lon), lat2TileY(lat));
}

QString TileWidget::getTilePath(int x, int y)
{
	return m_path + QString::number(m_level) + "/" +
			QString::number(x) + "/" +
			QString::number(y) + ".png";
}

QString TileWidget::getCoorPath(double lon, double lat)
{
	return getTilePath(lon2TileX(lon), lat2TileY(lat));
}

bool TileWidget::setLavelPath(int x, int y)
{
	QString path = m_path + QString::number(m_level) + "/";

	if(createDir(path) == false)
		return false;
	path.append(QString::number(x) + "/");
	if(createDir(path) == false)
		return false;
	return true;
}

void TileWidget::setRect(const QVector<double> &rect)
{
	m_rect = rect;
	m_x = lon2TileX(m_rect[1]);
	m_y = lat2TileY(m_rect[2]);
}

QVector<double> TileWidget::getRect()
{
	return 	m_rect;
}

void TileWidget::setLevel(int level)
{
	m_level = level;
}

void TileWidget::recalcExtRect(int x, int y)
{
	m_x = x;
	m_y = y;

	//TR_INF << x << y << m_move;
	y+=1;
	double lon1 = tileX2Lon(x);
	double lat1 = tileY2Lat(y);
	x+=1;
	y+=1;
	double lon2 = tileX2Lon(x);
	double lat2 = tileY2Lat(y);
	m_zoom_ref.setVisibleWorld(lon1 * 100000, lat1 * 100000, lon2 * 100000, lat2 * 100000);
	m_zoom_ref.zoom2Rect();
	m_move = true;
	update();
}

void TileWidget::recalcExtRect()
{
}

void TileWidget::paint(QPainter *p)
{
	p->setBrush(QBrush(m_background));
	p->drawRect(0,0, 257, 257);

	if(m_doc != nullptr)
	{
		if(m_doc->m_is_loaded)
			m_doc->draw(m_zoom_ref, p, 0);
	}

	if(m_move)
	{
		//TR_INF << m_x << m_y;
		m_move = false;
		emit posChanged(m_x, m_y);
	}
}

void TileWidget::createPngImage(QImage & image)
{
	QPainter painter;

	painter.begin(&image);
	paint(&painter);

	render(&painter);
	painter.end();
}

void TileWidget::createPngImageByPath(const QString &path)
{
	QImage img(256, 256, QImage::Format_ARGB32);
	createPngImage(img);
	img.save(path);
}