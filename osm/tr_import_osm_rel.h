/******************************************************************
 *
 * @short	data import in OSM format
 *
 * project:	Trafalgar/OSM
 *
 * class:	TrImportOsmRel
 * superclass:	---
 * modul:	tr_import_osm_rel.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	02.2024
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


#ifndef TR_OSM_REL_H
#define TR_OSM_REL_H

#include <tr_geo_object.h>

#include <QtCore/qvector.h>

#include "osm_load.h"

#include "osm_types.h"
#include "tr_point.h"

#include <QtCore/qxmlstream.h>

// TODO: use own class/module?
struct Relation
{
	uint64_t m_flags;
	QVector<RelMember_t> m_members;

	Relation();
	friend QDebug operator<<(QDebug dbg, const Relation& member);

	int isMultiPolyRing();
	void resetPolyRing(QMap<uint64_t, Way_t> & waylist);
};


class TrImportOsmRel
{
private:
	QMap<QString, QString> m_tags;
	int64_t m_id;

	void closeRelation(QMap<uint64_t, Way_t> & waylist, Relation & rel);

	bool setMemberData(QMap<uint64_t, Way_t> & waylist, Relation & rel, uint64_t type);
	bool handleMultiPoly(QMap<uint64_t, Way_t> & waylist, Relation & rel);

public:
	TrImportOsmRel();
	virtual ~TrImportOsmRel();

	bool osmRelationRead(QXmlStreamReader & xml, QMap<uint64_t, Way_t> & waylist, Relation & rel);
			//QMap<uint64_t, Rel_t> & rellist);

	void readRelation(const QXmlStreamAttributes &attributes, Relation & rel);

	void readTag(const QXmlStreamAttributes &attributes);
	void readMember(const QXmlStreamAttributes &attributes, Relation & rel);

	static uint64_t getBuildingClass(const QString & value);
	static uint64_t getBarrierClass(const QString & value);
	static uint64_t getLanduseClass(const QString & value);
	static uint64_t getNaturalClass(const QString & value);
	static uint64_t getWaterWayClass(const QString & value);
	static uint64_t getRailWayClass(const QString & value);

	static uint64_t getHighwayPointClass(const QString & value);
	static uint64_t getRailwayPointClass(const QString & value);
	static uint64_t getAmenityClass(const QString & value, bool node);
	static uint64_t getShopClass(const QString & value);
};

#endif

