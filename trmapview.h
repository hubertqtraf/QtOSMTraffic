/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024 - 2025
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

#ifndef TRMAPVIEW_H
#define TRMAPVIEW_H

#include "tr_canvas.h"
#include "tr_document.h"
#include "trnodedock.h"
#include "trlinkdock.h"
//#include <QSvgGenerator>
#include <QDockWidget>
#include <QWidget>

class TrMapView : public TrCanvas
{
    Q_OBJECT
private:
    uint64_t m_pos_select;

    TrDocument m_doc;
    TrZoomMap m_zoom_ref;
    Qt::MouseButton m_move_pressed;
    TrGeoObject * m_selected;

    void paint(QPainter * p);
    TrPoint getWorldPoint(const QPoint & pt);
    TrGeoObject * selectObject(const TrPoint & pt, uint64_t & pos, uint64_t flag);

    QDockWidget * m_elementDock;
    TrNodeDock * m_dockNode;
    TrLinkDock * m_dockLink;

    TrMapList m_solarList;
    bool m_solar;

protected:
    void resizeEvent(QResizeEvent *);

public:
    TrMapView(QWidget *parent);

    TrDocument & getDocument();

    void initObjects(uint64_t ctrl);

    void setLoadedFlag(bool loaded);

    void setElementDock(QDockWidget * dock);

    void recalcExtRect();
    void resetZoom();
    void zoomChange(double value, const QPoint pt, int limit);
    void zoomChange(bool dir);

    bool notifyCoor(const QPoint pt, int mode, Qt::MouseButton button);
    bool notifyPress(const QPoint pt, Qt::MouseButton button);
    bool notifyMove(const QPoint pt, Qt::MouseButton button);
    bool notifyRelease(const QPoint pt, Qt::MouseButton button);
    bool notifyClick(const QPoint,  int mode, Qt::MouseButton button);
    bool notifyWheel(const QPoint, int a, int b);
    bool notifyRectSelect(const QRect & r, Qt::MouseButton button);

    void setSettingsData(QStringList modes, QStringList layers);

    void setSolarOption(bool solar);
    TrMapList & getOverlayList(int mode);

    //void paintSvg(QSvgGenerator &generator);

signals:
    void sendMessage(const QString, int);
};

#endif // TRMAPVIEW_H
