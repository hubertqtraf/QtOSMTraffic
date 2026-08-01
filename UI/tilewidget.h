#ifndef TILEWIDGET_H
#define TILEWIDGET_H

#include "tr_document.h"

#include <QWidget>

class TileWidget : public QWidget
{
	Q_OBJECT
private:
	TrDocument *m_doc;
	TrZoomMap m_zoom_ref;
	int m_level;

public:
	explicit TileWidget(QWidget *parent = nullptr);

	int lon2TileX(double lon);
	int lat2TileY(double lat);
	double tileX2Lon(int x);
	double tileY2Lat(int y);

	void setDocument(TrDocument * doc);

	void recalcExtRect();
	void createPngImage(QImage &image);
	virtual void paint(QPainter * p);
signals:

};

#endif // TILEWIDGET_H
