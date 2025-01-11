/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024-2025
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


#ifndef TRLINKDOCK_H
#define TRLINKDOCK_H

#include <tr_map_link_road.h>

#include <QWidget>

namespace Ui {
class TrLinkDock;
}

class TrLinkDock : public QWidget
{
    Q_OBJECT

public:
    explicit TrLinkDock(QWidget *parent = nullptr);
    ~TrLinkDock();

    void setData(TrGeoObject *obj);
private:
    Ui::TrLinkDock *ui;
};

#endif // TRLINKDOCK_H
