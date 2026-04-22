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

#include <math.h>
#include <tr_geo_object.h>

Ruler::Ruler(QWidget *parent)
	: QWidget{parent}
	, m_active(false)
	, m_dist(1000.0)
	, m_bpt{20,20}
{
}

void Ruler::showRuler(bool enable)
{
	m_active = enable;
}

void Ruler::setBasePoint(const QPoint pt)
{
	m_bpt = pt;
}

void Ruler::init(const TrZoomMap &zoom_ref)
{
	TrPoint pt;
	TrPoint pta;
	pt.x = m_bpt.x();
	pt.y = pta.y = m_bpt.y();
	zoom_ref.getPoint(&pt.x, &pt.y);
	pta.x = m_bpt.x() + 100;
	zoom_ref.getPoint(&pta.x, &pta.y);

	m_dist = fabs(pt.x - pta.x);

	m_base_line.setPoints(4, m_bpt.x()+5, m_bpt.y()+19, m_bpt.x()+5, m_bpt.y()+29,
		m_bpt.x()+105, m_bpt.y()+29, m_bpt.x()+105, m_bpt.y()+19);
}

void Ruler::paint(QPainter *p)
{
	if(!m_active)
		return;

	p->fillRect(m_bpt.x(), m_bpt.y(),110,40, QColor(200, 200, 200, 200));
	p->setPen(QColor(0, 0, 0));
	p->drawPolyline(m_base_line);
	p->drawText(QPoint(m_bpt.x() + 10, m_bpt.y() + 19), QString::number(m_dist) + " [m]");
}
