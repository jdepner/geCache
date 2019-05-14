
/********************************************************************************************* 

    geCacheDef.hpp

    Copyright (c) 2016, Jan C. Depner


    This file is part of geCache.

    geCache is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    geCache is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with geCache.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************************************/


#ifndef _GE_CACHE_DEF_HPP_
#define _GE_CACHE_DEF_HPP_


#include "functions.h"
#include "qPosfix.hpp"


#ifdef _MSC_VER

  #define ATTR_UNUSED

#else

  #define ATTR_UNUSED __attribute__((unused))

  #include <signal.h>

#endif


#include <QtCore>
#include <QtGui>
#include <QtWidgets>


using namespace std;


#define ABOUT_TAB      0
#define CACHE_TAB      1
#define PREF_TAB       2

#define RECT_TAB       0
#define POLY_TAB       1

#define NO_BOUNDS      -1
#define NW_BOUNDS      0
#define NE_BOUNDS      1
#define SW_BOUNDS      2
#define SE_BOUNDS      3
#define NORTH_BOUNDS   4
#define WEST_BOUNDS    5
#define EAST_BOUNDS    6
#define SOUTH_BOUNDS   7


//  The OPTIONS structure contains all those variables that can be saved to the users geCache QSettings.

typedef struct
{
  int32_t           position_form;              //  Position format number
  QFont             font;                       //  Uh... font?
  QString           stash_dir;                  //  Last directory used to save or load a cache directory
  NV_F64_XYMBR      cache_mbr;                  //  Minimum bounding rectangle for the cache preview or build
  int32_t           build_box_size;             //  The cache build initial area size
  int32_t           cache_update_frequency;     //  Update frequency in seconds for cache building
  int32_t           icon_size;                  //  Button icon size in pixels
  QString           ge_name;                    //  Name of the Google Earth executable or script
  QString           ge_dir;                     //  Path to the GoogleEarth folder (Windows) or path to the .googleearth/Cache directory (Linux)
  QColor            warning_color;              //  Color used for buttons that have active running processes associated with them (e.g. Build cache)
  int32_t           start_tab;                  //  Whatever tab you were on when you closed geCache (this is where you'll start next time)
  int32_t           shape_tab;                  //  The current shape tab
  std::vector<NV_F64_COORD2> polygon;           //  Polygon points
  int32_t           window_width;               //  Main window width
  int32_t           window_height;              //  Main window height
  int32_t           window_x;                   //  Main window x position
  int32_t           window_y;                   //  Main window y position
} OPTIONS;


//  General stuff.

typedef struct
{
  int32_t           process_id;                 //  This program's process ID
  char              version[128];               //  geCache version string
  uint8_t           googleearth_available;      //  true if googleearth is available (and in the PATH)
  uint8_t           ge_linked;                  //  true if geCache and Google Earth area linked
  int32_t           second_count;               //  Second counter for cache update
  double            box_size_x_deg;             //  Cache build viewing area box size in decimal degrees of longitude
  double            box_size_y_deg;             //  Cache build viewing area box size in decimal degrees of latitude
  uint8_t           poly_flag;
  uint8_t           view_flag;
  NV_F64_XYMBR      build_area_mbr;
  NV_F64_XYMBR      view_area_mbr;
  int32_t           iterations;
  int32_t           poly_iterations;
  int32_t           total_rect_time;
  int32_t           total_poly_time;
  double            x_border;
  double            y_border;
  QLabel            *meterWidth;
  QLabel            *meterHeight;
  QLabel            *rectEstTime;
  QLabel            *numBoxes;
  QLabel            *polyEstTime;
} MISC;


#endif
