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

Ruler::Ruler(QWidget *parent)
	: QWidget{parent}
{
}

void Ruler::showRuler(bool enable)
{
	m_active = enable;
}

void Ruler::paint(QPainter *p)
{
	if(m_active)
		p->fillRect(20,20,100,20, QColor(100,100,100));
}
