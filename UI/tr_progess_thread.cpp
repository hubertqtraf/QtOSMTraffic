/******************************************************************
 * project:    	OSM Traffic
 *
 * class:       ---
 * superclass:  ---
 * modul:       tr_progress_thread.h
 *
 * system:      UNIX/LINUX
 * compiler:    gcc
 *
 * beginning:   03.2017
 *
 * (C)          Schmid Hubert 2017-2025
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


#include "tr_progess_thread.h"

//#include <tr_cmd_open.h>

#include <tr_import_osm.h>
// TODO: misssing lib
//#include <tr_import_atkis.h>
//#include <Itr_import_layer.h>

#include <tr_map_net_road.h>

#include <QtCore/qfile.h>

TrProgress::TrProgress(QObject *parent)
	: QThread(parent)
	, m_world(nullptr)
	, m_doc(nullptr)
	, m_type(TR_TREAD_IMP)
	, m_level(0)
	, m_bar(nullptr)
{
}

void TrProgress::createObjects(const QStringList & list, TrImportOsm & filter, const QString & mode)
{
    for(int i = 0; i < list.size(); i++)
    {
        if(mode == "roadnet")
        {
            TrMapNet * road_net = new TrMapNetRoad();
            filter.createNet(road_net, list[i]);
            m_doc->addMapLayerObjectByName(list[i], road_net);
        }
        if(mode == "net")
        {
            TrMapNet * net = new TrMapNet();
            filter.createNet(net, list[i]);
            m_doc->addMapLayerObjectByName(list[i], net);
        }
        if(mode == "face")
        {
            TrMapList * tr_list = new TrMapList();
            if(filter.createFaceList(tr_list, list[i]))
                m_doc->addMapLayerObjectByName(list[i], tr_list);
        }
    }
}


void TrProgress::run()
{
	//usleep(100000);
	TrImportOsm osm_filter;
	if(m_bar != nullptr)
	{
		m_bar->setRange(0,100);
		connect(&osm_filter, SIGNAL(valueChanged(int)), m_bar, SLOT(setValue(int)));
	}
	TR_INF << "start loading";

	//world->getFileName(), world->getNameList()

	if(osm_filter.read(m_doc->getFileName(), m_doc->getNameList(), 0) == false)
	{
		TR_INF << "error";
	}
	TR_INF << "OK";

	createObjects(m_list["roadnet"], osm_filter, "roadnet");
	createObjects(m_list["net"], osm_filter, "net");
	createObjects(m_list["face"], osm_filter, "face");

	//m_map_view->getDocument().setFileName(filename);

	TrMapList * poi_map = osm_filter.createPoiMap("poi");
	if(poi_map != nullptr)
		m_doc->addMapLayerObjectByName("poi", poi_map);
	//m_map_view->setSettingsData(m_profile_dlg->getElemStringList("modes"),
	//                            m_profile_dlg->getElemStringList("layer"));

	m_doc->m_is_loaded = true;

	/*if(m_type == TR_TREAD_IMP_O)
	{
		m_world = importDoc(m_doc);
	}
	if(m_type == TR_TREAD_IMP_A)
	{
		m_world = importDoc(m_doc);
        *//*TrImportAtkis * imAtkis = new TrImportAtkis(filename);
		if(imAtkis->read(filename, m_world->getNameList()) == false)
		{
			TR_WRN << "load error!";
        }*//*
	}
	if(m_type == TR_TREAD_IMP)
	{
		if(readDocument(m_doc) == false)
			m_world = nullptr;
		m_world = m_doc;
	*/
	TR_INF << "end loading";

	emit resultReady(&m_world);
}

TrGeoObject * TrProgress::importDoc(TrDocument * world)
{
#ifdef ATKIS
	TrImportAtkis * imAtkis = nullptr;
#endif
	TrImportOsm * imOsm = nullptr;
#ifdef ATKIS
	if(m_type == TR_TREAD_IMP_A)
		imAtkis = new TrImportAtkis(world->getFileName());
#endif
	if(m_type == TR_TREAD_IMP_O)
		imOsm = new TrImportOsm();

	if(world == nullptr)
		return nullptr;

	TR_MSG << world->getFileName();

	if(m_bar != nullptr)
	{
		m_bar->setRange(0,100);
                connect(imOsm, SIGNAL(valueChanged(int)), m_bar, SLOT(setValue(int)));
	}
	world->resetNameList();
#ifdef ATKIS
	if((m_type == TR_TREAD_IMP_A) && (imAtkis != nullptr))
	{
		TR_INF << "start with file: " << world->getFileName();
		if(imAtkis->read(world->getFileName(), world->getNameList()) == false)
		{
			TR_WRN << world->getFileName() << "failed";
			return nullptr;
		}
	}
#endif
	if((m_type == TR_TREAD_IMP_O) && (imOsm != nullptr))
	{
		if(imOsm->read(world->getFileName(), world->getNameList(), 0) == false)
		{
			TR_WRN << world->getFileName() << "failed";
			return nullptr;
		}
	}

	// TODO: get the list from profile
	//QStringList layer_list = m_profile_dlg->getElemStringList("layer", "net");
	// TODO: 'net' vs. 'road_net' -> in profile
	QStringList layer_list = {"road","rail","stream"};
	// TODO: 'natural' -> 'field' => from profile!
	QStringList face_list = {"field", "landuse", "building"};
	if(imOsm != nullptr)
	{
		//createNetObjects(layer_list, imOsm, world);
		//createFaceObjects(face_list, imOsm, world);
	}
#ifdef ATKIS
	if(imAtkis != nullptr)
	{
		createNetObjects(layer_list, imAtkis, world);
		createFaceObjects(face_list, imAtkis, world);
	}

	if((m_type == TR_TREAD_IMP_A) && (imAtkis != nullptr))
	{
		TrMapList * poi_map = imAtkis->createPoiMap("poi");
		if(poi_map != nullptr)
			world->addMapLayerObjectByName("poi", poi_map);
		imAtkis->disconnect();
		delete imAtkis;
	}
#endif
	if((m_type == TR_TREAD_IMP_O) && (imOsm != nullptr))
	{
		TrMapList * poi_map = imOsm->createPoiMap("poi");
		if(poi_map != nullptr)
			world->addMapLayerObjectByName("poi", poi_map);
		imOsm->disconnect();
		delete imOsm;
	}
	return world;
}

bool TrProgress::saveList(QString filename, QVector<TrGeoObject *> edit_objects)
{
	// TODO

	return false;
}

void TrProgress::setDocument(TrDocument * doc, int type, int level)
{
	m_doc = doc;
	m_type = type;
	m_level = level;
}

void TrProgress::addList(const QMap<QString, QStringList> & list)
{
	m_list = list;
}

void TrProgress::setProgressBar(QProgressBar * bar)
{
	if(bar == nullptr)
	{
		TR_ERR << "Progress Bar is nullptr";
		return;
	}
	m_bar = bar;
}

