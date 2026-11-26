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

#include "imageviewer.h"

#include <QHBoxLayout>
#include <QImageReader>


ImageViewer::ImageViewer(QWidget *parent, const QString &fname)
	: QDialog(parent)
	, m_imageLabel(new QLabel)
	, m_scrollArea(new QScrollArea)
{
	setWindowTitle("Show Image");

	m_imageLabel->setBackgroundRole(QPalette::Base);
	m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_imageLabel->setScaledContents(true);

	m_scrollArea->setBackgroundRole(QPalette::Dark);
	m_scrollArea->setWidget(m_imageLabel);
	m_scrollArea->setVisible(false);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_scrollArea);

	//createActions();

	//resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

	QImageReader reader(fname);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	//if (newImage.colorSpace().isValid())
	//    newImage.convertToColorSpace(QColorSpace::SRgb);
	setImage(newImage);

	m_imageLabel->setPixmap(QPixmap::fromImage(newImage));
	resize(400,600);
	m_imageLabel->resize(newImage.width(), newImage.height());
}


void ImageViewer::setImage(const QImage &newImage)
{
	m_image = newImage;
	//if (m_image.colorSpace().isValid())
	//    m_image.convertToColorSpace(QColorSpace::SRgb);
	m_imageLabel->setPixmap(QPixmap::fromImage(m_image));

	m_scaleFactor = 1.0;

	m_scrollArea->setVisible(true);
	/*printAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	updateActions();

	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();*/
}
