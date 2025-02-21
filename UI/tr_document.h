/******************************************************************
 * project:	Trafalgar/GUI
 *
 * class:	TrDocument
 * superclass:	TrGeoObject
 * modul:	tr_document.h
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * beginning:	11.2012
 *
 * (C)		Schmid Hubert 2012-2022
 *
 * history:
 *  file: tr_map_world -> tr_document
 *  dir:  view -> lib/gui
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



#ifndef TR_DOCUMENT_H
#define TR_DOCUMENT_H

#include <tr_geo_object.h>

#include <tr_stack.h>
#include <tr_map_list.h>

class TrDocument : public QObject, public TrGeoObject
{
	Q_OBJECT

private:
	// layer name
	QString m_name;

	// 'world' stack: roads, rail, ..,
	TrStack m_map_stack;

	// TODO: rework, names and poi's -> add as layers?
	// names of links/nodes
	TrMapList m_name_map;

	// XML file name
	QString m_fname;

	QString m_selection_layer;

	QVector<double> m_rect;

	// bool addColors(QDomNode & col_nd);

public:
    bool m_is_dirty;
    bool m_is_loaded;

	TrDocument(const QString & name, QObject * parent = nullptr);

    TrDocument(QObject * parent = nullptr);

	virtual ~TrDocument();

    void clean();

	virtual QString getName() const;

    virtual bool setName(const QString & name);

	const QString & getFileName();

	void setFileName(const QString & fname);

    uint64_t getSettings() const;

	const QStringList getLayerNames();

	void setSelectionLayer(const QString & layer);

	const QString & getSelectionLayer() const;

	// TODO: rework, names -> add as layers?
	TrMapList & getNameList();

	void resetNameList();

	virtual bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	bool setLayerItemData(const QMap<QString, uint64_t> & layers);

	void createEmtyLayers(const QStringList & layer_names);

	void addLayerType(const QList<QString> & type_list);

	void addMapLayerObjectByName(const QString layer_name, TrGeoObject * obj);

	void appendListMembers(QStringList & list);

	void addOrderByType(const QString & type, const QList<QString> & layer_list);

	void setFunctionOrder(const QStringList func, const QString & type);

	TrGeoObject * getLayerObjectByName(const QString & name);

	TrGeoObject * getLayerObjectBySelection();

	uint64_t findSelect(const TrZoomMap & zoom_ref, const TrPoint & inside, uint64_t pos);

	uint64_t editElement(const TrZoomMap & zoom_ref, TrPoint & set, QVector<uint64_t> & ids);

	bool save(const QString & filename);

	void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode);

	bool setSurroundingRect();

	QVector<double> getSurroundingVecRect();

	void setSurroundingVecRect(const QVector<double> & rect);

	int checkFileHeader(const QXmlStreamAttributes & attrs);
#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif

signals:
	void valueChanged(int value);

};

#endif // TR_DOCUMENT_H

