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

#ifndef TRMAPVIEW_H
#define TRMAPVIEW_H

#include "tr_canvas.h"
#include "tr_document.h"
//#include <QSvgGenerator>
#include <QWidget>

class TrMapView : public TrCanvas
{
    Q_OBJECT
private:
    TrDocument m_doc;
    TrZoomMap m_zoom_ref;
    Qt::MouseButton m_move_pressed;
    void paint(QPainter * p);

protected:
    void resizeEvent(QResizeEvent *);

public:
    TrMapView(QWidget *parent);

    TrDocument & getDocument();

    void initObjects(uint64_t ctrl);

    void setLoadedFlag(bool loaded);

    void recalcExtRect();
    void resetZoom();
    void zoomChange(double value, const QPoint pt, int limit);
    void zoomChange(bool dir);

    bool notifyCoor(const QPoint pt, int mode, Qt::MouseButton button);
    bool notifyClick(const QPoint,  int mode, Qt::MouseButton button);
    bool notifyWheel(const QPoint, int a, int b);
    bool notifyRectSelect(const QRect & r, Qt::MouseButton button);

    void setSettingsData(QStringList modes, QStringList layers);

    //void paintSvg(QSvgGenerator &generator);
signals:
    void sendMessage(const QString, int);
};

#endif // TRMAPVIEW_H
