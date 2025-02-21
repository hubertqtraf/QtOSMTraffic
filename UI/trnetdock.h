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

#ifndef TRNETDOCK_H
#define TRNETDOCK_H

#include <QWidget>

namespace Ui {
class TrNetDock;
}

class TrNetDock : public QWidget
{
    Q_OBJECT

public:
    explicit TrNetDock(QWidget *parent = nullptr);
    ~TrNetDock();

    uint64_t getNetFlags();

private slots:
    void on_double_2_clicked(bool checked);

    void on_crossings_clicked(bool checked);

    void on_nodes_clicked(bool checked);

    void on_text_clicked(bool checked);

    void on_parking_clicked(bool checked);

    void on_rd_name_clicked(bool checked);

private:
    Ui::TrNetDock *ui;

    void setFlag(int checked, uint64_t flag);

    uint64_t m_flags;

signals:
    void selectModeChanged(uint64_t);
};

#endif // TRNETDOCK_H
