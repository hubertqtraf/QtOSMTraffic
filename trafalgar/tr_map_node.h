/******************************************************************
 *
 * @short	node class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoint
 * superclass:	TrMapNode
 * modul:	tr_map_node.h	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2015-2022
 *
 * beginning:	06.2015
 *
 * history:
 */
/******************************************************************/

/* The trafalgar package is free software.  You may redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software foundation; either version 2, or (at your
 * option) any later version.
 *
 * The GNU trafalgar package is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with the GNU plotutils package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin St., Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef TR_MAP_NODE_H
#define TR_MAP_NODE_H

#include "tr_geo_point.h"
#include "tr_geo_segment.h"
#include "tr_map_list.h"

#define TR_NODE_DIR_EMTY 0x01
#define TR_NODE_DIR_ONE  0x02

#define TR_NODE_DIR_FORK_IN  0x10
#define TR_NODE_DIR_FORK_OUT 0x20

// used in m_dir_flags
#define TR_NODE_IS_SHADOW 0x80

#define TR_NODE_IN  true
#define TR_NODE_OUT false

struct TrConnectionMember
{
	double m_dir;
	TrGeoObject * tr_obj;

	TrConnectionMember();
	TrConnectionMember(TrGeoObject * tr_mobj);

	friend QDebug operator<<(QDebug dbg, const TrConnectionMember& member);

	void setDir(double adir);
};

class TrMapNode : public TrGeoPoint
{
private:
	uint8_t m_dir_flags;
	// TODO: m_in_flags, m_out_flags -> remove?
	uint64_t m_in_flags;		// blocked connection(s)
	uint64_t m_out_flags;
	TrGeoObject * m_shadow;
	QVector<TrConnectionMember> m_vec_in;	// to link(s)
	QVector<TrConnectionMember> m_vec_out;	// from link(s)
	// TODO: only debug test
	//QVector<TrPoint> m_test;

	bool setDirectionAngles(const TrZoomMap & zoom_ref, QVector<TrConnectionMember> & vec, bool dir);
	int getDirNextAngle(TrConnectionMember & member, QVector<TrConnectionMember> & vec, bool dir);
	//
	int getDirNextAngleIndex(double & angle, bool & dir);
	void setCrossing(const TrZoomMap & zoom_ref, TrGeoObject * first_obj, TrGeoObject * next_obj);

	void initConnections(const TrZoomMap & zoom_ref, QVector<TrConnectionMember> & vec,
			TrMapList * pr_list, TrMapList & nd_list);

	// TODO: to remove?
	QVector<TrConnectionMember> & getConVec(bool dir);
	int hasRamp(QVector<TrConnectionMember> & vec);

	bool setCrossingByAngle(const TrZoomMap & zoom_ref, bool type);

protected:

public:
	TrMapNode();

	TrMapNode (const TrMapNode& other);

	virtual ~TrMapNode();

	friend QDebug operator<<(QDebug dbg, const TrMapNode& seg);

	QString getXmlName() const;

	static double getDeg(double rad);

	bool addConnection(TrGeoObject * obj, bool dir);

	bool removeConnection(TrGeoObject * obj, bool dir, bool mode);

	uint8_t getIn(bool filter) const;

	uint8_t getOut(bool filter) const;

	TrGeoObject * getConLink(int pos, bool dir, double & ang);

	TrMapNode* getShadow(bool orig);

	void resetIoOut();

	bool setDirFlags();

	int getShiftPoint(const TrZoomMap & zoom_ref);

	TrGeoObject * getSingleElement(int mode);

	bool init(const TrZoomMap & zoom_ref, uint64_t ctrl = 0, TrGeoObject * base = nullptr);

	// TODO rework...
	void initCon(const TrZoomMap & zoom_ref, TrMapList * pr_list, TrMapList & nd_list);

	TrGeoObject * getElement(uint8_t n, bool dir, double & angle);

	TrGeoObject * getParallelElement(TrGeoObject * element, bool dir);

	TrGeoObject * getNextOutElement(double angle);

	bool setConnectionAngles(const TrZoomMap & zoom_ref, bool dir);

	int getNextAngleObj(TrConnectionMember & member, bool dir);

	bool checkTwoFork(bool dir, bool filter);

	int checkTwoByTwo();

	void setShadowNode(TrMapNode * node);

	bool markDivider(TrMapList & list, TrGeoObject * obj);

	int getIndexFromObj(const TrGeoObject * obj, bool dir);

	bool static switchLinksByAngle(double ang1, double ang2, bool dir);

	double static switchLinksByAngle(QVector<TrConnectionMember> & vec, bool dir);

	int static checkAngle(TrConnectionMember & base, double angle, double limit);

	virtual void draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode = 0);

	void drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode = 0);

	bool setSurroundingRect();

	void setInfo(const TrZoomMap & zoom_ref);

	QString getXmlDescription();
#ifdef TR_SERIALIZATION
	virtual uint64_t readXmlDescription(QXmlStreamReader & xml_in);

	virtual void writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id);
#endif
};

#endif // TR_MAP_NODE_H
