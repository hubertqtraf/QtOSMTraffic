/******************************************************************
 * project:	Trafalgar/OSM-Toolz
 *
 * class:	TrImportOsm
 * superclass:	---
 * modul:	tr_import_osm.cc
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * (C)		Schmid Hubert 2012-2025
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


#include "tr_map_node.h"
#include "tr_map_poi.h"
#include <tr_prof_class_def.h>

#ifdef OSM_C_FILTER
#include "osm_main.h"
#include "osm_func.c"
#endif
#include "osm_load_rel.h"

#include "tr_name_element.h"
#include "tr_import_osm_stream.h"

#include "tr_defs.h"
#include "tr_import_osm.h"

#include <QtCore/qfile.h>
//#include <QDebug>
#include <tr_map_face.h>
#include <tr_map_net.h>

#ifndef OSM_C_FILTER
int inline findNode(Point_t * nodes, int n_node, uint64_t idx)
{
	int pos1 = 0;
	int pos2 = n_node-1;
	int d;

	uint64_t id = idx;

	if((id > nodes[n_node-1].id) || (id < nodes[0].id))
		return -1;

	while(nodes[pos2].id != id)
	{
		if(nodes[pos1].id == id)
			return pos1;

		d = ((pos2-pos1) >> 1);

		if(d == 0)
			return -1;

		if(id < nodes[pos1+d].id)
			pos2 -= d;
		else
			pos1 += d;
	}
	return pos2;
}
#endif

TrImportOsm::TrImportOsm()
	: m_waySize(0)
	, m_ways(nullptr)
	, m_nodeSize(0)
	, m_nodes(nullptr)
	, m_poi_map(nullptr)
{
}

TrImportOsm::~TrImportOsm()
{
	// TODO: delete data...
}

uint16_t TrImportOsm::setTrainType(TrOsmLink * link, uint64_t ttype)
{
	if(ttype & FLAG_PLANNED)
		return 7;

	if(ttype & FLAG_DISUSED)
		return 6;

	if(ttype & FLAG_CONSTRUCTION)
		return 8;

	uint64_t type = (ttype & 0x00000000000000f0) >> 4;
	//TR_MSG << hex << type << " - " << ttype;
	if(type >= 8)
	{
		type -= 7;
		return static_cast <uint16_t>(type);
	}
	return 7;
}

uint16_t TrImportOsm::setWaterType(TrOsmLink * link, uint64_t ttype)
{
	uint16_t type = ttype & 0x0000000000000f;
	if(type == 0)
		return 1;
	if(type > 5)
	{
		return 5;
	}
	return type;
}

void TrImportOsm::appendPoi(void * world, const Point_t & point)
{
	World_t * osm2_world = static_cast<World_t *>(world);

	if(point.pt_type)
	{
		TrMapPoi * elem = new TrMapPoi;
		TrPoint pt;
		// TODO: check the factor '100.0'
		pt.x = (point.x)/100.0;
		pt.y = (point.y)/100.0;
		elem->setPoint(pt);
		elem->setPoiName(osm2_world->m_point_name_map[point.id]);
		elem->setPoiTypeFlags(point.pt_type);
		elem->setPoiNumData(point.pt_data);

		m_poi_map->appendObject(elem, point.id);
	}
	else
	{
		//TR_MSG << "no type info" << point.id << " " <<
		//	osm2_world->m_point_name_map[point.id].toLocal8Bit().constData();
	}
}


bool TrImportOsm::read(const QString & filename, TrMapList & name_list, uint8_t mode)
{
	World_t osm2_world;
	bool add_it = true;
	TrPoint pt;

	TrOsmLink * olink = nullptr;

	emit valueChanged(3);

	// POI - Point Of Interest
	m_poi_map = new TrMapList();
	// set layer name
	m_poi_map->setObjClass("poi");

	if(filename.endsWith(".OSM", Qt::CaseInsensitive))
	{
		TR_INF << "osm file: " << filename;

		TrImportOsmStream ios(filename);
		ios.osmRead(osm2_world);
		for (auto i = ios.getNodeMap().cbegin(), end = ios.getNodeMap().cend(); i != end; ++i)
		{
			appendPoi(&osm2_world, i.value());
		}
		ios.setNodeIds(osm2_world);
#ifdef OSM_C_FILTER
		m_nodeSize = osm2_world.info.node.count;
#else
		m_nodeSize = osm2_world.node_count;
#endif
		m_nodes = osm2_world.nodes;

		ios.createRelFaces(m_face_list);

		//return true;
	}
	else
	{
#ifdef OSM_C_FILTER
		QByteArray ba = filename.toLatin1();
		//osm2_world.m_name_list = &name_list;

		if(read_main(1, ba.data(), &osm2_world) == (-1))
		{
			TR_ERR << "file error, reading " << filename;
			return false;
		}
#endif
	}

	QMap<QString, name_set>::const_iterator i = osm2_world.m_name_map.constBegin();
	while (i != osm2_world.m_name_map.constEnd())
	{
		TrNameElement * elem = new TrNameElement;
		elem->setName(i.key());
		elem->setNameNumber(i.value().number);
		name_list.appendObject(elem, i.value().id);
		++i;
	}
#ifdef TESTX
	for(unsigned int i=0; i < osm2_world.info.point.count; i++)
	{
		// single point list
		appendPoi(&osm2_world, osm2_world.points[i]);
	}
#endif
#ifdef OSM_C_FILTER
	for(unsigned int i=0; i < osm2_world.info.node.count; i++)
#else
	for(unsigned int i=0; i < osm2_world.node_count; i++)
#endif
	{
		// way points with POI info
		appendPoi(&osm2_world, osm2_world.nodes[i]);
	}
#ifdef OSM_C_FILTER
	m_nodeSize = osm2_world.info.node.count;
#else
	m_nodeSize = osm2_world.node_count;
#endif
	m_nodes = osm2_world.nodes;
#ifdef OSM_C_FILTER
	for(unsigned int i=0; i < osm2_world.info.rel.count; i++)
	{
		if(count_outer(&(osm2_world.relations[i])))
		{
			createRelFace((osm2_world.relations[i]), osm2_world.ways, osm2_world.info.way.count);
		}
	}
#endif
	//printRawData(&osm2_world);

	QVector <RawNode> osm_nodes;
	RawNode act_node;
#ifdef OSM_C_FILTER
	m_nodeSize = osm2_world.info.node.count;
	m_waySize = osm2_world.info.way.count;
#else
	m_nodeSize = osm2_world.node_count;
	m_waySize = osm2_world.way_count;
#endif
	m_ways = osm2_world.ways;
	m_nodes = osm2_world.nodes;

	emit valueChanged(20);

	unsigned int level = m_waySize/8;
	unsigned int count = 1;

	for(unsigned int i=0; i < m_waySize; i++)
	{
		add_it = true;
		olink = new TrOsmLink();
		//link->setId(osm2_world.ways[i].id);	// TODO: setId unused?

		if(i == level * count)
		{
			emit valueChanged(20+(count * 8));
			count++;
		}

		for(int j=0; j < osm2_world.ways[i].n_nd_id; j++)
		{
			int id = findNode(osm2_world.nodes, m_nodeSize,
				osm2_world.ways[i].nd_id[j]);

			if(id < 0)
			{
				printf("findNode: cannot find %lu %i %i %i\n",
					osm2_world.ways[i].nd_id[j], id, i, j);
				add_it = false;
			}

			pt.x = (osm2_world.nodes[id].x/100.0);
			pt.y = (osm2_world.nodes[id].y/100.0);
	
			act_node.id = osm2_world.ways[i].nd_id[j];
			act_node.x = osm2_world.nodes[id].x;
			act_node.y = osm2_world.nodes[id].y;
			olink->addRawNode(osm2_world.ways[i].nd_id[j]);
			olink->appendPolyPoint(pt);
			osm_nodes.append(act_node);
		}

		if(add_it)
		{
			uint16_t rd_class = 0;

			// TODO: check parameter
			switch(osm2_world.ways[i].type & 0x00000000000f0000)
			{
			case TYPE_ROAD:
			case (TYPE_ROAD | TYPE_RAIL):      // TODO: add to railway too
				// TODO: should be ff: 16 > 256
				rd_class = osm2_world.ways[i].type & 0x000000000000000f;
				if(osm2_world.ways[i].type & FLAG_RAMP)
					rd_class |= TR_LINK_RAMP_FLAG;

				olink->setRdClass(rd_class);

				olink->setOneWay((osm2_world.ways[i].type & 0x0000000f00000000) >> 32);   // 0x0600) >> 9);

				// set both modes...
				olink->setOsmLanes(osm2_world.ways[i].lanes);
				olink->setLanes(osm2_world.ways[i].lanes);

				olink->setWidth(osm2_world.ways[i].width);
				olink->setOsmWidth(osm2_world.ways[i].width);

				olink->setNameId(osm2_world.ways[i].name_id);

				// parking lanes
				olink->setOsmParking(osm2_world.ways[i].parking);

				// TODO: set a flag for the SIDEWALK option?
				if(osm2_world.ways[i].type & FLAG_SIDEWALK)
					olink->setOsmSidewalk(1);

				road_raw_link_list.append(olink);

				addRawNodes(olink->getRawNodes(), road_poly_nodes, olink->getOneWay());
				addRawCoor(&osm_nodes, road_poly_nodes);
				break;

			case TYPE_RAIL:
				olink->setRdClass(setTrainType(olink, osm2_world.ways[i].type));
				// TODO: set the name of the train connection?
				olink->setNameId(static_cast<uint32_t>(osm2_world.ways[i].name_id));

				rail_raw_link_list.append(olink);
				if(addRawNodes(olink->getRawNodes(), rail_poly_nodes, olink->getOneWay()) == true)
					addRawCoor(&osm_nodes, rail_poly_nodes);
				break;

			case TYPE_STREAM:
				//TR_MSG << "stream";
				olink->setRdClass(setWaterType(olink, osm2_world.ways[i].type));
				// TODO: set the name of the river?
				olink->setNameId(static_cast<uint32_t>(osm2_world.ways[i].name_id));
				stream_raw_link_list.append(olink);

				if(addRawNodes(olink->getRawNodes(), stream_poly_nodes, olink->getOneWay()) == true)
					addRawCoor(&osm_nodes, stream_poly_nodes);
				break;

			default:
				delete olink;
				break;
			}
			osm_nodes.clear();
		}
		else
			delete olink;
	}
	emit valueChanged(100);
	return true;
}


bool TrImportOsm::createNet(TrMapNet * osm_net, QString name)
{
	if(osm_net == nullptr)
	{
		TR_WRN << "NULL Pointer at " << name;
		return false;
	}

	osm_net->setName(name);
	osm_net->init();

	// from 'tr_map_net'
	if(name == "road")
	{
		if(finalizeNet(road_raw_link_list, road_poly_nodes, "net", osm_net, true) == false)
		{
			TR_ERR << "read (road)";
			return false;
		}
	}
	if(name == "rail")
	{
		if(finalizeNet(rail_raw_link_list, rail_poly_nodes, "net", osm_net, false) == false)
		{
			TR_ERR << "read (rail)";
			return false;
		}
	}
	if(name == "stream")
	{
		if(finalizeNet(stream_raw_link_list, stream_poly_nodes, "net", osm_net, false) == false)
		{
			TR_ERR << "read (stream)";
			return false;
		}
	}
	return true;
}

bool TrImportOsm::appendFacePoint(uint64_t id, TrMapFace & face)
{
	TrPoint pt;

	int nd_id = findNode(m_nodes, static_cast<int>(m_nodeSize), id);
	if(nd_id < 0)
		return false;
	pt.x = (m_nodes[nd_id].x/100.0);
	pt.y = (m_nodes[nd_id].y/100.0);

	face.appendPolyPoint(pt);
	return true;
}

bool TrImportOsm::appendFacePoints(const Way_t & way, TrMapFace & face, bool dir)
{
	if(dir)
	{
		for(int i= (way.n_nd_id -1); i ; i--)
		{
			if(!appendFacePoint(way.nd_id[i], face))
				return false;
		}
	}
	else
	{
		for(int i=0; i < way.n_nd_id; i++)
		{
			if(!appendFacePoint(way.nd_id[i], face))
				return false;
		}
	}
	return true;
}

int TrImportOsm::checkDir(const Way_t & way1, const Way_t & way2)
{
	uint64_t nd1 = way1.nd_id[0];
	uint64_t nd2 = way1.nd_id[way1.n_nd_id -1];
	uint64_t nd3 = way2.nd_id[0];
	uint64_t nd4 = way2.nd_id[way2.n_nd_id -1];
	//TR_INF << nd1 << nd2 << nd3 << nd4;

	if((nd1 == nd3) && (nd2 != nd4))
	{
		return 1;
	}
	if((nd1 == nd4) && (nd2 != nd3))
	{
		return 2;
	}
	if((nd2 == nd3) && (nd1 != nd4))
	{
		return 3;
	}
	if((nd2 == nd4) && (nd1 != nd3))
	{
		return 4;
	}
	return 0;
}

// TODO: create a function for blocks of outer or inner
#ifdef OSM_C_FILTER
void TrImportOsm::createRelFace(Rel_t & relation, Way_t * ways, uint64_t n_way)
{
	int64_t idx1 = (-1);
	int64_t idx2 = (-1);
	bool first = true;
	TrMapFace * face = nullptr; //new TrMapFace();
	//face->appendPolygon(0);
	for(uint32_t i = 0; i< relation.r_count; i++)
	{
		//idx_1 = findWay(world->ways, world->info.way.count, world->rel_id_buf[1].id);
		if(relation.members[i].flags & REL_MEM_ROLE_OUT)
		{
			if(face == nullptr)
			{
				face = new TrMapFace();
				face->appendPolygon(0);
			}
			idx1 = findWay(ways, n_way, relation.members[i].id);
			if(idx1 == (-1))
				return;
			if(idx1 != (-1))
			{
				if(idx2 != (-1))
				{
					int code = checkDir(ways[idx2], ways[idx1]);
					if(first)
					{
						//TR_INF << "first" << ways[idx2].id << ways[idx1].id << code;
						if((code == 2) || (code == 1))
							appendFacePoints(ways[idx2], *face, true);
						if((code == 3) || (code == 4))
							appendFacePoints(ways[idx2], *face, false);
						first = false;
					}
					else
					{
						//TR_INF << "next" << ways[idx2].id << " # " << ways[idx1].id << code;
					}
					if((code == 2) || (code == 4))
						appendFacePoints(ways[idx1], *face, true);
					if((code == 1) || (code == 3))
						appendFacePoints(ways[idx1], *face, false);
				}
				idx2 = idx1;
				// TODO: way1 way2 -> dir
				//appendFacePoints(ways[idx1], *face, true);
			}
			//TR_INF << "member: " << relation.members[i].id << idx;
		}
		if(relation.members[i].flags & REL_MEM_ROLE_IN)
		{
			if(face != nullptr)
			{
				uint64_t type = (relation.flags & 0x000000f000000000) >> 24;
				face->setType((relation.flags & 0x00000000000000ff) | type);
				face->setDrawType(type & 0xf000);
				m_face_list.append(face);
				idx1 = (-1);
				idx2 = (-1);
				first = true;
				face = nullptr;
			}
		}
		//face->setFaceClass(relation.flags >> 16);
		//face->setType(relation.flags | 0x4000);
	}
	if(face != nullptr)
	{
		uint64_t type = (relation.flags & 0x000000f000000000) >> 24;
		face->setType((relation.flags & 0x00000000000000ff) | type);
		face->setDrawType(type & 0xf000);
		m_face_list.append(face);
	}
}
#endif

bool TrImportOsm::createFaceList(TrMapList * osm_list, QString name)
{
	uint64_t type = 0xffffffffffffffff;

	if(osm_list == nullptr)
	{
		TR_MSG << "error NULL pointer at " << name;
		return false;
	}

	TrMapFace * face = nullptr;

	if(name == "field")
		type = TYPE_NATURAL;    // 0x00000005

	if(name == "building")
		type = TYPE_BUILDING;	// 0x00000007

	if(name == "landuse")
		type = TYPE_LANDUSE;	// 0x00000006

	// TR_MSG << hex << world_->way_flags << " -> " << QString((const char*)sax->tag_start);

	for(unsigned int i=0; i < m_waySize; i++)
	{
		if((m_ways[i].type & 0x0000000000f00000) == type)
		{
			// TODO: use multipolygon
			face = new TrMapFace();
			face->appendPolygon(0x00);
			//if((osm2_world.ways[i].type & 0x0F) > 3)
			if(appendFacePoints(m_ways[i], *face, true))
			{
				uint64_t type = (m_ways[i].type & 0x000000f000000000) >> 24;
				face->setType((m_ways[i].type & 0x00000000000000ff) | type);
				face->setDrawType(type & 0xf000);
				osm_list->appendObject(face);
			}
			else
			{
				if(face != nullptr)
				{
					delete face;
					face = nullptr;
				}
			}
		}
	}
	for (int i = 0; i < m_face_list.size(); ++i)
	{
		uint64_t f_class = m_face_list[i]->getType();
		TR_INF << name << HEX << f_class << (f_class << 16)  << m_face_list[i]->getDrawType();
		if((name == "field") && ((f_class << 16) & TYPE_NATURAL))
			osm_list->appendObject(m_face_list[i]);
		if((name == "landuse") && ((f_class << 16) & TYPE_LANDUSE))
			osm_list->appendObject(m_face_list[i]);
		if((name == "building") && ((f_class << 16) & TYPE_BUILDING))
			osm_list->appendObject(m_face_list[i]);
		//m_face_list[i]->setFaceClass(1);
	}
	return true;
}



TrMapList * TrImportOsm::createPoiMap(QString name)
{
	return m_poi_map;
}


bool TrImportOsm::addNode(uint64_t id, QMap<uint64_t, PolyNode> & poly_nodes)
{
	//printf("TrImportOsm2::addNode %i\n", id);

	if(poly_nodes.find(id) == poly_nodes.end())
	{
		PolyNode act_node;

		act_node.x = 0;
		act_node.y = 0;
		act_node.n_in = act_node.n_out = act_node.flags = 0x00;

		poly_nodes[id] = act_node;

		return true;
	}
	return false;
}


bool TrImportOsm::addRawNodes(QVector <uint64_t> * nodes, QMap<uint64_t, PolyNode> & poly_nodes, uint8_t dir)
{
	if(nodes->size() <2)
		return false;

	for(int i = 0; i < nodes->size(); ++i)
	{
		if(this->addNode(nodes->at(i), poly_nodes) == false)
		{
			//printf("TrImportOsm2::addRawNodes error\n");
		}
		this->addNode(nodes->at(i), poly_nodes);
	}
	for(int i = 0; i < (nodes->size()); ++i)
	{
		poly_nodes[nodes->at(i)].n_out++;
		poly_nodes[nodes->at(i)].n_in++;
		if(!dir)
		{
			poly_nodes[nodes->at(i)].n_out++;
			poly_nodes[nodes->at(i)].n_in++;
		}
	}
	if(!dir)
	{
		poly_nodes[nodes->at(0)].n_in--;
		poly_nodes[nodes->at(0)].n_out--;
		poly_nodes[nodes->at(nodes->size()-1)].n_in--;
		poly_nodes[nodes->at(nodes->size()-1)].n_out--;
	}
	if(dir == 1)
		poly_nodes[nodes->at(0)].n_in--;

	if(dir == 2)
		poly_nodes[nodes->at(nodes->size()-1)].n_out--;

	return true;
}

void TrImportOsm::addRawCoor(QVector <RawNode> * nodes, QMap<uint64_t, PolyNode> & poly_nodes)
{
	for(int i = 0; i < nodes->size(); ++i)
	{
		if(poly_nodes.find(nodes->at(i).id) == poly_nodes.end())
		{
			printf("node id not found %lu\n", nodes->at(i).id);
		}
		else
		{
			poly_nodes[nodes->at(i).id].x = nodes->at(i).x;
			poly_nodes[nodes->at(i).id].y = nodes->at(i).y;
		}
	}
}

bool TrImportOsm::addNodeObj(TrMapList * node_map, int64_t id, double x, double y)
{
	if(node_map->getMapObject(id) == nullptr)
	{
		TrMapNode * opt = new TrMapNode;

		TrPoint pt;
		pt.x = x;
		pt.y = y;

		opt->setPoint(pt);
		opt->setGeoId(id);

		return node_map->appendObject(opt, id);
	}
	return false;
}

bool TrImportOsm::cutLink(TrOsmLink * olink, uint64_t * id,
	QMap<uint64_t, PolyNode> & poly_nodes, TrMapNet * osm_net, bool is_road)
{
	QVector <uint64_t> * raw = olink->getRawNodes();
	TrPoint pt;
	int in_out_limit = 2;
	TrGeoPolygon * poly = nullptr;
	int64_t i_idx = 0;
	uint64_t prim_id = 0;

	TrMapList * primive_map = osm_net->getNetList(TR_MASK_SELECT_POLY, false);
	TrMapList * node_map = osm_net->getNetList(TR_MASK_SELECT_POINT, false);

	if(raw->size() < 2)
		return false;

	TrMapLink * fwd = nullptr;
	TrMapLink * bwd = nullptr;

	addNodeObj(node_map, raw->at(0), poly_nodes[raw->at(0)].x/100.0,
		poly_nodes[raw->at(0)].y/100.0);
	addNodeObj(node_map, raw->at(raw->size()-1), poly_nodes[raw->at(raw->size()-1)].x/100.0,
		poly_nodes[raw->at(raw->size()-1)].y/100.0);

	bool set_sideway = false;
	uint8_t sideway_class = 0x0f;

	switch(olink->getOneWay())
	{
	case 0:
		fwd = olink->cloneLink(true, false, is_road);
		// both directions
		fwd->setOneWay(0x00);
		fwd->setNodeFrom(node_map, raw->at(0));

		bwd = olink->cloneLink(false, false, is_road);
		// reverse (both directions)
		bwd->setOneWay(TR_LINK_DIR_BWD);
		bwd->setNodeFrom(node_map, raw->at(raw->size()-1));
		break;

	case 1:
		fwd = olink->cloneLink(true, false, is_road);
		// single direction
		fwd->setOneWay(TR_LINK_DIR_ONEWAY);
		fwd->setNodeFrom(node_map, raw->at(0));
		if(olink->getOsmSidewalk() != 0)
		{
			TR_MSG << olink->getOsmSidewalk();
			bwd = olink->cloneLink(false, true, is_road);
			bwd->setNodeFrom(node_map, raw->at(raw->size()-1));
			bwd->setOneWay(TR_LINK_DIR_BWD);
			bwd->setRdClass(sideway_class);
			set_sideway = true;
		}
		in_out_limit = 1;
		break;

	case 2:
		bwd = olink->cloneLink(false, false, is_road);
		// reverse + single direction
		bwd->setOneWay(TR_LINK_DIR_BWD | TR_LINK_DIR_ONEWAY);
		bwd->setNodeFrom(node_map, raw->at(raw->size()-1));
		if(olink->getOsmSidewalk() != 0)
		{
			TR_MSG << olink->getOsmSidewalk();
			fwd = olink->cloneLink(true, true, is_road);
			fwd->setNodeFrom(node_map, raw->at(0));
			//bwd->setOneWay(TR_LINK_DIR_FWD);
			fwd->setRdClass(sideway_class);
			set_sideway = true;
		}
		in_out_limit = 1;
		break;

	default:
		break;
	}

	for (int i = 1; i < (raw->size()-1); ++i)
	{
		pt.x = (poly_nodes[raw->at(i)].x/100.0);
		pt.y = (poly_nodes[raw->at(i)].y/100.0);

		// double use as point and POI
		TrGeoObject * obj = m_poi_map->getObject(raw->at(i));
		TrMapPoi * poi = nullptr;
		if(obj != nullptr)
		{
			poi = dynamic_cast<TrMapPoi *>(obj);
		}

		if((poly_nodes[raw->at(i)].n_out > in_out_limit) ||
			(poly_nodes[raw->at(i)].n_out > in_out_limit))
		{
			addNodeObj(node_map, raw->at(i), pt.x, pt.y);

			if(poly != nullptr)
			{
				prim_id = primive_map->objCountMap() + 1;
				primive_map->appendObject(poly, prim_id);
				poly = nullptr;
			}

			if(fwd)
			{
				fwd->setNodeTo(node_map, raw->at(i));
				fwd->setGeoId(prim_id);
				osm_net->appendLink(fwd);
				//id_tmp = prim_id;

				fwd = olink->cloneLink(true, false, is_road);
				if(((olink->getOsmSidewalk() != 0) == (olink->getOneWay() != 0)) && set_sideway)
					bwd->setRdClass(sideway_class);
				fwd->setNodeFrom(node_map, raw->at(i));
			}
			if(bwd)
			{
				bwd->setNodeFrom(node_map, raw->at(i));
				bwd->setNodeTo(node_map, raw->at(i_idx));
				bwd->setGeoId(prim_id);
				osm_net->appendLink(bwd);

				bwd = olink->cloneLink(false, false, is_road);
				bwd->setOneWay(4);
				if(((olink->getOsmSidewalk() != 0) == (olink->getOneWay() != 0)) && set_sideway)
					bwd->setRdClass(sideway_class);
				bwd->setNodeFrom(node_map, raw->at(i_idx));
			}
			prim_id = 0;
			i_idx = i;
		}
		else
		{
			if(poly == nullptr)
			{
				poly = new TrGeoPolygon;
			}
			// TODO: append on local list
			bool set = true;
			if(poi != nullptr)
			{
			}
			if(set)
			{
				QVector<TrPoint> next_points;
				next_points.append(pt);
				poly->appendPoints(next_points);
			}
			if(poly->getSize() == 0)
				poly = nullptr;
		}
	}

	if(poly != nullptr)
	{
		// TODO: check the polygon...
		prim_id = primive_map->objCountMap() + 1;
		primive_map->appendObject(poly, prim_id);
		poly = nullptr;
	}

	if(fwd)
	{
		fwd->setGeoId(prim_id);
		osm_net->appendLink(fwd);
		fwd->setNodeTo(node_map, raw->at(raw->size()-1));
	}
	if(bwd)
	{
		bwd->setGeoId(prim_id);
		osm_net->appendLink(bwd);
		bwd->setNodeFrom(node_map, raw->at(raw->size()-1));
		bwd->setNodeTo(node_map, raw->at(i_idx));
	}
	return true;
}


bool TrImportOsm::finalizeNet(QVector<TrOsmLink *> raw_list, QMap<uint64_t,
	PolyNode> & poly_nodes, QString name, TrMapNet * osm_net, bool is_road)
{

	int i = 0;

	uint64_t id = 1;

	for (i = 0; i < raw_list.size(); ++i)
	{
		cutLink(raw_list[i], &id, poly_nodes, osm_net, is_road);
	}

	//TR_MSG << raw_list.size();

	TrMapList * primive_map = osm_net->getNetList(TR_MASK_SELECT_POLY, false);
	TrMapList * link_list = osm_net->getNetList(TR_MASK_SELECT_LINK, false);

	for (size_t i = 0; i < link_list->objCount(); ++i)
	{
		TrMapLink * link = dynamic_cast <TrMapLink*>(link_list->getVecObject(i));
		if(link != nullptr)
		{
			link->setPrimiveById(primive_map);
		}
	}
	return true;
}


