/******************************************************************
 *
 * @short	geometrical functions  
 *
 * project:	Trafalgar (4.0)
 *
 * modul:	geo_ref.h	geometrical  function defs	
 * @version:	0.2
 *
 * system:	UNIX/LINUX
 * compiler:	gcc
 *
 * @author	Schmid Hubert (C)2005-2014
 *
 * beginning:	12.2005
 *
 * history:
 ******************************************************************/


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


#ifndef _geo_ref_h
#define _geo_ref_h

typedef struct
{
	double (*trg_x)(void * ref, double x, double y);
	double (*trg_y)(void * ref, double x, double y);
	double (*trg_orig_x)(void * ref, double x, double y);
	double (*trg_orig_y)(void * ref, double x, double y);
	double (*trg_len)(void * ref, double x1, double y1,
		double x2, double y2);
	double (*trg_local)(void * ref, double x, double y);
	int error_code;
}TrGeo2DCore;

typedef struct
{
        TrGeo2DCore core;
}TrGeo2DRef;

void GeoIoRefInit(TrGeo2DRef * ref);
void GeoIoRefInitDefault(TrGeo2DRef * ref);

double Geo2DX(TrGeo2DRef * ref, double x, double y);
double Geo2DY(TrGeo2DRef * ref, double x, double y);
double Geo2DOrigX(TrGeo2DRef * ref, double x, double y);
double Geo2DOrigY(TrGeo2DRef * ref, double x, double y);
double Geo2DLen(TrGeo2DRef * ref, double x1, double y1, double x2, double y2);
double Geo2DLocal(TrGeo2DRef * ref, double x, double y);

#endif // _geo_ref_h

