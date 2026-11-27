/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2026-2026
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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

class ImageViewer : public QDialog
{
	Q_OBJECT
public:
	explicit ImageViewer(QWidget *parent = nullptr, const QString &fname = "");

	void read(const QString &fname);
	void setImage(const QImage &newImage);

signals:

private:
	QImage m_image;
	QLabel *m_imageLabel;
	QScrollArea *m_scrollArea;
	double m_scaleFactor = 1;
};

#endif // IMAGEVIEWER_H
