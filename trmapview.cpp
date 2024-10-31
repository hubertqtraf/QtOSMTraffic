/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024
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

#include "trmapview.h"

#include <qpainter.h>


TrMapView::TrMapView(QWidget *parent)
    : TrCanvas(parent)
    , m_move_pressed(Qt::NoButton)  // TODO: check the default, was '0'
{
}

TrDocument &TrMapView::getDocument()
{
    return m_doc;
}

void TrMapView::setSettingsData(QStringList modes, QStringList layers)
{
    // TODO: from profile...
    //const QStringList layers = {"road", "poi"};
    //TR_INF << layers << modes;
    for (int i = 0; i < modes.size(); ++i)
    {
        //TR_INF << modes.at(i) << layers;
        m_doc.addOrderByType(modes.at(i), layers);
    }
}

void TrMapView::initObjects(uint64_t ctrl)
{
    m_doc.init(m_zoom_ref, ctrl);
}

void TrMapView::setLoadedFlag(bool loaded)
{
    m_doc.m_is_loaded = true;
}

void TrMapView::paint(QPainter *p)
{
    if(m_doc.m_is_loaded)
    {
        p->setRenderHint(QPainter::Antialiasing);
        m_doc.draw(m_zoom_ref, p, 0);
    }
    else
    {
        p->setPen(Qt::blue);
        p->setFont(QFont("Arial", 30));
        p->drawText(rect(), Qt::AlignCenter, "Load a OSM Document");
    }
}

/*void TrMapView::paintSvg(QSvgGenerator & generator)
{
    QPainter painter;
    painter.begin(&generator);
    m_doc.draw(m_zoom_ref, &painter, 0);
    painter.end();
}*/

void TrMapView::recalcExtRect()
{
    TR_MSG << QString::number(m_doc.getSurroundRectVal(0),'f', 2) <<
              QString::number(m_doc.getSurroundRectVal(1),'f', 2) <<
              QString::number(m_doc.getSurroundRectVal(2),'f', 2) <<
              QString::number(m_doc.getSurroundRectVal(3),'f', 2); // << "edit_objects" << m_layerMap.size();

    TR_MSG << "m_base.setSurroundingRect";
    m_doc.setSurroundingRect();

    m_zoom_ref.setVisibleWorld(m_doc.getSurroundRectVal(0), m_doc.getSurroundRectVal(1),
                               m_doc.getSurroundRectVal(2), m_doc.getSurroundRectVal(3));
    m_zoom_ref.zoom2Rect();
    update();
    TR_MSG << m_doc.getSurroundRectVal(0) << m_doc.getSurroundRectVal(1) <<
              m_doc.getSurroundRectVal(2) << m_doc.getSurroundRectVal(3);
}

void TrMapView::resetZoom()
{
    m_zoom_ref.setVisibleWorld(m_doc.getSurroundRectVal(0),
                               m_doc.getSurroundRectVal(1),
                               m_doc.getSurroundRectVal(2),
                               m_doc.getSurroundRectVal(3));
    m_zoom_ref.zoom2Rect();
    update();
}

void TrMapView::zoomChange(bool dir)
{
    QPoint pt;
    pt.setX(static_cast <int>((width()/2.0)));
    pt.setY(static_cast <int>((height()/2.0)));
    if(dir)
        zoomChange(0.8, pt, 0);
    else
        zoomChange(1.2, pt, 0);
}

void TrMapView::zoomChange(double value, const QPoint pt, int limit)
{
    emit sendMessage("coor", 0);
    m_zoom_ref.setScreenDimension(width(), height());
    m_zoom_ref.moveToPoint(pt.x(), pt.y(), value);

    update();
}

void TrMapView::resizeEvent(QResizeEvent *)
{
    m_zoom_ref.setScreenDimension(width(), height());
    m_zoom_ref.zoom2Rect();
}


bool TrMapView::notifyCoor(const QPoint pt, int mode, Qt::MouseButton button)
{
    if(!m_doc.m_is_loaded)
        return false;

    TrPoint dpt;
    TrPoint dpt1;
    dpt.x = TR_POINT_V(pt.x());
    dpt.y = TR_POINT_V(pt.y());
    m_zoom_ref.getPoint(&dpt.x, &dpt.y);

    switch(mode)
    {
    case MOUSE_MODE_PRESS:
        m_move_pressed = button;
        emit sendMessage("coor: lon " + TR_COOR_VAL(dpt.x) + "; lat " + TR_COOR_VAL(dpt.y), 0);
        break;

    case MOUSE_MODE_MOVE:
        dpt1 = dpt;
        m_zoom_ref.getPoint(&dpt1.x, &dpt1.y);
        if(m_move_pressed == 0)
                emit sendMessage("coor: lon " + TR_COOR_VAL(dpt1.x) + "; lat " + TR_COOR_VAL(dpt1.y), 0);

        if(m_select_box.isRubber())
        {
                TrPoint spt;

                QPoint qpt = m_select_box.getStart();
                spt.x = qpt.x();
                spt.y = qpt.y();
                m_zoom_ref.getPoint(&spt.x, &spt.y);

                m_zoom_ref.getPoint(&dpt.x, &dpt.y);
                emit sendMessage("move coor: " + TR_COOR(spt) + " - " + TR_COOR(dpt), 0);
        }
        else
        {
                if(m_move_pressed)
                {
                        QPoint diff = m_select_box.diff();
                        //TR_MSG << diff.x() << " | " << diff.y();
                        m_zoom_ref.setMove(diff.x(), diff.y());
                }
        }
        break;

    case MOUSE_MODE_RELEASE:
        m_move_pressed = Qt::NoButton;  // TODO: check the default, was '0'
        if(m_select_box.isRubber())
        {
                m_select_box.setZoomRect(m_zoom_ref);
                m_select_box.setHide();
        }
        else
        {
                m_zoom_ref.setMove(0,0);
                m_zoom_ref.moveToPoint(static_cast <int>((width()/2.0)) - (pt.x() - m_select_box.getStart().x()),
                        static_cast <int>((height()/2.0)) + (pt.y() - m_select_box.getStart().y()));
                m_zoom_ref.zoom2Rect();
        }
        break;

    default:
        break;
    }
    return true;
}

bool TrMapView::notifyClick(const QPoint, int mode, Qt::MouseButton button)
{
    return false;
}

bool TrMapView::notifyWheel(const QPoint, int a, int b)
{
    return false;
}

bool TrMapView::notifyRectSelect(const QRect &r, Qt::MouseButton button)
{
    return false;
}
