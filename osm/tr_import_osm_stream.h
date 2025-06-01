/******************************************************************
 *
 * @short	data import in OSM format
 *
 * project:	Trafalgar/OSM
 *
 * class:	TrImportOsmStream
 * superclass:	QXmlDefaultHandler
 * modul:	tr_import_osm_stream.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	01.2024
 *
 * @author	Schmid Hubert (C)2024-2025
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


#ifndef TR_OSM_STREAM_H
#define TR_OSM_STREAM_H

#include <tr_geo_object.h>
#include <tr_map_face.h>
#include <tr_map_parklane.h>

#include <QtCore/qvector.h>

#include "osm_load.h"

#include "osm_types.h"
#include "tr_point.h"

#include "tr_import_osm_rel.h"

class TrImportOsmStream : public TrGeoObject
{
private:
	uint8_t m_mode;
	QString m_filename;
	//<tag k="highway" v="motorway"/>
	QMap<QString, QString> m_tags;
	uint64_t m_id;
	TrPoint m_point;
	QMap<uint64_t, Point_t> m_nodelist;
	QMap<uint64_t, Way_t> m_waylist;
	QVector<Rel_t> m_rellist;
	QVector<uint64_t> m_way_reflist;

	void readTag(const QXmlStreamAttributes &attributes);
	void readNodePoint(const QXmlStreamAttributes &attributes);
	void readWay(const QXmlStreamAttributes &attributes);

	void closeNode(QMap<QString, name_set> & name_map,
			uint64_t & act_id, QMap<uint64_t, QString> & n_map);
	void closeWay(QMap<QString, name_set> & name_map, uint64_t & act_id);
	void closeOsm(World_t & world);

	void addRelation(const Relation & rel);

	uint64_t getClass(const QString & value);
	uint64_t getDir(const QString & value);
	uint64_t getLanes();
	bool parkingMode(const QString & part, uint64_t & code);
	uint64_t parkingOrientation(const QString & value, uint64_t & code);
	uint64_t parkingRes(const QString & value, uint64_t & code);
	uint64_t getParking();
	uint64_t getPlacement(const QString & value);

	bool setRel2Face(Rel_t & rel, QVector<TrMapFace *> & face_list);
	bool appendFacePoint(uint64_t id, TrMapFace & face);
	bool appendMultiWayPoint(Way_t & way, QVector<QVector<uint64_t>> & rings);

public:
	TrImportOsmStream(const QString & name);
	TrImportOsmStream();
	virtual ~TrImportOsmStream();

	bool osmRead(World_t & world);

	void setNodeIds(World_t & world);

	QMap<uint64_t, Point_t> & getNodeMap();
	QVector<Rel_t> & getRelationList();

	void createRelFaces(QVector<TrMapFace *> & face_list, uint8_t mode);

	virtual bool init(const TrZoomMap& zoom, uint64_t contr, TrGeoObject* obj);
	virtual bool setSurroundingRect();

	QString errorString() const;
};

#endif

