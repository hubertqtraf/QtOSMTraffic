/******************************************************************
 * project:	OSM Traffic
 *
 * class:	TrProgress
 * superclass:	QThread
 * modul:	tr_progress_thread.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	03.2017
 *
 * (C)		Schmid Hubert 2017-2025
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

#ifndef _TR_PROGRESS_H
#define _TR_PROGRESS_H

#include "tr_document.h"
//#include "tr_map_route.h"

#include <tr_import_osm.h>

#include <QtCore/qthread.h>
#include <QtWidgets/qprogressbar.h>

#define TR_TREAD_IMP     1
#define TR_TREAD_EXP     2
#define TR_TREAD_IMP_O   3
#define TR_TREAD_IMP_A   4

class TrProgress : public QThread
{
	Q_OBJECT

private:
	const TrGeoObject * m_world;
	TrDocument * m_doc;
	int m_type;
	int m_level;
	QProgressBar * m_bar;
	QMap<QString, QStringList> m_list;

	// overriding the QThread's run() method
	void run();

	TrGeoObject * loadDoc(TrDocument * world);
	bool readDocument(TrDocument * doc);
	TrGeoObject * importDoc(TrDocument * world);
	// TODO: option, save the file in thread, was removed because there is duplicated code

	bool saveList(QString filename, QVector<TrGeoObject *> edit_objects);
	//void createNetObjects(const QStringList & list, ITrImportLayer * filter, TrDocument * doc);
	//void createFaceObjects(const QStringList & list, ITrImportLayer * filter, TrDocument * doc);

signals:
	void resultReady(const TrGeoObject ** result);

public:
	explicit TrProgress(QObject *parent = nullptr);

	void setDocument(TrDocument * doc, int type, int level);
	void addList(const QMap<QString, QStringList> & list); //const QStringList & list, const QString & name);
	void setProgressBar(QProgressBar * bar);
};

#endif // _TR_PROGRESS_H

