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

#ifndef FILEOPTIONS_H
#define FILEOPTIONS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class FileOptions;
}

class FileOptions : public QDialog
{
	Q_OBJECT

public:
	explicit FileOptions(QWidget *parent = nullptr);
	~FileOptions();

	QString getOsmDir();
	QString getProfileFileName();
	bool getShiftOption();
	bool getRelationOption();
	int laneWith();
	void setLaneWith(int width);


	void manageSettings(QSettings &settings, bool mode);

signals:
	void updateSettings();

private slots:
	void on_setOsmDir_clicked();

	void on_setProfileDir_clicked();

	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

private:
	Ui::FileOptions *ui;
};

#endif // FILEOPTIONS_H
