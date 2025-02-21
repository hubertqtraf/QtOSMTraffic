/******************************************************************
 *
 * @short	basic class for Trafalgar drawing area
 *
 * project:	Trafalgar/GUI
 *
 * modul:	tr_select_box.h	
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2020-2022
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


#ifndef TR_SELECT_BOX_H
#define TR_SELECT_BOX_H

#include <tr_zoom_map.h>

#include <QtWidgets/qrubberband.h>

class SelectBox : public QRubberBand
{
Q_OBJECT
private:
	QPoint m_start;
	QPoint m_next;
	QRubberBand::Shape m_shape;
	bool m_rubber;

public:
	SelectBox(QWidget * parent = nullptr);
	void setFirst(const QPoint pt, bool rshow);
	void setSecond(const QPoint pt, bool rshow);
	void setHide();
	void draw(QPainter *p);

	const QRect & getRect() const;
	QPoint & getStart();
	QPoint & getNext();
	bool isRubber();
	QPoint diff();
	int getManhattanLength();

	void setZoomRect(TrZoomMap & zoom_ref);
};

#endif // TR_SELECT_BOX_H 

