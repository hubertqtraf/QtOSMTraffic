/******************************************************************
 *
 * @short	basic class for Trafalgar drawing area
 *
 * project:	Trafalgar/Gui
 *
 * modul:	tr_canvas.cc		drawing area
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2004/2022
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


#include <math.h>

#include <stdio.h>

#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>
//#include <QtWidgets/qscrollbar.h>

#include <QtWidgets/qapplication.h>

#include "tr_canvas.h"

#include "tr_geo_object.h"


KeyEnterReceiver::KeyEnterReceiver()
{
}


bool KeyEnterReceiver::eventFilter(QObject* obj, QEvent* event)
{
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		if((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return))
		{
			//Enter or return was pressed
		}
		else
		{
			emit getKey(key);
			return QObject::eventFilter(obj, event);
		}
		return true;
	}
	else
	{
		return QObject::eventFilter(obj, event);
	}
}

TrCanvas::TrCanvas(QWidget * parent)
	: QWidget(parent)
	, flags(0)
	, m_double_click(0,0)
	, m_font(nullptr)
	, m_count_click(0)
{
	QPalette palette(QApplication::palette());
	palette.setColor(backgroundRole(),QColor(0,155,0));
	setPalette(palette);

	m_select_box.setParent(this);
	m_background.setRgb(55, 150, 50);
}


void TrCanvas::setBackgroundColor(QMap<int, QColor> & c_map)
{
	// '0x1000' is predefined
	if(c_map.contains(0x1000))
	{
		m_background = c_map[0x1000];
		QPalette palette(QApplication::palette());
		palette.setColor(backgroundRole(), m_background);
		setPalette(palette);
		//setStyleSheet("background-color:black;");
	}
}

void TrCanvas::setFont(QFont * font)
{
	m_font = font;
}

void TrCanvas::zoomReset()
{
}

bool TrCanvas::notifyCoor(const QPoint pt, int mode, Qt::MouseButton button)
{
	// default
	return false;
}

bool TrCanvas::notifyPress(const QPoint pt, Qt::MouseButton button)
{
	// default
	return false;
}

bool TrCanvas::notifyMove(const QPoint pt, Qt::MouseButton button)
{
	// default
	return false;
}

bool TrCanvas::notifyRelease(const QPoint pt, Qt::MouseButton button)
{
	// default
	return false;
}

bool TrCanvas::notifyClick(const QPoint pt, int mode, Qt::MouseButton button)
{
	// default
	return false;
}

// TODO rework? remove?
bool TrCanvas::notifyWheel(const QPoint pt, int a, int b)
{
	// default, notifiy on base class
	return false;
}

bool TrCanvas::notifyRectSelect(const QRect & r, Qt::MouseButton button)
{
	// default
	return false;
}

void TrCanvas::notifyKey(const QString & text)
{
	TR_MSG << text;
}

void TrCanvas::userMessage(unsigned char type, int value, void * add)
{
	// default
}

void TrCanvas::addMenuEntrys(QMenu * add_to, unsigned char type) 
{
	// default
}


void TrCanvas::mousePressEvent(QMouseEvent * m)
{
	TR_MSG << "mousePressEvent";

	//area_moved = false;
	switch(m->button())
	{
	case Qt::LeftButton:
		m_select_box.setFirst(m->pos(), false);
		m_select_box.setSecond(m->pos(), false);
		break;

	case Qt::RightButton:
		m_select_box.setFirst(m->pos(), true);
		m_select_box.setSecond(m->pos(), true);
		break;

	default:
		break;
	}
	notifyRectSelect(m_select_box.getRect(), m->button());

	// TODO: dialog to select bottons - map the bottons

	if(notifyPress(m->pos(), m->button()))
	{
		return;
	}
	update();
}

void TrCanvas::mouseMoveEvent(QMouseEvent * m)
{
	//TR_MSG << "mouseMoveEvent";

	if(m->button() == Qt::NoButton)
	{     
		m_select_box.setSecond(m->pos(), false);
	}

	notifyMove(m->pos(), m->button());

	update();
}

void TrCanvas::mouseReleaseEvent(QMouseEvent * m)
{
	switch(m->button())
	{
	case Qt::LeftButton:
		m_select_box.setSecond(m->pos(), false);
		break;

	case Qt::RightButton:
		m_select_box.setSecond(m->pos(), true);
		break;

	default:
		break;
	}

	if(notifyRelease(m->pos(), m->button()))
	{
		// no need for rect selection -> return
		m_select_box.setHide();
		update();
		return;
	}

	// or QWidget::mouseDoubleClickEvent()?
	if(m_select_box.getManhattanLength() < MIN_SELECT)
	{
		QPoint next = m_select_box.getNext();
		if(m_count_click)
		{
			QPoint point = next - m_double_click;

			//TR_MSG << point.manhattanLength() << " - " << m_count_click;

			if(point.manhattanLength() < MIN_SELECT)
			{
				m_count_click++;
			}
			else
			{
				m_count_click = 0;
			}
		}
		m_double_click = next;
		m_count_click++;

		notifyClick(next, m_count_click, m->button());
	}
	else
	{
		m_count_click = 0;
	}

	notifyRectSelect(m_select_box.getRect(), m->button());

	m_select_box.setHide();

	update();
}

void TrCanvas::wheelEvent(QWheelEvent *event)
{
	if(event->angleDelta().y() != 0)
	{
		// Qt v5.15: 'int QWheelEvent::delta() const’ is deprecated: Use angleDelta()
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		QPoint pt = event->position().toPoint();
#else
		QPoint pt = event->pos();
#endif
		if(event->angleDelta().y() > 0)
			zoomChange(0.8, pt, 0);
		else
			zoomChange(1.2, pt, 0);
	}
	event->accept();
}

void TrCanvas::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	// TODO: workround, background color not working at included Qt version
	p.setPen(m_background);
	p.setBrush(m_background);
	p.drawRect(0, 0, width(), height());

	// TODO: use flag -> move derv class
	//p.setRenderHint(QPainter::Antialiasing);
	if(m_font != nullptr)
		p.setFont(*m_font);
	paint(&p);
	m_select_box.draw(&p);
}

void TrCanvas::paint(QPainter * p)
{
	// default of virtual func
}

void TrCanvas::zoomChange(double value, const QPoint pt, int limit)
{
	TR_MSG;
}

void TrCanvas::configureSelect(Qt::MouseButtons button, bool show)
{
}

const QRect & TrCanvas::getSelectRect() const
{
	return m_select_box.getRect();
}

void TrCanvas::slotKey(QKeyEvent* key)
{
	if(key != nullptr)
	{
		TR_MSG << key << key->text();
		notifyKey(key->text());
	}
}

