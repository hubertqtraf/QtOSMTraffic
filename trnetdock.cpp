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

#include "trnetdock.h"
#include "ui_trnetdock.h"

#include <tr_defs.h>

TrNetDock::TrNetDock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TrNetDock)
    , m_flags(0)
{
    ui->setupUi(this);
}

TrNetDock::~TrNetDock()
{
    delete ui;
}

void TrNetDock::setFlag(int checked, uint64_t flag)
{
    if(checked)
    {
        m_flags |= flag;
    }
    else
    {
        m_flags &= ~flag;
    }
    emit selectModeChanged(m_flags);
}

void TrNetDock::on_double_2_clicked(bool checked)
{
    setFlag(checked, TR_MASK_MORE_LINES);
}

void TrNetDock::on_crossings_clicked(bool checked)
{
    setFlag(checked, TR_MASK_SET_RAMPS);
}

void TrNetDock::on_nodes_clicked(bool checked)
{
    setFlag(checked, TR_MASK_SHOW_POINTS);
}

void TrNetDock::on_text_clicked(bool checked)
{
    setFlag(checked, TR_MASK_POINTS_NUM);
}

void TrNetDock::on_parking_clicked(bool checked)
{
    setFlag(checked, TR_MASK_SHOW_PARKING);
}

void TrNetDock::on_rd_name_clicked(bool checked)
{
    setFlag(checked, TR_MASK_SHOW_ROADNAME);
}

uint64_t TrNetDock::getNetFlags()
{
    return m_flags;
}
