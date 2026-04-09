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


#include "ruler.h"

#include <tr_geo_object.h>

Ruler::Ruler(QWidget *parent)
	: QWidget{parent}
	, m_active(false)
	, m_dist(1000.0)
{
}

void Ruler::showRuler(bool enable)
{
	m_active = enable;
}

void Ruler::init(const TrZoomMap &zoom_ref)
{
	TrPoint pt = {20,25};

	zoom_ref.getPoint(&pt.x, &pt.y);
	TrPoint pta = pt;
	pt.x = 120;
	zoom_ref.getPoint(&pt.x, &pt.y);

	m_dist = fabs(pt.x - pta.x);
}

void Ruler::paint(QPainter *p)
{
	if(m_active)
	{
		p->fillRect(20,20,100,20, QColor(200,200,200));
		p->setPen(QColor(0,0,0));
		p->drawText(QPoint(25,35), QString::number(m_dist) + " [m]");
	}
}
