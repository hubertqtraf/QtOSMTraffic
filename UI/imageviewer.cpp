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
	//setCentralWidget(m_scrollArea);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	//mainLayout->addWidget(m_scrollArea);
	mainLayout->addWidget(m_imageLabel);
	resize(100,100);

	//createActions();

	//resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

	QImageReader reader(fname);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	//if (newImage.colorSpace().isValid())
	//    newImage.convertToColorSpace(QColorSpace::SRgb);

	m_imageLabel->setPixmap(QPixmap::fromImage(newImage));
	resize(newImage.width(), newImage.height());
}
