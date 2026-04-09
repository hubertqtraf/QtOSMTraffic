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


#ifndef RULER_H
#define RULER_H

#include <QWidget>
#include <QPainter>

#include <tr_zoom_map.h>

class Ruler : public QWidget
{
	Q_OBJECT
public:
	explicit Ruler(QWidget *parent = nullptr);

	void showRuler(bool enable);

	void init(const TrZoomMap & zoom_ref);

	virtual void paint(QPainter * p);
signals:

private:
	bool m_active;
	double m_dist;
};

#endif // RULER_H
