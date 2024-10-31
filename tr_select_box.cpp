/******************************************************************
 *
 * @short	basic class for Trafalgar drawing area
 *
 * project:	Trafalgar/GUI
 *
 * modul:	tr_select_box.cc	
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2020/2022
 *
 * beginning:	09.2020
 *
 * history:
 */
/******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software foundation; either version 2, or (at your
 * option) any later version.
 *
 * The GNU trafalgar package is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with the GNU plotutils package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
 * Boston, MA 02110-1301, USA. 
 */


#include <math.h>

#include <stdio.h>

#include "tr_select_box.h"

#include "tr_geo_object.h"


SelectBox::SelectBox(QWidget * parent)
	: QRubberBand(Rectangle, parent)
	, m_start(0,0)
	, m_next(0,0)
	, m_rubber(false)
{
	hide();
}

void SelectBox::draw(QPainter *p)
{
}

void SelectBox::setFirst(const QPoint pt, bool rshow)
{
	m_start = pt;
	m_rubber = rshow;

	if(rshow)
	{
		show();
		setGeometry(m_start.x(), m_start.y(), m_next.x()-m_start.x(), m_next.y()-m_start.y());
	}
}

void SelectBox::setSecond(const QPoint pt, bool rshow)
{
	m_next = pt;

	if(!m_rubber)
		return;

	if(m_next.x() > m_start.x())
	{
		if(m_next.y() > m_start.y())
			setGeometry(m_start.x(), m_start.y(), m_next.x()-m_start.x(),
				m_next.y() - m_start.y());
		else
			setGeometry(m_start.x(), m_next.y(), m_next.x()-m_start.x(),
				m_start.y()-m_next.y());
	}
	else
	{
		if(m_next.y() > m_start.y())
			setGeometry(m_next.x(), m_start.y(), m_start.x()-m_next.x(),
				m_next.y()-m_start.y());
		else
			setGeometry(m_next.x(), m_next.y(), m_start.x()-m_next.x(),
				m_start.y()-m_next.y());
	}
}

QPoint & SelectBox::getStart()
{
	return m_start;
}

QPoint & SelectBox::getNext()
{
	return m_next;
}

bool SelectBox::isRubber()
{
	return m_rubber;
}

QPoint SelectBox::diff()
{
	return m_next-m_start;
}

int SelectBox::getManhattanLength()
{
	QPoint point = m_next-m_start;

	return point.manhattanLength();
}

void SelectBox::setHide()
{
	hide();
}

const QRect & SelectBox::getRect() const
{
	return geometry();
}

void SelectBox::setZoomRect(TrZoomMap & zoom_ref)
{
	// TODO: set a limit
	if(!width() || !height())
		return;
	//TR_MSG << geometry();
	TrPoint pt1;
    pt1.x = static_cast <double>(x());
    pt1.y = static_cast <double>(y());
	TrPoint pt2;
    pt2.x = static_cast <double>((x()) + width());
    pt2.y = static_cast <double>((y()) + height());
    zoom_ref.getPoint(&pt1.x, &pt1.y);
	zoom_ref.getPoint(&pt2.x, &pt2.y);
	// TODO: return the string...
	QString str = QString("rect: -r %1,%3,%2,%4").arg(pt1.x/TR_COOR_FACTOR).
		arg(pt2.y/TR_COOR_FACTOR).arg(pt2.x/TR_COOR_FACTOR).arg(pt1.y/TR_COOR_FACTOR);
	TR_INF << str;
	zoom_ref.setVisibleWorld(pt1.x, pt2.y, pt2.x, pt1.y);
	zoom_ref.zoom2Rect();
}

