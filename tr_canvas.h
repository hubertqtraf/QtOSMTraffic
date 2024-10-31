/******************************************************************
 *
 * @short	basic class for Trafalgar drawing area
 *
 * project:	Trafalgar/Gui
 *
 * modul:	tr_canvas.h		drawing area
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2004-2022
 *
 * beginning:	08.2004
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


#ifndef TR_CANVAS_H
#define TR_CANVAS_H

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qmenu.h>

#include "tr_select_box.h"

#define MOUSE_MODE_PRESS 	1
#define MOUSE_MODE_RELEASE 	2
#define MOUSE_MODE_MOVE 	3

#define MIN_SELECT		1	// double click range

#include <QtGui/qevent.h>

// from https://wiki.qt.io/How_to_catch_enter_key/de
class KeyEnterReceiver : public QObject
{
	Q_OBJECT
public:
	KeyEnterReceiver();
	bool eventFilter(QObject* obj, QEvent* event);

signals:
	void getKey(QKeyEvent* key);
};


class TrCanvas : public QWidget
{
Q_OBJECT

private:
	unsigned char flags;

	QPoint m_double_click;
	QColor m_background;
	QFont * m_font;


signals:
	void documentChanged();		// TODO, unused?

protected:
	SelectBox m_select_box;
	int m_count_click;  

	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *event);

	void paintEvent(QPaintEvent *event);

	virtual void paint(QPainter * p);

	void configureSelect(Qt::MouseButtons button, bool show);
	const QRect & getSelectRect() const;

public:
    explicit TrCanvas(QWidget *parent = nullptr);

	void setFont(QFont * font);

	virtual void zoomReset();
	virtual void zoomChange(double value, const QPoint pt, int limit);
	void setBackgroundColor(QMap<int, QColor> & map);

    virtual bool notifyCoor(const QPoint pt, int mode, Qt::MouseButton button);
    virtual bool notifyClick(const QPoint pt, int mode, Qt::MouseButton button);
	virtual bool notifyWheel(const QPoint pt, int a, int b);
    virtual bool notifyRectSelect(const QRect & r, Qt::MouseButton button);

	void notifyKey(const QString & s);

	virtual void userMessage(unsigned char type, int value, void * add);

	virtual void addMenuEntrys(QMenu * add_to, unsigned char type);

public slots:
	void slotKey(QKeyEvent* key);
};

#endif // TR_CANVAS_H

