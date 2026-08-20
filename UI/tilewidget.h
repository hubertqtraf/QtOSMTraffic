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

#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include "tr_document.h"

#include <QProgressBar>
#include <QWidget>

class TileWidget : public QWidget
{
	Q_OBJECT
private:
	TrDocument *m_doc;
	TrZoomMap m_zoom_ref;
	QProgressBar *m_bar;
	int m_level;
	int m_x;
	int m_y;
	int m_start_x;
	QString m_path;
	QVector<double> m_rect;

	bool createDir(const QString &path);

public:
	explicit TileWidget(QWidget *parent = nullptr);

	bool m_move;
	bool m_active;
	bool m_copy;
	QColor m_background;

	int lon2TileX(double lon);
	int lat2TileY(double lat);
	double tileX2Lon(int x);
	double tileY2Lat(int y);

	void setDocument(TrDocument * doc);
	void setRect(const QVector<double> &rect);
	QVector<double> getRect();
	void setLevel(int level);
	int getLevel();
	bool resetX(int &x, int &y);

	void setBasePath(const QString & path);
	QString getPath(QVector<int> & data);
	QString getCoorPath(double lon, double lat);
	QString getTilePath(int x, int y);
	bool setLavelPathCoor(double lon, double lat);
	bool setLavelPath(int x, int y);

	void recalcExtRect();
	void recalcExtRect(int x, int y);
	void setProgressBar(QProgressBar *bar);
	void createPngImage(QImage &image);
	virtual void paint(QPainter * p);

	void createPngImageByPath(const QString &path);

signals:
	void posChanged(int x, int y);
	void valueChanged(int value);
};

#endif // TILEWIDGET_H
