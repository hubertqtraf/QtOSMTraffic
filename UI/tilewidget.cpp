#include "tilewidget.h"

#include <qapplication.h>
#include <qpainter.h>


TileWidget::TileWidget(QWidget *parent)
	: QWidget{parent}
	, m_level(1)
{
	//TR_INF << "! " << size();
	resize(1,1);
}

void TileWidget::setDocument(TrDocument *doc)
{
	m_doc = doc;
}

int TileWidget::lon2TileX(double lon)
{
	return static_cast<int32_t>(std::floor((lon + 180.0) / 360.0 * (1 << m_level)));
}

int TileWidget::lat2TileY(double lat)
{
	// Convert latitude to radians
	double lat_rad = lat * M_PI / 180.0;
	return static_cast<int32_t>(std::floor((1.0 - std::log(std::tan(lat_rad) + (1.0 /
		std::cos(lat_rad))) / M_PI) / 2.0 * (1 << m_level)));
}

double TileWidget::tileX2Lon(int x)
{
	return static_cast<double>(x) / (1 << m_level) * 360.0 - 180.0;
}

double TileWidget::tileY2Lat(int y)
{
	double lat_rad = std::atan(std::sinh(M_PI * (1.0 - 2.0 * static_cast<double>(y) / (1 << m_level))));
	return lat_rad * 180.0 / M_PI;
}

void TileWidget::recalcExtRect()
{
	if(m_doc == nullptr)
		return;
	TR_MSG << QString::number(m_doc->getSurroundRectVal(0),'f', 2) <<
		QString::number(m_doc->getSurroundRectVal(1),'f', 2) <<
		QString::number(m_doc->getSurroundRectVal(2),'f', 2) <<
		QString::number(m_doc->getSurroundRectVal(3),'f', 2); // << "edit_objects" << m_layerMap.size();

	m_doc->setSurroundingRect();

	m_level = 19;
	int x = lon2TileX(11.111);
	int y = lat2TileY(22.222);
	double lon1 = tileX2Lon(x);
	double lat1 = tileY2Lat(y);
	x+=1;
	y-=1;
	double lon2 = tileX2Lon(x);
	double lat2 = tileY2Lat(y);
	m_zoom_ref.setVisibleWorld(lon1 * 100000, lat1 * 100000, lon2 * 100000, lat2 * 100000);
	m_zoom_ref.zoom2Rect();
	update();
	//TR_MSG << m_doc->getSurroundRectVal(0) << m_doc->getSurroundRectVal(1) <<
	//	m_doc->getSurroundRectVal(2) << m_doc->getSurroundRectVal(3);
}


void TileWidget::paint(QPainter *p)
{
	if(m_doc != nullptr)
	{
		if(m_doc->m_is_loaded)
			m_doc->draw(m_zoom_ref, p, 0);
	}
	p->drawRect(0,0, 257, 257);
}

void TileWidget::createPngImage(QImage & image)
{
	QPainter painter;

	painter.begin(&image);
	paint(&painter);

	render(&painter);
	painter.end();
}
