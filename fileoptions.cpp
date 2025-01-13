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

#include "fileoptions.h"
#include "ui_fileoptions.h"

#include <tr_defs.h>
#include <tr_map_link_road.h>

#include <QFileDialog>

FileOptions::FileOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileOptions)
{
    ui->setupUi(this);
    ui->laneSpinBox->setValue(TrMapLinkRoad::ms_lane_width_p);
}

FileOptions::~FileOptions()
{
    delete ui;
}

QString FileOptions::getOsmDir()
{
    return ui->osmDir->text();
}

bool FileOptions::getShiftOption()
{
    return (ui->shiftCheck->checkState() == Qt::Checked);
}

int FileOptions::laneWith()
{
    return ui->laneSpinBox->value();
}

void FileOptions::setLaneWith(int width)
{
     ui->laneSpinBox->setValue(TrMapLinkRoad::ms_lane_width_p);
}

QString FileOptions::getProfileFileName()
{
    return ui->profileDir->text();
}

void FileOptions::on_setOsmDir_clicked()
{
    QFileDialog dialog;
#if QT_VERSION < 0x060000
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
#else
    dialog.setFileMode(QFileDialog::Directory);
    //dialog.setOption(QFileDialog::ShowDirsOnly, true);
#endif
    QString oldPath = ui->osmDir->text();
    if(!oldPath.size())
        oldPath = ".";
    TR_INF << oldPath;
    dialog.setDirectory(oldPath);
    dialog.exec();
    ui->osmDir->clear();
    ui->osmDir->insert(dialog.directory().path());
}

void FileOptions::on_setProfileDir_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Profile File"),
                                         ui->profileDir->text(), tr("Profile File (*.xml)"));
    ui->profileDir->clear();
    ui->profileDir->insert(fileName);
}

void FileOptions::manageSettings(QSettings & settings, bool mode)
{
    settings.beginGroup("Files");
    if(mode)        // read
    {
        ui->osmDir->setText(settings.value("OSM").toString());
        ui->profileDir->setText(settings.value("Profile").toString());
        if(settings.value("Shift").toInt())
        {
            ui->shiftCheck->setCheckState(Qt::Checked);
        }
        else
        {
            ui->shiftCheck->setCheckState(Qt::Unchecked);
        }
    }
    else            // write
    {
        //TR_INF << "write" << this->getWorldLayer() << this->getSelect();
        settings.setValue("OSM", ui->osmDir->text());
        settings.setValue("Profile", ui->profileDir->text());
        if(ui->shiftCheck->checkState() == Qt::Checked)
        {
            settings.setValue("Shift", 2);
        }
        else
        {
            settings.setValue("Shift", 0);
        }
    }
    settings.endGroup();
}

void FileOptions::on_buttonBox_accepted()
{
    TrMapLinkRoad::ms_lane_width_p = ui->laneSpinBox->value();
    TrMapLinkRoad::ms_lane_width_n = 0-ui->laneSpinBox->value();
    emit updateSettings();
}

void FileOptions::on_buttonBox_rejected()
{
    ui->laneSpinBox->setValue(TrMapLinkRoad::ms_lane_width_p);
}
