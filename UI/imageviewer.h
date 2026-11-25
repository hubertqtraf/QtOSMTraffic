#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>

class ImageViewer : public QDialog //QWidget
{
	Q_OBJECT
public:
	explicit ImageViewer(QWidget *parent = nullptr, const QString &fname = "");

signals:

private:
	QImage m_image;
	QLabel *m_imageLabel;
	QScrollArea *m_scrollArea;
	double m_scaleFactor = 1;
};

#endif // IMAGEVIEWER_H
