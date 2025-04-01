/******************************************************************
 *
 * @short	node class of trafalgar geometric selectable objects
 *
 * project:	Trafalgar lib
 *
 * class:	TrGeoPoint
 * superclass:	TrMapNode
 * modul:	tr_map_node.cc	geometric selectable object
 * @version	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2015-2025
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



#include <math.h>

#include "tr_map_node.h"
#include "tr_geo_poly.h"
#include "tr_geo_segment.h"

// only for debug output
#include "tr_map_net.h"
// TODO only once used, change?
//#include "tr_map_link_road.h"

TrConnectionMember::TrConnectionMember()
	: m_dir{10.0}
	, tr_obj{nullptr}
{
}

TrConnectionMember::TrConnectionMember(TrGeoObject * tr_mobj)
	: m_dir{10.0}
	, tr_obj{tr_mobj}
{
}

QDebug operator<<(QDebug dbg, const TrConnectionMember& member)
{
	if(member.tr_obj != nullptr)
	{
		TrMapLink * link = dynamic_cast<TrMapLink *>(member.tr_obj);
		if(link != nullptr)
			return dbg << *link << " " << member.m_dir;
	}
	return dbg << member.tr_obj << " " << member.m_dir;
}

void TrConnectionMember::setDir(double adir)
{
	m_dir = adir;
}

TrMapNode::TrMapNode()
	: TrGeoPoint()
	, m_dir_flags(TR_NODE_DIR_EMTY)
	, m_in_flags(0x0000000000000000)
	, m_out_flags(0x0000000000000000)
	, m_shadow(nullptr)
{
	m_inst_mask = (TR_MASK_EXIST | TR_MASK_DRAW);
	//m_geo_id = -1;
}

TrMapNode::TrMapNode (const TrMapNode& other)
	: TrGeoPoint(other)
	, m_shadow(nullptr)
{
	m_dir_flags = other.m_dir_flags;
}

TrMapNode::~TrMapNode()
{
	this->resetRef();
}

QDebug operator<<(QDebug dbg, const TrMapNode& node)
{
	return dbg << node.getXmlName() << node.getGeoId() << "in:" <<
		node.getIn(false) << "out:" << node.getOut(false);
}

QString TrMapNode::getXmlName() const
{
	return "map_node";
}

double TrMapNode::getDeg(double rad)
{
	return (rad / (2*M_PI)) * 360.0;
}

bool TrMapNode::addConnection(TrGeoObject * obj, bool dir)
{
	TrConnectionMember con(obj);
	if(obj != nullptr)
	{
		if(dir == TR_NODE_IN)
			m_vec_in.append(con);
		else
			m_vec_out.append(con);
		//m_vec_out.append(TrPointMember(obj));
	}
	return true;
}

bool TrMapNode::removeConnection(TrGeoObject * obj, bool dir, bool mode)
{
	int idx = getIndexFromObj(obj, dir);

	if(idx < 0)
		return false;
	if(dir == TR_NODE_IN)
	{
		if(mode)
		{
			TrConnectionMember con = m_vec_in[idx];
			m_vec_out.append(con);
		}
		m_vec_in.remove(idx);
	}
	else
	{
		if(mode)
		{
			TrConnectionMember con = m_vec_out[idx];
			m_vec_in.append(con);
		}
		m_vec_out.remove(idx);
	}
	return true;
}

TrGeoObject * TrMapNode::getConLink(int pos, bool dir, double & ang)
{
	QVector<TrConnectionMember> * con = &m_vec_out;
	if(dir)
		con = &m_vec_in;
	if(pos >= con->size())
		return nullptr;
	ang = con->at(pos).m_dir;
	//TR_INF << con->at(pos);
	return con->at(pos).tr_obj;
}

uint8_t TrMapNode::getIn(bool filter) const
{
	if(filter)
	{
		uint8_t n = 0;
		for (int i = 0; i < m_vec_in.size(); ++i)
		{
			if(!(m_in_flags & (1 << i)))
				n++;
		}
		return n;
	}
	return static_cast <uint8_t>(m_vec_in.size());
}

uint8_t TrMapNode::getOut(bool filter) const
{
	if(filter)
	{
		uint8_t n = 0;
		for (int i = 0; i < m_vec_out.size(); ++i)
		{
			if(!(m_out_flags & (1 << i)))
				n++;
		}
		return n;
	}
	return static_cast <uint8_t>(m_vec_out.size());
}

QVector<TrConnectionMember> & TrMapNode::getConVec(bool dir)
{
	if(dir)
		return m_vec_in;
	else
		return m_vec_out;
}

TrMapNode * TrMapNode::getShadow(bool orig)
{
	if(orig)
	{
		if(!(m_dir_flags & TR_NODE_IS_SHADOW))
			return nullptr;
	}
	else
	{
		if(m_dir_flags & TR_NODE_IS_SHADOW)
			return nullptr;
	}
	return dynamic_cast<TrMapNode *>(m_shadow);
}

void TrMapNode::resetIoOut()
{
	m_vec_in.clear();
	m_vec_out.clear();
	if(m_dir_flags & TR_NODE_IS_SHADOW)
		m_dir_flags = (TR_NODE_IS_SHADOW | TR_NODE_DIR_EMTY);
	else
		m_dir_flags = TR_NODE_DIR_EMTY;
	//TR_INF << HEX << m_dir_flags;
}

bool TrMapNode::setDirFlags()
{
	if((m_vec_in.size() == 0) && (m_vec_out.size() == 0))
	{
		if(m_dir_flags & TR_NODE_IS_SHADOW)
			m_dir_flags = (TR_NODE_IS_SHADOW | TR_NODE_DIR_EMTY);
		else
			m_dir_flags = TR_NODE_DIR_EMTY;
		return false;
	}
	m_dir_flags &= ~(TR_LINK_DIR_ONEWAY);
	for (TrConnectionMember item : m_vec_in)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			m_dir_flags |= TR_NODE_DIR_ONE;
		}
	}
	for (TrConnectionMember item : m_vec_out)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link->getOneWay() & TR_LINK_DIR_ONEWAY)
		{
			m_dir_flags |= TR_NODE_DIR_ONE;
		}
	}
	//TR_INF << HEX << m_dir_flags;
	return true;
}

int TrMapNode::getShiftPoint(const TrZoomMap & zoom_ref)
{
	for (TrConnectionMember item : m_vec_out)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link != nullptr)
		{
			if((next_link->getType() & 0x000f) < 9)
			{
				if(!(next_link->getOneWay() & TR_LINK_DIR_ONEWAY))
					return false;
			}
		}
	}
	//for (TrConnectionMember item : m_vec_out)
	// ....
	return true;
}

TrGeoObject * TrMapNode::getSingleElement(int mode)
{
	int count = 0;
	TrGeoObject * ret = nullptr;
	for (TrConnectionMember item : m_vec_out)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link != nullptr)
		{
			if((next_link->getType() & 0x000f) < 9)
			{
				//TR_INF << "O:" << *next_link << mode;
				if(next_link->getOneWay() & TR_LINK_DIR_ONEWAY)
				{
					if((mode == 1) && (!count))
					{
						//TR_INF << "Ox:" << *next_link << mode;
						ret = next_link;
						count++;
					}
				}
				else
				{
					if((mode == 3) && (!count))
					{
						//TR_INF << "O-:" << *next_link << mode;
						ret = next_link;
						count++;
					}
				}
			}
		}
	}
	if((ret == nullptr) && (count > 1))
	{
		return nullptr;
	}
	count = 0;
	for (TrConnectionMember item : m_vec_in)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link != nullptr)
		{
			if((next_link->getType() & 0x000f) < 9)
			{
				//TR_INF << "I:" << *next_link << mode;
				if(next_link->getOneWay() & TR_LINK_DIR_ONEWAY)
				{
					if((mode == 2) && (!count))
					{
						//TR_INF << "II:" << count << *next_link << mode;
						ret = next_link;
						count++;
					}
				}
			}
		}
	}
	if((ret == nullptr) && (count > 1))
		return nullptr;
	return ret;
}

bool TrMapNode::setCrossingByAngle(const TrZoomMap & zoom_ref, bool type)
{
	TrGeoObject * first_obj = nullptr;
	TrGeoObject * next_obj = nullptr;
	TrGeoObject * last_obj = nullptr;
	TrGeoObject * test_obj = nullptr;
	double ang = -0.0001;
	int idx = -1;
	do
	{
		bool dir = true;
		idx = getDirNextAngleIndex(ang, dir);

		//TR_INF << idx << getDeg(ang) << dir;
		if(idx >= 0)
		{
			if(dir)
				test_obj = m_vec_in[idx].tr_obj;
			else
				test_obj = m_vec_out[idx].tr_obj;
		}
		if(((test_obj->getType() & 0x000f) <= 9) || type)
		{
			next_obj = test_obj;
			if(last_obj == nullptr)
				last_obj = next_obj;
			if(idx >= 0)
				setCrossing(zoom_ref, first_obj, next_obj);
			first_obj = next_obj;
		}
	}while(idx >= 0);
	setCrossing(zoom_ref, first_obj, last_obj);

	return true;
}


bool TrMapNode::init(const TrZoomMap & zoom_ref, uint64_t ctrl, TrGeoObject * base)
{
	int fork = 0;

	if(ctrl & TR_INIT_COLORS)
	{
		if(base != nullptr)
		{
			TrMapList * list = dynamic_cast<TrMapList *>(base);
			if(list != nullptr)
			{
				// text -> 1002;
				QPen * str_pen = list->getObjectPen(0x1002);
				// TODO check rule!; create a 'shadow rule'?
				// TODO remove the 'm_flags' parameter
				int code = checkTwoByTwo();
				if(code & 2)
					str_pen = list->getObjectPen(0x1020);
				if(code & 1)
					str_pen = list->getObjectPen(0x1021);
				if(str_pen != nullptr)
				{
					m_geo_active_pen = str_pen;
				}
			}
		}
	}

	if((ctrl & 0xff) == 33)
	{
		// code to set the angles
		setConnectionAngles(zoom_ref, TR_NODE_OUT);
		setConnectionAngles(zoom_ref, TR_NODE_IN);
	}

	// find the shift point
	if((ctrl & 0xff) == 44)
	{
		//TODO: shift...
		//getShiftPoint(zoom_ref);
	}

	if((ctrl & 0xff) == 21)
	{
		// getIn == 2
		// TODO: rework, simplify...
		if(checkTwoFork(true, true))
		{
			//getOut == 1
			fork = 1;
		}
		// getOut == 2
		if(checkTwoFork(false, true))
		{
			fork = 2;
			//return true;
		}
		//m_test.clear();
		//TR_INF << "in out" << getIn(false) << getOut(false);
		if((getIn(false) <= 1) && (getOut(false) <= 1))
		{
			return true;
		}
		// TODO: one to one double dir -> more checking...
		if((getIn(false) == 2) && (getOut(false) == 2))
		{
			//return true;
		}
		// TODO: check: is needed! -> old? double used?
		setConnectionAngles(zoom_ref, TR_NODE_OUT);
		setConnectionAngles(zoom_ref, TR_NODE_IN);

		if(fork == 1)
		{
			TrGeoObject * next_obj = m_vec_out[0].tr_obj;
			TrMapLink * next_link = dynamic_cast<TrMapLink *>(next_obj);
			if(next_link != nullptr)
			{
				if(next_link->setRamp(zoom_ref, true))
					return true;
			}
		}
		if(fork == 2)
		{
			TrGeoObject * next_obj = m_vec_in[0].tr_obj;
			TrMapLink * next_link = dynamic_cast<TrMapLink *>(next_obj);
			if(next_link != nullptr)
			{
				if(next_link->setRamp(zoom_ref, false))
					return true;
			}
			//return true;
		}

		setCrossingByAngle(zoom_ref, false);

		return true;
	}
	return TrGeoPoint::init(zoom_ref, ctrl, base);
}

TrGeoObject * TrMapNode::getParallelElement(TrGeoObject * element, bool dir)
{
	TrMapLink * link = dynamic_cast<TrMapLink *>(element);
	if(link == nullptr)
		return link;
	if(dir)
	{
		for (TrConnectionMember item : m_vec_out)
		{
			TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
			if(next_link != nullptr)
			{
				//TR_INF << "next to" << link->getNodeFrom() << next_link->getNodeTo();
				TrMapNode * node = next_link->getNodeToRef()->getShadow(false);
				if(node != nullptr)
				{
					if(link->getNodeFrom() == node->getGeoId())
						return item.tr_obj;
				}
				if(link->getNodeFrom() == next_link->getNodeTo())
					return item.tr_obj;
			}
		}
	}
	else
	{
		for (TrConnectionMember item : m_vec_in)
		{
			TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
			if(next_link != nullptr)
			{
				TrMapNode * node = next_link->getNodeFromRef()->getShadow(true);
				if(node != nullptr)
				{
					if(link->getNodeTo() == node->getGeoId())
						return item.tr_obj;
				}
				if(link->getNodeTo() == next_link->getNodeFrom())
					return item.tr_obj;
			}
		}
	}
	return nullptr;
}

// for ramp mode
TrGeoObject * TrMapNode::getElement(uint8_t count, bool dir, double & angle)
{
	QVector<TrConnectionMember> * vec = &m_vec_out;
	uint64_t filter = m_out_flags;
	if(dir)
	{
		vec = &m_vec_in;
		filter = m_in_flags;
	}
	if(vec->size() > 64)
		return nullptr;
	uint8_t n = 0;
	for (int i = 0; i < vec->size(); ++i)
	{
		if(filter & (1 << i))
		{
			n++;
		}
		else
		{
			if(i >= count)
			{
				if((count + n) < vec->size())
				{
					angle = vec->at(count + n).m_dir;
					return vec->at(count + n).tr_obj;
				}
			}
		}
	}
	return nullptr;
}

TrGeoObject * TrMapNode::getNextOutElement(double angle)
{
	int idx = -1;
	for (int i = 0; i < m_vec_out.size(); ++i)
	{
		double check = fabs(angle - m_vec_out[i].m_dir);
		if((check > 0.5) && (idx == (-1)))
		{
			idx = i;
		}
	}
	if(idx != (-1))
		return m_vec_out[idx].tr_obj;
	return nullptr;
}

void TrMapNode::setCrossing(const TrZoomMap & zoom_ref, TrGeoObject * first_obj, TrGeoObject * next_obj)
{
	if((first_obj == nullptr) || (next_obj == nullptr))
		return;
	TrMapLink * first_link = dynamic_cast<TrMapLink *>(first_obj);
	TrMapLink * next_link = dynamic_cast<TrMapLink *>(next_obj);

	if((first_link == nullptr) || (next_link == nullptr))
	{
		TR_WRN << "class missmatch";
		return;
	}

	TrGeoSegment first_segment;
	TrGeoSegment next_segment;
	TrPoint cross_pt;

	//TR_INF << "first";
	first_obj = first_link->getSegmentWithParm(first_segment, getGeoId(), false);
	//TR_INF << "next";
	next_obj = next_link->getSegmentWithParm(next_segment, getGeoId(), true);
	if((first_segment.getLength(zoom_ref) < (first_link->getWidth()/1000.0)) &&
		(next_segment.getLength(zoom_ref) < (next_link->getWidth()/1000.0)))
		return;

	if((first_obj == nullptr) || (next_obj == nullptr))
		return;

	first_link = dynamic_cast<TrMapLink *>(first_obj);
	next_link = dynamic_cast<TrMapLink *>(next_obj);

	if((first_link == nullptr) || (next_link == nullptr))
		return;

	int code = first_segment.getAngleCode(zoom_ref, next_segment);
	if(code)
	{
		if(code < 0)
			TR_ERR << "unable to set the angle" << code;
		//TR_INF << "crossing angle code:" << code;
		return;
	}
	first_segment.getCrossPoint(zoom_ref, cross_pt, next_segment);

	if(first_link->getOneWay() & TR_LINK_DIR_ONEWAY)
	{
		if(first_link->getNodeToRef() == this)
		{
			first_link->setParPoint(false, cross_pt);
		}
		else
		{
			first_link->setCrossingPoint(cross_pt, true);
		}
	}
	else
	{
		if(first_link->getOneWay() & TR_LINK_DIR_BWD)
			first_link->setParPoint(true, cross_pt);
		else
			first_link->setParPoint(false, cross_pt);
	}
	if(next_link->getOneWay() & TR_LINK_DIR_ONEWAY)
	{
		if(next_link->getNodeToRef() == this)
		{
			next_link->setCrossingPoint(cross_pt, false);
		}
		else
		{
			next_link->setParPoint(true, cross_pt);
		}
	}
	else
	{
		if(next_link->getOneWay() & TR_LINK_DIR_BWD)
			next_link->setParPoint(false, cross_pt);
		else
			next_link->setParPoint(true, cross_pt);
	}
	//TR_INF << *first_link << *next_link;
}

void TrMapNode::initConnections(const TrZoomMap & zoom_ref, QVector<TrConnectionMember> & vec,
		TrMapList * pr_list, TrMapList & nd_list)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(vec[i].tr_obj);
		if(next_link != nullptr)
		{
			next_link->init(zoom_ref, TR_INIT_GEOMETRY);
			next_link->initSecs(zoom_ref, pr_list, nd_list);
			next_link->setSurroundingRect();
		}
	}
}

// special version of init with both lists
void TrMapNode::initCon(const TrZoomMap & zoom_ref, TrMapList * pr_list, TrMapList & nd_list)
{
	initConnections(zoom_ref, m_vec_out, pr_list, nd_list);
	initConnections(zoom_ref, m_vec_in, pr_list, nd_list);
}

bool TrMapNode::setDirectionAngles(const TrZoomMap & zoom_ref, QVector<TrConnectionMember> & vec, bool dir)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(vec[i].tr_obj);
		if(next_link == nullptr)
			return false;
		vec[i].m_dir = next_link->getAngle(zoom_ref, dir);
	}
	return true;
}

//SEGMENT_TO true
bool TrMapNode::setConnectionAngles(const TrZoomMap & zoom_ref, bool dir)
{
	setDirFlags();
	if(dir)
	{
		//TR_INF << "IN" << dir;
		return setDirectionAngles(zoom_ref, m_vec_in, dir);
	}
	else
	{
		//TR_INF << "OUT" << dir;
		return setDirectionAngles(zoom_ref, m_vec_out, dir);
	}
}

int TrMapNode::getDirNextAngleIndex(double & angle, bool & dir)
{
	double act_ang_in = 10.0;
	double act_ang_out = 10.0;
	int idx_in = -1;
	int idx_out = -1;

	for (int i = 0; i < m_vec_in.size(); ++i)
	{
		if((m_vec_in[i].m_dir > angle) && (m_vec_in[i].m_dir <= act_ang_in))
		{
			act_ang_in = m_vec_in[i].m_dir;
			idx_in = i;
		}
	}
	for (int i = 0; i < m_vec_out.size(); ++i)
	{
		if((m_vec_out[i].m_dir > angle) && (m_vec_out[i].m_dir <= act_ang_out))
		{
			act_ang_out = m_vec_out[i].m_dir;
			idx_out = i;
		}
	}
	if(act_ang_in > act_ang_out)
	{
		angle = act_ang_out;
		dir = false;
		return idx_out;
	}
	else
	{
		angle = act_ang_in;
		dir = true;
		return idx_in;
	}
}


int TrMapNode::getDirNextAngle(TrConnectionMember & member, QVector<TrConnectionMember> & vec, bool dir)
{
	double diff = 10.0;
	int con_count = 0;
	int ret = 1;
	double ret_ang = 10.0;

	TrMapLink * base_link = dynamic_cast<TrMapLink *>(member.tr_obj);
	if(base_link == nullptr)
		return 1;

	member.tr_obj = nullptr;

	for (TrConnectionMember item : vec)
	{
		//TR_MSG << item.m_dir;
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(item.tr_obj);
		if(next_link == nullptr)
			return 1;		// nullptr
		if(!next_link->isAsDoubleLine())
		{
			//TR_MSG << class: " << link->getRdClass();
			if(ret)
				ret = 4;
			continue;
		}
		if(dir)
		{
			if((member.tr_obj == nullptr) && (next_link != nullptr))
				member.tr_obj = next_link;
			if(base_link->getNodeFrom() == next_link->getNodeFrom())
			{
				continue;
			}
		}
		else
		{
			if(base_link->getNodeFrom() == next_link->getNodeTo())
			{
				if(ret)
					ret = 5;
				continue;
			}
		}
		double a = item.m_dir - geoInvertAngle(member.m_dir);
		if(a < 0)
			a += (2*M_PI);

		if(a < diff)
		{
			member.tr_obj = next_link;
			ret_ang = item.m_dir;
			diff = a;
			ret = 0;
		}
		con_count++;
	}
	//TR_INF << dir << member.tr_obj;
	// return the one to one connection
	// check the change of the width of the links
	if(con_count == 1)
	{
		// TODO, needed?
		//return 3;
	}
	member.m_dir = ret_ang;
	TR_MSG << "ret: " << ret << member.m_dir;
	return ret;
}

int TrMapNode::getNextAngleObj(TrConnectionMember & member, bool dir)
{
	// TODO: if (node oneway test flag) -> vec = m_vec_in + m_vec_out;
	if(dir)
		member.m_dir = geoInvertAngle(member.m_dir);
	if(dir)
	{
		//TR_INF << m_vec_in;
		return getDirNextAngle(member, m_vec_in, dir);
		// TODO: impl the m_vec_in connections
	}
	else
	{
		return getDirNextAngle(member, m_vec_out, dir);
	}
}

int TrMapNode::hasRamp(QVector<TrConnectionMember> & vec)
{
	int count_ramp = 0;

	for (int i = 0; i < vec.size(); ++i)
	{
		TrMapLink * next_link = dynamic_cast<TrMapLink *>(vec[i].tr_obj);
		if(next_link != nullptr)
		{
			if(next_link->getType() & TR_LINK_RAMP_FLAG)
				count_ramp++;
			//m_type & TR_LINK_RAMP_FLAG
		}
	}
	return count_ramp;
}

// if all are oneway links
bool TrMapNode::checkTwoFork(bool dir, bool filter)
{
	if(dir)
	{
		if((getIn(filter)  == 2) && (getOut(filter) == 1))
		{
			if((hasRamp(m_vec_in) == 1) && (getIn(filter)  == 2))
				return true;
		}
	}
	else
	{
		if ((getIn(filter)  == 1) && (getOut(filter) == 2))
		{
			if((hasRamp(m_vec_out) == 1) && (getOut(filter)  == 2))
				return true;
		}
	}
	return false;
}

int TrMapNode::checkTwoByTwo()
{
	if((m_vec_in.size() == 1) && (m_vec_out.size() == 1))
	{
		TrGeoObject * obj = m_vec_in[0].tr_obj;
		if(getParallelElement(obj, true) == m_vec_out[0].tr_obj)
		{
			return 4; //3;
		}
		// ono to one (-->*-->)
		return 1;
	}
	if((m_vec_in.size() == 2) && (m_vec_out.size() == 2))
	{
		TrGeoObject * p_obj = getParallelElement(m_vec_in[0].tr_obj, true);
		if((m_vec_out[0].tr_obj == p_obj) || (m_vec_out[1].tr_obj == p_obj))
		{
			p_obj = getParallelElement(m_vec_in[1].tr_obj, true);
			if((m_vec_out[0].tr_obj != p_obj) && (m_vec_out[0].tr_obj != p_obj))
			{
				// two to two (<==>*<==>)
				return 2;
			}
		}
	}
	return 0;
}


void TrMapNode::setShadowNode(TrMapNode * node)
{
	if(m_shadow == nullptr)
	{
		m_shadow = node;
		m_dir_flags |= TR_NODE_IS_SHADOW;
		TR_MSG << m_shadow << getGeoId() << HEX << m_dir_flags;
	}
}

bool TrMapNode::markDivider(TrMapList & list, TrGeoObject * obj)
{
	if(m_shadow == nullptr)
	{
		// create m_shadow node
		m_shadow = new TrMapNode(*this);
		uint64_t node_id = list.createMapNextId();
		list.appendObject(m_shadow, node_id);
		TrMapNode * shadow = dynamic_cast<TrMapNode *>(m_shadow);
		shadow->setGeoId(node_id);
		if(shadow != nullptr)
			shadow->setShadowNode(this);
	}

	//TrMapLink * obj_link = dynamic_cast<TrMapLink *>(obj);
	//TR_INF << "this" << *this << *obj_link;
	TrGeoObject * par1 = getParallelElement(obj, true);
	if(par1 == nullptr)
		return false;
	TrMapLink * par_link1 = nullptr;
	TrMapLink * par_link2 = nullptr;
	if(par1 != nullptr)
	{
		par_link1 = dynamic_cast<TrMapLink *>(par1);
		//TR_INF << "base par" << *par_link1;
		if(par_link1 == nullptr)
			return false;
	}
	TrGeoObject * par2 = nullptr;
	removeConnection(par1, false, false);

	for (int i = 0; i < m_vec_out.size(); ++i)
	{
		par2 = getParallelElement(m_vec_out[i].tr_obj, false);
		if(par2 != nullptr)
		{
			par_link2 = dynamic_cast<TrMapLink *>(par2);
			par_link1->switchShadowNode(false);
			par_link2->switchShadowNode(true);
			removeConnection(par2, true, false);
		}
	}
	return false;
}

int TrMapNode::getIndexFromObj(const TrGeoObject * obj, bool dir)
{
	QVector<TrConnectionMember> * vec = &m_vec_out;
	if(dir)
		vec = &m_vec_in;

	for (int i = 0; i < vec->size(); ++i)
	{
		if(vec->at(i).tr_obj == obj)
			return i;
	}
	return -1;
}

bool TrMapNode::switchLinksByAngle(double ang1, double ang2, bool dir)
{
	double diff = ang1 - ang2;
	//TR_INF << "NEW1" << ang1 << ang2 << diff;
	if(diff < -M_PI)
	{
		diff += (2*M_PI);
	}
	if(diff > M_PI)
	{
		diff -= (2*M_PI);
	}
	bool do_change = false;
	if(dir && (diff > 0.0))
		do_change = true;
	if(!dir && (diff < 0.0))
		do_change = true;
	//TR_INF << "NEW2" << ang1 << ang2 << do_change;
	return do_change;
}

double TrMapNode::switchLinksByAngle(QVector<TrConnectionMember> & vec, bool dir)
{
	double diff = 10.0;

	if(vec.size() != 2)
		return diff;
	diff = vec.value(1).m_dir - vec.value(0).m_dir;
	//TR_INF << "OLD" << vec.value(1).m_dir << vec.value(0).m_dir << diff;
	if(diff < -M_PI)
	{
		diff += (2*M_PI);
		//TR_MSG << "ret" << angle1 << angle2 << diff;
	}
	if(diff > M_PI)
	{
		diff -= (2*M_PI);
	}
	bool do_change = false;
	if(dir && (diff > 0.0))
		do_change = true;
	if(!dir && (diff < 0.0))
		do_change = true;
	//TR_INF << do_change;
	if(do_change)
	{
		TrConnectionMember sw = vec[0];
		vec[0] = vec[1];
		vec[1] = sw;
	}
	return diff;
}

int TrMapNode::checkAngle(TrConnectionMember & base, double angle, double limit)
{
	double diff = abs(base.m_dir - angle);

	if((angle > 9) || (base.m_dir > 9))
		return -1;
	if(diff < limit)
		return 1;
	if((diff < (M_PI + limit)) && (diff > (M_PI - limit)))
		return 2;
	return 0;
}

void TrMapNode::draw(const TrZoomMap & zoom_ref, QPainter * p, unsigned char mode)
{
	//TR_INF << m_dir_flags << getGeoId();
	if(getActivePen() == nullptr)
	{
		TR_WRN << "getActivePen() == nullptr" << *this;
		return;
	}
	if(this->clip(zoom_ref))
                return;

	if(m_inst_mask & TR_MASK_SELECTED)
		drawSelect(zoom_ref, p, mode);

	int p_mode = 0;
	if(m_dir_flags & TR_NODE_IS_SHADOW)
		p_mode = 1;

	// TODO check crash
	p->setPen(*getActivePen());
	// TODO: remove hardcoded color
	p->setBrush(QBrush(QColor(220,174,192)));

	/* test code
	p->setPen(QPen(QColor(0,0,200)));
	for (int i = 0; i < m_test.size(); ++i)
	{
		TrPoint screen_1 = m_test[i];
		zoom_ref.setMovePoint(&screen_1.x,&screen_1.y);
		p->drawRect(screen_1.x-5, screen_1.y-5, 10, 10);
	}*/

	if(s_mask & TR_MASK_SHOW_POINTS)
	{
		drawElement(zoom_ref, p, p_mode);
		drawText(zoom_ref, p, "# ");
	}
	drawElement(zoom_ref, p, p_mode);
}

void TrMapNode::drawSelect(const TrZoomMap & zoom_ref, QPainter * p, uint8_t mode)
{
	TrGeoPoint::drawSelect(zoom_ref, p, mode);
}

bool TrMapNode::setSurroundingRect()
{
	return TrGeoPoint::setSurroundingRect();
}

QString TrMapNode::getXmlDescription()
{
	return QString();
}

#ifdef TR_SERIALIZATION
uint64_t TrMapNode::readXmlDescription(QXmlStreamReader & xml_in)
{
	return TrGeoPoint::readXmlDescription(xml_in);
}

void TrMapNode::writeXmlDescription(QXmlStreamWriter & xml_out, uint64_t id)
{
	if(!(m_dir_flags & TR_NODE_IS_SHADOW))
		TrGeoPoint::writeXmlDescription(xml_out, id);
	else
		TR_INF << "TR_NODE_IS_SHADOW";
}
#endif

