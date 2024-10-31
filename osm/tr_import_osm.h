/******************************************************************
 * project:	Trafalgar/OSM-Toolz
 *
 * class:	TrImportOsm
 * superclass:  ---
 * modul:	tr_import_osm.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * (C)		Schmid Hubert 2012-2022
 *
 * history:
 *
 ******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software foundation; either version 2, or (at your
   option) any later version.

   The GNU trafalgar package is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with the trafalgar package; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
   Boston, MA 02110-1301, USA. */


#ifndef TR_IMPORT_OSM_H 
#define TR_IMPORT_OSM_H 

#include <QMap>
#include <QObject>

#include <tr_map_list.h>
#ifdef TESTX
#include "Itr_import_layer.h"
#endif

#include <tr_map_net.h>

#include "tr_osm_link.h"
#include "tr_map_face.h"

#include "osm_types.h"

typedef struct
{
	uint64_t id;
	int32_t x;
	int32_t y;
}RawNode;

typedef struct
{
	int32_t x;
	int32_t y;
	uint8_t n_in;
	uint8_t n_out;
	uint8_t flags;
}PolyNode;


class TrImportOsm : public QObject
{
	Q_OBJECT
private:
	QVector<TrOsmLink *> road_raw_link_list;
	QMap<uint64_t, PolyNode> road_poly_nodes;

	QVector<TrOsmLink *> rail_raw_link_list;
	QMap<uint64_t, PolyNode> rail_poly_nodes;

	QVector<TrOsmLink *> stream_raw_link_list;
	QMap<uint64_t, PolyNode> stream_poly_nodes;

    size_t m_waySize;	//osm2_world.info.way.count
	Way_t * m_ways;

    size_t m_nodeSize;
	// Node + POI
	Point_t * m_nodes;

	TrMapList * m_poi_map;
	QVector<TrMapFace *> face_list;

	void appendPoi(void * world, const Point_t & point);
    uint16_t setTrainType(TrOsmLink * link, uint64_t ttype);
	uint16_t setWaterType(TrOsmLink * link, uint64_t ttype);

protected:

public:
	TrImportOsm();

	virtual ~TrImportOsm();

    bool read(const QString & filename, TrMapList & name_list);
	//int64_t osmWaySize();

	//void appendLinkOsm(TrOsmLink * link, QVector<TrOsmLink *> * raw_list);
	bool addRawNodes(QVector <uint64_t> * nodes, QMap<uint64_t, PolyNode> & poly_nodes, uint8_t dir);
	void addRawCoor(QVector <RawNode> * nodes, QMap<uint64_t, PolyNode> & poly_nodes);
	bool addNode(uint64_t id, QMap<uint64_t, PolyNode> & poly_nodes);
	bool addNodeObj(TrMapList * node_map, int64_t id, double x, double y);
	bool cutLink(TrOsmLink * link, uint64_t * id, QMap<uint64_t, 
		PolyNode> & poly_nodes, TrMapNet * osm_net, bool is_road);

	bool finalizeNet(QVector<TrOsmLink *> raw_list, QMap<uint64_t, 
		PolyNode> & poly_nodes, QString name, TrMapNet * osm_net, bool is_road);

	bool createNet(TrMapNet * osm_net, QString name);

	int checkDir(const Way_t & way1, const Way_t & way2);
	bool appendFacePoint(uint64_t id, TrMapFace & face);
	bool appendFacePoints(const Way_t & way, TrMapFace & face, bool dir);

#ifdef OSM_C_FILTER
	void createRelFace(Rel_t & relation, Way_t * ways, uint64_t n_way);
#endif

	bool createFaceList(TrMapList * osm_list, QString name);

	TrMapList * createPoiMap(QString name);

signals:
	void valueChanged(int value);

};

#endif // TR_IMPORT_OSM_H

